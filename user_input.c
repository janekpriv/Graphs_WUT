#include<stdlib.h>
#include<stdio.h>

#include "graph_generator.h"

void get_input(graph *g){

    int nodes_count;
    int first_node;
    int second_node;

    printf("Graph generator\n----------------\nChoose graph properties: \n");
    printf("How many nodes should graph have: ");
    scanf("%d", &nodes_count);
    printf("\nNow add edges by entering ids of nodes in order\n");
    printf("f.e: 1 2 will result in 1->2 edge\n to stop adding edges type '-1'\n");

    while(0){
        scanf("%d %d", first_node, second_node);
        if(first_node == -1){
            break;
        }   

        if(!check_if_edge_exists(g)){
            add_edge(g, first_node, second_node);
        }else{
            fprintf(stderr, "this edge already exists, try adding another one");
            continue;
        }
    }


}