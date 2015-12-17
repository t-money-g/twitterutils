#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <json/json.h>
#include <liboauthcpp/liboauthcpp.h>

using namespace std;
/* These are input settings that make this demo actually work -- you need to get
 * these, e.g. by referring to the Twitter documentation and by registering an
 * application with them. Here we have examples from Twitter. If you
 * don't enter any, you'll be prompted to enter them at runtime.
 */
string consumer_key = "comsumer_key"; // Key from Twitter
string consumer_secret = "secret"; // Secret from Twitter
string oauth_token = "token"; // User-specific token acquired by auth process
string oauth_token_secret = "secret"; // User-specific secret acquired by auth process.
// An example resource to be accessed, along with parameters for the request
string oauth_protected_resource = "https://api.twitter.com/1.1/friends/list.json";

string oauth_protected_resource_params = "count=32&screen_name=username&skip_status=true&include_user_entities=false";



std::string getUserString(std::string prompt) {
    std::cout << prompt << " ";

    std::string res;
    std::cin >> res;
    std::cout << std::endl;
    return res;
}

//
//  libcurl variables for error strings and returned data
static std::string buffer;

static std::string returnBuffer;

string getFollowers(const string &url);
string removeFollower(const string &screen_name, const string &user_id,OAuth::Client oath);

int main(int argc, char** argv) {

   // OAuth::SetLogLevel(OAuth::LogLevelDebug);

    // Initialization
    if (consumer_key.empty()) consumer_key = getUserString("Enter consumer key:");
    if (consumer_secret.empty()) consumer_secret = getUserString("Enter consumer secret:");
    // We assume you have gotten the access token. You may have e.g., used
    // simple_auth to get it.
    if (oauth_token.empty()) oauth_token = getUserString("Enter access token:");
    if (oauth_token_secret.empty()) oauth_token_secret = getUserString("Enter access token secret:");
    OAuth::Consumer consumer(consumer_key, consumer_secret);
    OAuth::Token token(oauth_token, oauth_token_secret);
    OAuth::Client oauth(&consumer, &token);
    OAuth::Client oauth2(&consumer, &token);
    // Get the query string. Note that we pass in the URL as if we were
    // accessing the resource, but *the output query string includes the
    // parameters you passed in*. Below, we append the result only to the base
    // URL, not the entire URL we passed in here.
    string oAuthQueryString =
        oauth.getURLQueryString(OAuth::Http::Get, oauth_protected_resource + "?" + oauth_protected_resource_params);

    cout << "Enter the following in your browser to access the resource: " << endl;
    cout << oauth_protected_resource << "?" << oAuthQueryString << endl;

    cout << "Doing request" << endl;
    const string url = oauth_protected_resource + "?" + oAuthQueryString;


    buffer = getFollowers(url);

    Json::Value root;
    istringstream data(buffer);
    data >> root;

    const Json::Value users = root["users"];


    for(int i = 0; i < users.size(); ++i) {
        cout << "Name:" << users[i]["name"].asString() << endl;
        cout << "user_Id:" << users[i]["user_id"].asString()  << endl;
        cout << "Screen Name:" << users[i]["screen_name"].asString()  << endl;
        returnBuffer = removeFollower(users[i]["screen_name"].asString(), users[i]["id"].asString(),oauth2);

        cout << returnBuffer <<endl;
    }

    return 0;
}

string getFollowers(const string &url) {

    auto r = cpr::Get(cpr::Url{url});

    return r.text;

}

string removeFollower(const string &screen_name, const string &user_id, OAuth::Client oauth) {



    string resource = "https://api.twitter.com/1.1/friendships/destroy.json";
    string formattedString ="screen_name=" + screen_name + "&user_id="  + user_id;
    //string formattedString ="user_id=" + user_id;
    string queryString =  oauth.getURLQueryString(OAuth::Http::Post,resource + "?" + formattedString);
    string headers = oauth.getHttpHeader(OAuth::Http::Post,resource + "?" + formattedString);
    struct curl_slist *chunk = NULL;
    string url = resource + "?" + formattedString;

    auto r = cpr::Post(cpr::Url{resource},
                       cpr::Payload{{"screen_name", screen_name},
                                    {"user_id", user_id}},
                       cpr::Header{{"Authorization", headers}}
        );

    return r.text;


}