#include<stdlib.h>
#include<stdio.h>

#include "graph_generator.h"

void get_input(graph *g, int nodes_count){

    int first_node;
    int second_node;
    int edge_counter = 0;

    printf("Now add edges by entering ids of nodes in order\n");
    printf("f.e: 1 2 will result in 1->2 edge\nto stop adding edges type '-1 -1'\n");

    while(1){

        if(edge_counter>=(nodes_count-1)*nodes_count){
            printf("you have reached maximum number of edges\n");
            break;
        }
        
        scanf("%d %d", &first_node, &second_node);
        if(first_node == -1){
            break;
        }   

        if (first_node < 0 || first_node >= nodes_count || second_node < 0 || second_node >= nodes_count) {
            fprintf(stderr, "Invalid node index. Must be between 0 and %d.\n", nodes_count - 1);
            continue;
        }

        if(!check_if_edge_exists(g, first_node, second_node)){
        add_edge(g, first_node, second_node);
        }else{
           fprintf(stderr, "this edge already exists, try adding another one\n");
           continue;
        }
        edge_counter++;
    }


}