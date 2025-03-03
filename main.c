#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include "graph_generator.h"
#include "user_input.h"

int main(int argc, char **argv){



    int nodes_count;
    int first_node;
    int second_node;
    int edge_counter = 0;

    printf("Graph generator\n----------------\nChoose graph properties: \n");
    printf("How many nodes should graph have: ");
    
    scanf("%d", &nodes_count);
    
    graph *g  = create_graph(nodes_count);
    
    get_input(g, nodes_count);
    
    print_graph(g);
    
    return 0;
}