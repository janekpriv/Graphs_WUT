#include <stdio.h>
#include <curl/curl.h>
#include <cjson/cJSON.h> 
#define DEBUG
int main(){
    CURL *curl;
    CURLcode response;

    char payload[1200];
    char * system_prompt =  "Your purpose is to extract graph information from the user. Dismiss unrelated questions."  
                            "You need to establish the number of nodes, whether the graph should be random or manual."
                            "If user have to specifies connections between nodes it's a manual graph."
                            "If you are missing one of the informations, you have to alert the user."
                            "You must spot errors, mismatches and alert the user if the number of nodes specified doesn’t match the unique nodes in the connections."
                            "You have two streams: 'u' for user replies (to clarify info or report errors),"
                            "and 'd' for extracted data, the final result, that should never be included in 'u' responses."
                            "Each response starts with 'u' or 'd' followed by a '\\n' and nothing else."
                            "When there is no errors and you don't need to contact the user, make a 'd' response."
                            "For 'd' responses: the second line is the number of nodes, third line is either 'r' for random graphs or 'm' for manual graphs."
                            "If it's a manual graph, next lines must represent edges as a pair of numbers (e.g., '0 1'). No extra text is allowed."
                            "The 'u' response should be regular text message to the user.";
    char * user_prompt = "make a graph ";
    snprintf(payload, sizeof(payload),
                 "{ \"messages\": [{\"role\": \"system\", \"content\": \"%s\"}, {\"role\": \"user\", \"content\": \"%s\"}] }",system_prompt, user_prompt);
    #ifdef DEBUG
    puts(payload);
    #endif

    curl_global_init(CURL_GLOBAL_ALL);
    struct curl_slist *list = NULL;
    curl = curl_easy_init();

    if(curl) {
        // set up http connection
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:1234/v1/chat/completions" );
        list = curl_slist_append(list, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // send data from the local stack 
        curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, payload);

        response = curl_easy_perform(curl);
                
        if(response != CURLE_OK){
            fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(response));
        }
        curl_slist_free_all(list); 
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}