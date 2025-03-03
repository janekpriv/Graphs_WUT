#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include <time.h>

int main() {
    srand(time(NULL));
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

    Graph *g2 = graph_init(10, DIRECTED);
    if (g == NULL){
        perror("Graph initalization failed");
        return 1;
    }

    g2 = generate_rgraph(g2);
    if (g == NULL){
        perror("Graph generation failed");
        return 1;
    }
    printf("\nDirected Graph\n");
    print_list_repr(g2);
    free_graph(g2);

    return 0;
}