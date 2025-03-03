#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#define DEBUG

int contains(int id, Node *list){
    if (list == NULL) return 0;
    while (list) {
        if (list->id == id) {

            return 1; 
            
        }
        list = list->next;
    }
    return 0;
}

Node *create_node(int id){
    Node *v = malloc(sizeof(Node));
    if (!v) return NULL;
    v->id = id;
    v->ne = 0;
    v->next = NULL;
    return v;
}

void push_node(Node **head, Node *neighbor) {
    if (*head == NULL) {
        *head = neighbor;
        return;
    }
    Node *temp = *head;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = neighbor;
}

Graph * graph_init(int n, GraphType type) {
    Graph *g = (Graph *)malloc(sizeof(Graph));
    if (g == NULL) return NULL;

    g->type = type;
    g->n = n;

    g->nodes = malloc(n * sizeof(Node*));
    if (g->nodes == NULL) {
        free(g);
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        g->nodes[i] = create_node(i);
        if (!g->nodes[i]) {
            free_graph(g);
            return NULL;
        }
    }
    return g;
}

Graph *generate_rgraph(Graph *g){ 
    int i, j;
    int v = g->n;

    int rne; // random number of edges
    int re;  // random edge
    
    for (i = 0; i < v; i++){
        //rne = (rand() % (v-1)) + 1;
        rne = rand() % (v/2 + 1) + 1;
        g->nodes[i]->ne = rne;

        #ifdef DEBUG
        printf("Node %d, rne = %d\n", i, rne);
        #endif
        
        for(j = 0; j < rne; j++){
            re = i;
            int attempts = 0;

            while (attempts < v) {  // Ensure we don't retry forever
                re = rand() % v;
                if (re != i && !contains(re, g->nodes[i]->next)) break;  // Found valid re
                attempts++;
            }
        
            if (attempts >= v) {
                // Failed to find a valid edge
                #ifdef DEBUG
                printf("  Skipped edge for %d (no valid re found after %d attempts)\n", i, attempts);
                #endif
                continue;
            }
        
            // Successfully found a valid re
            push_node(&g->nodes[i]->next, g->nodes[re]);
            g->nodes[i]->ne++;
            #ifdef DEBUG
            printf("  Added edge %d -> %d\n", i, re);
            #endif
        
            if (g->type == UNDIRECTED && !contains(i, g->nodes[re]->next)) {
                push_node(&g->nodes[re]->next, g->nodes[i]);
                #ifdef DEBUG
                printf("Added edge between %d and %d\n", i, re);
                #endif
            }
            
        }
    }
    return g;
}

void print_list_repr(Graph *g){
    if (g == NULL) {
        printf("Graph is NULL\n");
        return;
    }
    printf("Adjacency List Representation:\n");
    for (int i = 0; i < g->n; i++) {
        printf("%d: ",  g->nodes[i]->id);

        Node * h = g->nodes[i]->next;
        while(h){
            printf("%d ", h->id);
            h = h->next;
        }    
        printf("\n");
    }
}



void free_graph(Graph *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) {
        Node *current = g->nodes[i];
        while (current) {
            Node *next = current->next;
            free(current);
            current = next;
        }
    }
    free(g->nodes);
    free(g);
}
