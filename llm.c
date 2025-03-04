#include <stdio.h>
#include <curl/curl.h>
#include <cjson/cJSON.h> 

int main(){
    CURL *curl;
    CURLcode response;

    // create a cJSON object 
    cJSON *json_payload = cJSON_CreateObject(); 
    cJSON *messages_arr = cJSON_CreateArray();
    cJSON *messages_obj = cJSON_CreateObject(); 
    cJSON_AddStringToObject(messages_obj, "role", "user");
    cJSON_AddStringToObject(messages_obj, "content", "hello");
    cJSON_AddItemToArray(messages_arr, messages_obj);
    cJSON_AddStringToObject(json_payload, "messages", cJSON_Print(messages_arr)); 
    
    // convert the cJSON object to a JSON string 
    char *json_str = cJSON_Print(json_payload); 
    
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
        curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, json_str);

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