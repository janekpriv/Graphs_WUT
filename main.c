#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include "graph_generator.h"

int main(int argc, char **argv){

    int min = 0;
    int max = 10;

    srand(time(NULL));

    int number_of_nodes = rand() + (max - min +1)+ min;

    graph *g  = create_graph(2);

    add_edge(g, 0, 1);
    add_edge(g, 1, 0);

    print_graph(g);
    
    return 0;
}