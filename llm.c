#include <stdio.h>
#include <curl/curl.h>
#include <cjson/cJSON.h> 
#include <stdlib.h>
#include <string.h>
#include "llm.h"
#include "graph.h"

Graph *parse_chat_reply(char rep[]){
    char reply_type;

    rep++; // skip quotation mark
    rep[strlen(rep)-1] = '\0'; // remove closing quotation mark

    int nodes = 0;
    char gtype, gcreation;
    Graph *g;
    if (((sscanf(rep, "%c", &reply_type)) == 1) && (reply_type == 'd' || reply_type == 'u')){
        rep++; 

        if (reply_type == 'd'){
            if (((sscanf(rep, "%d %c %c", &nodes, &gcreation, &gtype)) == 3) && (gcreation == 'm' || gcreation == 'r')&& (gtype == 'd' || gtype == 'u')){
                
                if (gtype == 'u'){
                    g = graph_init(nodes, UNDIRECTED);
                }else if (gtype == 'd'){
                    g = graph_init(nodes, DIRECTED);
                }
                if (g == NULL){
                    perror("Graph initalization failed");
                    return NULL;
                }
                
                if (gcreation == 'm'){
                    while ((*(rep) != 'd') && (*rep != 'u')) {
                        rep++; // move text 
                    }
                    rep++;
                    
                    char * cursor = rep;
                    
                    int c = 0; int i;
                    while (*cursor != '\0'){
                        long int n1 = strtol(cursor, &cursor, 10);
                        long int n2 = strtol(cursor, &cursor, 10);
                        while (*cursor == ' ') // prevent infinite loop
                        cursor++;
                        
                        Node nd1, nd2;
                        if (c <= (g->n)){
                            if ((i = contains(n1, g->nodes, c)) == -1){
                                nd1 = create_Node(n1);
                                g->nodes[c] = nd1;
                                g->nodes[c++]->links = malloc(((g->n)-1) * sizeof(struct Node *)); // fixed: g->nodes[i] was accessing -1 element and causing segm fault
                                if (!g->nodes[i]) {
                                    free_graph(g);
                                return NULL;
                                }
                            }else{ 
                                nd1 = g->nodes[i];
                            }
                            if (c <= (g->n) && (i = contains(n2, g->nodes, c)) == -1){
                                nd2 = create_Node(n2);
                                g->nodes[c]  = nd2;
                                g->nodes[c++]->links = malloc(((g->n)-1) * sizeof(struct Node *));
                                if (!g->nodes[i]) {
                                    free_graph(g);
                                return NULL;
                                }
                            }else{
                                nd2 = g->nodes[i];
                            }
                            if (nd1 && nd2){
                                link_nodes(nd1, nd2);
                                if (g->type == UNDIRECTED){
                                    link_nodes(nd2, nd1);
                                }
                            }
                        }
                    }
                    g->n = c; // actual number of nodes
                }
            
                else if (gcreation == 'r'){

                    g = generate_rgraph(g);
                    if (g == NULL){
                        perror("Graph generation failed");
                        return NULL;
                    }
                    
                }
                printf("Here is your graph based on the description you provided.\n\n");
                return g;
            }
            
        }
        else if (reply_type == 'u'){
            printf("%s", rep);
            return NULL;
        }
    }
    printf("Something went wrong. Try again.");

    return NULL;
}

char *ask_llm(char * user_prompt, char *history){
    CURL *curl;
    curl_global_init(CURL_GLOBAL_ALL);
    struct curl_slist *list = NULL;
    curl = curl_easy_init();

    char payload[100000]; 
    
    char * system_prompt =  "Your purpose is to derive graph information from the talk with the user. Dismiss unrelated questions."  
                            "You need to establish the number of nodes, whether the graph should be random"
                            "Ensure the number of nodes is more than zero."
                            "If user specifies connections between nodes it's not a random graph."
                            "You have two streams: 'u' for user replies (to clarify info or report errors),"
                            "and 'd' for extracted data, the final result, that should never be included in 'u' responses."
                            "Each response starts with 'u' or 'd', you must include them (e.g., 'uPlease specify the type of graph')."
                            "When there is no errors and you don't need to contact the user, make a 'd' response."
                            "You can make a 'd' response only when you have gathered all information"
                            "The 'd' response format looks like this: (<> represent placeholder and slash means either one if the letters)'<d//u><int> <m//r> <d//u> <n1> <n2> ...' "
                            ": first you put letter 'u' or 'd', without a space you must put number of nodes, "
                            "Example of a correct 'd' response - 'd5 m u 1 2 3 2 4 1 4 5; or 'd100 r d'"
                            "You can't mix responses 'u' and 'd' together - one is for data other is meant for the user."
                            "next you separate each parameter with spaces:  put letter 'r' for random graphs or 'm' for manual graphs and after that "
                            "you must put the letter 'd' for directed graph or 'u' for undirected"
                            "If it's not a random graph, place edges as a pair of numbers (e.g., '0 1 2 3' meaning 0 connected to 1, 2 to 3). No extra text is allowed."
                            "Always set graph to be undirected, unless user mentions he wants a directed graph"
                            "The  response starting with 'u' should be regular casual text message to the user."
                            "You cant proceed without gathering data about connections unless it's a random graph."
                            "Ask the user for information he didn't provide before proceeding"
                            "Anticipate that user might express connections using arrow symbol, dashes (e.g., '1->2' or '1-2') or detailed verbal descriptions (e.g. 'node 1 connects to node 2')."
                            "Avoid duplicates. (e.g. '1->2 2<-1 1->2' should result in only one '1 2' )"
                            "If there is a direction, first node in an edge have be to the node that points to the other node (e.g. '1->2' and '2<-1' should result both in '1 2')";
                            
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
