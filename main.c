#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include "graph_generator.h"

int main(int argc, char **argv){

    int min = 0;
    int max = 10;

    srand(time(NULL));

    int number_of_nodes = rand() % (max - min +1)+ min;

    int number_of_edges = rand() % (number_of_nodes*number_of_nodes - number_of_nodes) + number_of_nodes;

    graph *g  = create_graph(number_of_nodes);
    
    //nested forloop for more than one connections, -> number of edges from each node
    for(int i = 0; i<number_of_nodes; i++){
        int random_node = rand() % (number_of_nodes +1);
        add_edge(g, i, random_node);
    }


    print_graph(g);
    
    return 0;
}