#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
#include <liboauthcpp/liboauthcpp.h>
#include <cstdio>

using namespace std;
/* These are input settings that make this demo actually work -- you need to get
 * these, e.g. by referring to the Twitter documentation and by registering an
 * application with them. Here we have examples from Twitter. If you
 * don't enter any, you'll be prompted to enter them at runtime.
 */
string consumer_key = "KsiiDpyIPVXjB6R1FRhSKm5o2"; // Key from Twitter
string consumer_secret = "slkiEnfQJhGJrUHyhgjtN0HBpPPpreFfYYPSKlMNlLGkS00gWZ"; // Secret from Twitter
string oauth_token = "8791292-2cjzZIJMoH0PKce5gFEgE0S6tUG9sO4Lk5fwEqPu7a"; // User-specific token acquired by auth process
string oauth_token_secret = "JdP26qUB0ivjgRJ3OOW1YyEQiOSNxodXXdlUNLKnzoJf7"; // User-specific secret acquired by auth process.
// An example resource to be accessed, along with parameters for the request
string oauth_protected_resource = "https://api.twitter.com/1.1/friends/list.json";

string oauth_protected_resource_params = "count=200&screen_name=tyoungjr&skip_status=true&include_user_entities=false";



std::string getUserString(std::string prompt) {
    std::cout << prompt << " ";

    std::string res;
    std::cin >> res;
    std::cout << std::endl;
    return res;
}

//
//  libcurl variables for error strings and returned data
static char errorBuffer[CURL_ERROR_SIZE];
static std::string buffer;

static std::string returnBuffer;

//
//  libcurl write callback function
//
static int writer(char *data, size_t size, size_t nmemb,
                  std::string *writerData)
{
    if (writerData == NULL)
        return 0;

    writerData->append(data, size*nmemb);

    return size * nmemb;
}

void getFollowers(const string &url );
void removeFollower(const string &screen_name, const string &user_id,OAuth::Client oath);

int main(int argc, char** argv) {

    OAuth::SetLogLevel(OAuth::LogLevelDebug);

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


    getFollowers(url);
    Json::Value root;

    istringstream data(buffer);
    data >> root;

    const Json::Value users = root["users"];


    for(int i = 0; i < users.size(); ++i) {
        cout << "Name:" << users[i]["name"].asString() << endl;
        cout << "user_Id:" << users[i]["user_id"].asString()  << endl;
        cout << "Screen Name:" << users[i]["screen_name"].asString()  << endl;
    }
    cout << "json String" << buffer << endl;


    removeFollower(users[0]["screen_name"].asString(), users[0]["id"].asString(),oauth2);

    cout << returnBuffer <<endl;
    return 0;
}

void getFollowers(const string &url) {

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);


    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL,url.c_str());
        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
#ifndef SKIP_PEER_VERIFICATION

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifndef SKIP_HOSTNAME_VERIFICATION

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif


        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);


        }

        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
        }

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    errorBuffer);
        }

/* always cleanup */
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();


}

void removeFollower(const string &screen_name, const string &user_id, OAuth::Client oauth) {
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    string resource = "https://api.twitter.com/1.1/friendships/destroy.json";
    string formattedString ="screen_name=" + screen_name + "&user_id="  + user_id;
    //string formattedString ="user_id=" + user_id;
    string queryString =  oauth.getURLQueryString(OAuth::Http::Post,resource + "?" + formattedString);
    string headers = oauth.getHttpHeader(OAuth::Http::Post,resource + "?" + formattedString);
    struct curl_slist *chunk = NULL;
    string url = resource + "?" + formattedString;
    if(curl) {

        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);



         //chunk = curl_slist_append(chunk, "Authorization: OAuth");
        //chunk = curl_slist_append(chunk, oauth.getHttpHeader());
        /* Remove a header curl would otherwise add by itself */
        chunk = curl_slist_append(chunk, headers.c_str());

        /* set our custom set of headers */
         res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        char *output = curl_easy_escape (curl,formattedString.c_str(),formattedString.size() + 0);

        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        /* Now specify the POST data */
        //escape the string
        curl_easy_setopt(curl, CURLOPT_POST, 1);

        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, output);

#ifndef SKIP_PEER_VERIFICATION

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifndef SKIP_HOSTNAME_VERIFICATION

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
        }
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &returnBuffer);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
        }

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);

        /* free the custom headers */
        curl_slist_free_all(chunk);

        curl_free(output);
    }
    curl_global_cleanup();
}