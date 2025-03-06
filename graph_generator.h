#ifndef GENERATE_GRAPH_H
#define GENERATE_GRAPH_H

typedef struct node{

    int id;
    struct node *next;
}node;

typedef struct graph{

    int nodes_count;
    struct graph **adjacency;

}graph;

node *create_node(int id);
graph *create_graph(int count);
void add_edge(graph *g, int a, int b);
void add_two_way_edge(graph *g, int a, int b);
void print_graph(graph *g);






#endif