#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef enum {
    UNDIRECTED,
    DIRECTED
} GraphType;

typedef struct node {
    int id;
    int ne;
    struct node **links;
} *Node;

typedef struct graph {
    GraphType type;
    int n;  
    Node *nodes;
} Graph;

int contains(int, Node*, int);
Node create_node(int);
Graph *generate_rgraph(Graph*);
void print_list_repr(Graph*);
void free_graph(Graph*);
Graph * graph_init(int, GraphType type);
void link_nodes(Node, Node);
#endif