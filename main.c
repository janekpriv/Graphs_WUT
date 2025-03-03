#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include <time.h>

int main() {
    
    srand(time(NULL));
    Graph *g = graph_init(20, UNDIRECTED);
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
    

    return 0;
}