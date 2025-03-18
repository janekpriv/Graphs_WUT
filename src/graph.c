#include <stdlib.h>
#include <stdio.h>
#include "../lib/graph.h"
#undef DEBUG
int contains(int id, Node *list, int size) {
    for (int i = 0; i < size; i++) {
        if (list[i]->id == id) {
            return i;
        }
    }
    return -1;
}

Node create_Node(int id){
    Node v = malloc(sizeof(struct Node));
    if (!v) return NULL;
    v->id = id;
    v->ne = 0;
    v->links = NULL;
    return v;
}



Graph * graph_init(int n, GraphType type) {
    Graph *g = malloc(sizeof(Graph));
    if (g == NULL) return NULL;

    g->type = type;
    g->n = n;

    g->nodes = malloc(n * sizeof(Node*));
    if (g->nodes == NULL) {
        free(g);
        return NULL;
    }
    
    return g;
}

void fill_nodes_0_to_1(Graph *g){
    for (int i = 0; i < g->n; i++) {
        g->nodes[i] = create_Node(i);
        g->nodes[i]->links = malloc(((g->n)-1) * sizeof(struct Node *));
        if (!g->nodes[i]) {
            free_graph(g);
            return NULL;
        }
    }
}

void link_nodes(Node node1, Node node2) {
    /* node1 -> node2*/
    node1->links[node1->ne] = node2;
    node1->ne++;
}


Graph *generate_rgraph(Graph *g){ 
    int i, j;
    int v = g->n;
    fill_nodes_0_to_1(g);
    int rne; // random number of links
    int re;  // random edge
    
    for (i = 0; i < v; i++){
        rne = ((rand() % (v/2)) + 1);
        
        if (g->nodes[i]->links == NULL) {
            free_graph(g);
            return NULL;
        }
        #ifdef DEBUG
                printf("\nrne = %d: ", rne);
        #endif

        int *ne = &g->nodes[i]->ne; // number of edges in the links array
        while(*ne < rne && *ne < v){
            re = i;
            int attempts = 0;
            while(re == i || contains(re, g->nodes[i]->links, g->nodes[i]->ne) != -1 ){
                re = rand() % v;
            
                if (attempts++ >= v) break;
            }

            if (attempts >= v) {
                break;
            }
            link_nodes(g->nodes[i], g->nodes[re]);
            
            if (g->type == UNDIRECTED && contains(i, g->nodes[re]->links, g->nodes[re]->ne) == -1 ){ 
                link_nodes(g->nodes[re], g->nodes[i]);
            }
            #ifdef DEBUG
                printf("%d ", re);
            #endif
            
        }
    }
    return g;
}

void print_list_repr(Graph *g){

    FILE *out = fopen("output/graph.txt", "w");
    if(out == NULL){
        fprintf(stderr, "unable to open the output file, terminating");
    }

    if (g == NULL) {
        printf("Graph is NULL\n");
        return;
    }
    printf("Adjacency List Representation:\n");
    for (int i = 0; i < g->n; i++) {
        printf("%d: ", g->nodes[i]->id);
        fprintf(out,"%d: ", g->nodes[i]->id);
        for (int j = 0; j < g->nodes[i]->ne; j++) {
            printf("%d ", g->nodes[i]->links[j]->id);
            fprintf(out, "%d ", g->nodes[i]->links[j]->id);
    
        }
        printf("\n");
        fprintf(out,"\n");
    }
}


void free_graph(Graph *g) {
    if (!g) return;

    for (int i = 0; i < g->n; i++) {
        if (g->nodes[i] != NULL) {
            if (g->nodes[i]->links != NULL) {
                free(g->nodes[i]->links);
            }
            free(g->nodes[i]);
        }
    }
    if (g->nodes != NULL) {
        free(g->nodes);
    }
    free(g);
}