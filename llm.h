
#ifndef __LLM_H__
#define __LLM_H__


#include <stdlib.h>
#include <curl/curl.h>
#include "graph.h"

Graph* parse_chat_reply(char rep[]);
char * send_query(char * payload, CURL *curl);
char * ask_llm(char *, char*);

struct memory {
    char *response;
    size_t size;
  };

static size_t write_callback(char *data, size_t size, size_t nmemb, void *clientp);

#endif