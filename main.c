#include <stdlib.h>
#include <stdio.h>
//#include "graph.h"
#include <time.h>
#include "llm.h"
#include <string.h>
int main() {
    srand(time(NULL));
    /*
    Graph *g = graph_init(10, UNDIRECTED);
    if (g == NULL){
        perror("Graph initalization failed");
        return 1;
    }
    g = generate_rgraph(g);
    if (g == NULL){
        perror("Graph generation failed");
        return 1;
    }
    printf("Undirected Graph\n");
    print_list_repr(g);
    free_graph(g);
    */
   char * buff[32];  int choice;

    printf("How would you like to enter graph properties\n"
           "0 - plain text (llm will analyze your input)\n"
           "1 - you will be asked about every property\n"
           "Choice: ");
           
    fgets(buff, sizeof(choice), stdin);
    if ((sscanf(buff, " %d",&choice)) == 1){
        if (choice == 0){
            char user_prompt[512] = "";
        
            char history[100000] = "";

            while (1) {
                printf("\n>>> ");
                if (fgets(user_prompt, sizeof(user_prompt), stdin) == NULL) {
                    printf("Error reading input. Exiting...\n");
                    break;
                }
                printf("AI: ");
                user_prompt[strcspn(user_prompt, "\n")] = '\0';  // Remove newline (ruins json)

                
                ask_llm(user_prompt, history);
            }
        }
        else if (choice == 1){
            Graph *g = graph_init(5, DIRECTED);
        if (g == NULL){
            perror("Graph initalization failed");
            return 1;
        }
        g = generate_rgraph(g);
        if (g == NULL){
            perror("Graph generation failed");
            return 1;
        }
        printf("Undirected Graph\n");
        print_list_repr(g);
        free_graph(g);

        }
    }
   
    
    return 0;
}