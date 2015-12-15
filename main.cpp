#include <iostream>
#include <curl/curl.h>
#include <json/json.h>
#include <liboauthcpp/liboauthcpp.h>


using namespace std;
//Consumer Key (API Key)	wR3kNNiSqaCmLqTUIUsTupElu
//Consumer Secret (API Secret)	mQUgZyXjIstjcUBzy5SodFwRF6f3LrlT6dpmPaUqXwVeaU5uyL

std::string consumer_key = "KsiiDpyIPVXjB6R1FRhSKm5o2"; // Key from Twitter
std::string consumer_secret = "slkiEnfQJhGJrUHyhgjtN0HBpPPpreFfYYPSKlMNlLGkS00gWZ"; // Secret from Twitter
std::string request_token_url = "https://api.twitter.com/oauth/request_token";
std::string request_token_query_args = "oauth_callback=oob";
std::string authorize_url = "https://api.twitter.com/oauth/authorize";
std::string access_token_url = "https://api.twitter.com/oauth/access_token";


int main() {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://twitter.com/");

#ifndef SKIP_PEER_VERIFICATION

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifndef SKIP_HOSTNAME_VERIFICATION

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}