#ifndef __GRAPH_H__
#define __GRAPH_H__



typedef enum {
    UNDIRECTED,
    DIRECTED
} GraphType;


typedef struct node {
    int id;
    int ne;
    struct node *next;
} Node;

typedef struct graph {
    GraphType type;
    int n;  
    Node **nodes;
} Graph;

int contains(int, Node*);
Node *create_node(int);
Graph *graph_init(int, GraphType);
Graph *generate_rgraph(Graph*);
void print_list_repr(Graph*);
void free_graph(Graph*);

void push_node(Node**, Node*);


#endif