#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include "graph_generator.h"
#include "user_input.h"
#include "prompt.h"

int main(int argc, char **argv){

    int nodes_count;
    int first_node;
    int second_node;
    int edge_counter = 0;
    int choice; 
    char prompt[1024];

    printf("How would you like to enter graph properties\n0 - plain text (llm will analyze your input)\n1 - you will be asked about every property\n");
    scanf("%d", &choice);

    if(choice == 0){
        scanf("%s", &prompt);
        send_query(prompt);

    }else{

        printf("Graph generator\n----------------\nChoose graph properties: \n");
        printf("How many nodes should graph have: ");
        
        scanf("%d", &nodes_count);
        
        graph *g  = create_graph(nodes_count);
        
        get_input(g, nodes_count);
    
        print_graph(g);
    }
    return 0;
}