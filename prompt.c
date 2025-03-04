#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<curl/curl.h>


void send_query(char *prompt) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    if (curl) {
        char *url = "http://127.0.0.1:1234/v1/completions";
        char post_data[512];

        snprintf(post_data, sizeof(post_data),
                 "{ \"model\": \"llama-3.2-1b-instruct\", \"prompt\": \"%s\", \"max_tokens\": 50 }",
                 prompt);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url); // Set url address for http request
        curl_easy_setopt(curl, CURLOPT_POST, 1L); // Set POST method instead of GET
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data); // Set post_data
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // Set HTTP headers
        //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parse_and_print_response); // Set default output function (from stdout to parse function)

        res = curl_easy_perform(curl);

        printf("%s", res);

        if (res != CURLE_OK) {
            fprintf(stderr, "ERROR: %s\n", curl_easy_strerror(res));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}