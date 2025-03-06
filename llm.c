#include <stdio.h>
#include <curl/curl.h>
#include <cjson/cJSON.h> 
#include <stdlib.h>
#include <string.h>
#include "llm.h"
#include "graph.h"
#undef DEBUG


Graph *parse_chat_reply(char rep[]){
    char reply_type;

    rep++; // skip quotation mark
    rep[strlen(rep)-1] = '\0'; // remove closing quotation mark

    #ifdef DEBUG
        char *ex = "d10 m 1 2 2 3 4 10    ";
        rep = ex;
    #endif

    int nodes = 0;
    char gtype;

    sscanf(rep, "%c", &reply_type); 
    rep++; 

    if (reply_type == 'd'){
        sscanf(rep, "%d %c", &nodes, &gtype);
        Graph *g = graph_init(nodes, UNDIRECTED);
        if (g == NULL){
            perror("Graph initalization failed");
            return NULL;
        }
        if (gtype == 'm'){
            while (*(rep++) != 'm' && *rep != 'r') ; // move text 
            char * cursor = rep;
            int c = 0; int i;
            while (*cursor != '\0'){
                long int n1 = strtol(cursor, &cursor, 10);
                long int n2 = strtol(cursor, &cursor, 10);
                while (*cursor == ' ') // prevent infinite loop
                cursor++;
                // segm fault somewhere there
                Node nd1, nd2;
                if (c < (g->n -1)){
                    if ((i = contains(n1, g->nodes, c)) == -1){
                        nd1 = create_node(n1);
                        g->nodes[c++] = nd1;
                    }else{ 
                        nd1 = g->nodes[i];
                    }
                    if ((i = contains(n2, g->nodes, c)) == -1){
                        nd2 = create_node(n2);
                        g->nodes[c++]  = nd2;
                    }else{
                        nd2 = g->nodes[i];
                    }
                    if (nd1 && nd2)
                        link_nodes(nd1, nd2);
                }
                
            }
            g->n = c; // actual number of nodes
            
        }
        else if (gtype == 'r'){
            fill_nodes_0_to_1(g);
            g = generate_rgraph(g);
            if (g == NULL){
                perror("Graph generation failed");
                return NULL;
            }
            
        }
        printf("Here is your graph based on the description you provided.\n");
        return g;
    }
    else if (reply_type == 'u'){
        printf("%s", rep);
      
    }
    return NULL;
}

char *ask_llm(char * user_prompt, char *history){
    CURL *curl;
    curl_global_init(CURL_GLOBAL_ALL);
    struct curl_slist *list = NULL;
    curl = curl_easy_init();

    char payload[100000]; 
    
    char * system_prompt =  "Your purpose is to derive graph information from the talk with the user. Dismiss unrelated questions."  
                            "You need to establish the number of nodes, whether the graph should be random or manually defined."
                            "Ensure the number of nodes is more than zero."
                            "If user specifies connections between nodes it's a manual graph."
                            "If you are missing one of the informations, you have to alert the user."
                            "You must spot errors, mismatches and alert the user if the number of nodes specified doesn’t match the unique nodes in the connections."
                            "You have two streams: 'u' for user replies (to clarify info or report errors),"
                            "and 'd' for extracted data, the final result, that should never be included in 'u' responses."
                            "Each response starts with 'u' or 'd', you must include them (e.g., 'uPlease specify the type of graph')."
                            "When there is no errors and you don't need to contact the user, make a 'd' response."
                            "You can make a 'd' response only when you have gathered all information"
                            "For 'd' responses: you put number of nodes, next you put letter 'r' for random graphs or 'm' for manual graphs, separating each parameter with a space."
                            "If it's a manual graph, place edges as a pair of numbers (e.g., '0 1 2 3' meaning 0 connected to 1, 2 to 3). No extra text is allowed."
                            "Example of a correct 'd' response - 'd5 m 1 2 3 2 4 1 4 5; or 'd100 r'"
                            "The 'u' response should be regular casual text message to the user."
                            "Manual graph REQUIRES specifying connections between nodes. You cant proceed without gathering data about connections unless it's a random graph."
                            "Ask the user for information he didn't provide before proceeding";
    char user_prompt_obj[2048];
    
    if (curl){
        // set up http connection
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:1234/v1/chat/completions" );
        list = curl_slist_append(list, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
    } else return NULL;
    
    snprintf(user_prompt_obj, sizeof(user_prompt_obj), "{\"role\": \"user\", \"content\": \"%s\"}", user_prompt);
    
    snprintf(payload, sizeof(payload),
                "{ \"messages\": [{\"role\": \"system\", \"content\": \"%s\"}, %s %s]}",system_prompt, history, user_prompt_obj);
    
    char * chat_reply = send_query(payload, curl);

    // add to history
    strcat(history, user_prompt_obj); // add latest prompt object to history
    strcat(history, ", ");
    
    strcat(history, chat_reply); // add chat reply to history
    strcat(history, ", ");
    
    
    curl_slist_free_all(list); 
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return history;
}

    
char * send_query(char * payload, CURL *curl){
    struct memory chunk = {0};
    CURLcode response;

    // send data to this function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    // send data from the local stack 
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, payload);

    response = curl_easy_perform(curl);
    if(response != CURLE_OK)
        fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(response));

    // parse the JSON data 
    cJSON *json = cJSON_Parse(chunk.response); 
    if (json == NULL) { 
        const char *error_ptr = cJSON_GetErrorPtr(); 
        if (error_ptr != NULL) { 
            printf("Error: %s\n", error_ptr); 
        } 
        cJSON_Delete(json); 
        return NULL; 
    } 
    // access the JSON data 
    cJSON *choices = cJSON_GetObjectItemCaseSensitive(json, "choices"); 
    cJSON *choices_arr = cJSON_GetArrayItem(choices, 0);
    cJSON *message_obj = cJSON_GetObjectItemCaseSensitive(choices_arr, "message");


    cJSON *content = cJSON_GetObjectItemCaseSensitive(message_obj,"content");
    
    char chat_reply[216];
    strncpy(chat_reply, cJSON_Print(content), sizeof(chat_reply) - 1);
    //cJSON_Delete(json);

    Graph *g = parse_chat_reply(chat_reply);
    if (g != NULL)
        print_list_repr(g);
    return cJSON_Print(message_obj);
    
}


static size_t write_callback(char *data, size_t size, size_t nmemb, void *clientp)
{
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *)clientp;
   
    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if(!ptr)
      return 0;  /* out of memory */
   
    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;
   
    return realsize;
}
