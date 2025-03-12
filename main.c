#include <stdlib.h>
#include <stdio.h>
//#include "graph.h"
#include <time.h>
#include "llm.h"
#include <string.h>

#include "user_input.h"
#include "graph_generator.h"
int main() {
    srand(time(NULL));
    
    
   char * buff[32];  int choice;

    printf("How would you like to enter graph properties?\n\n"
           "1 - plain text (llm will analyze your input)\n"
           "2 - you will be asked about every property\n\n"
           "Choice [1-2]: ");
           
    fgets(buff, sizeof(choice), stdin);
    if ((sscanf(buff, " %d",&choice)) == 1){
        if (choice == 1){
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

                
                char * res = ask_llm(user_prompt, history);
                if (res == NULL)
                    break;
            }
        }
        else if (choice == 2){
            int nodes_count, mode;
            printf("\n\nGraph generator\n----------------\nChoose graph properties: \n\n");
            printf("Should the graph be \n1. random\n2. not random\n\nChoice [1-2]: ");
            scanf("%d", &mode);
            printf("\nHow many nodes should the graph have: ");
            scanf("%d", &nodes_count);
            Graph *g;
            if (mode == 1){
                int type;
                printf("\nShould the graph be directed or undirected?\n\n1. directed\n2.undirected\n\nChoice [1-2]: ");
                scanf("%d", &type);
                if (type == 1)
                    g = graph_init(nodes_count, DIRECTED);
                else if (type == 2)
                    g = graph_init(nodes_count, UNDIRECTED);

                if (g == NULL){
                    perror("Graph initalization failed");
                    return 1;
                }
                g = generate_rgraph(g);
                if (g == NULL){
                    perror("Graph generation failed");
                    return 1;
                    
                }
                print_list_repr(g);
                free_graph(g);
            }
            else if (mode == 2){
                int type;
                printf("\nShould the graph be directed or undirected?\n\n1. directed\n2.undirected\n\nChoice [1-2]: ");            
                scanf("%d", &type);
                if (type == 1){
                    g = graph_init(nodes_count, DIRECTED);
                    get_input(g, nodes_count);
                }
                else if (type == 2){
                    g = graph_init(nodes_count, UNDIRECTED);
                    get_input(g, nodes_count);
                }
    
                print_list_repr(g);
            }
        }
    }
   
    
    return 0;
}