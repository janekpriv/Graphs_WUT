#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include "graph_generator.h"

node *create_node(int id){
    node *new = malloc(sizeof(node));
    new->id = id;
    new->next = NULL;

    return new;
}

graph *create_graph(int count){

    graph *g = malloc(sizeof(graph)*count);
    g->nodes_count = count;
    g->adjacency = malloc(sizeof(node)*count);

    for(int i = 0; i<count; i++){
        g->adjacency[i]=NULL;
    }
    return g;
}
// a and b has to be within count range, starting from 0, no way to name graphs
void add_two_way_edge(graph *g, int a, int b){
    //edge from b to a

    node *new = create_node(a);
    new->next = g->adjacency[b];
    g->adjacency[b] = new;

    //edge from a to b
    node *new_ = create_node(b);
    new_->next = g->adjacency[a];
    g->adjacency[a] = new_;

}

void add_edge(graph *g, int a, int b){
    //edge from a to b
    node *new = create_node(b);
    new->next = g->adjacency[a];
    g->adjacency[a] = new;
}



void print_graph(graph *g){
    for(int i = 0; i<g->nodes_count; i++){
        node *tmp = g->adjacency[i];
        printf("\nNode: %d\n", i);
        printf("%d: ", i);
        while(tmp){
            printf("%d -> ", tmp->id);
            tmp = tmp->next;
        }
    }
    printf("\n");
}

int check_if_edge_exists(graph *g, int a, int b) {
    node *current = g->adjacency[a];
    while (current) {
        if (current->id == b) {
            return 1;
        }
        current = current->next;
    }
    return 0; 
}


void generate_random_graph(){

    int min = 0;
    int max = 100;

    srand(time(NULL));

    int number_of_nodes = rand() + (max - min +1)+ min;

    int number_of_edges;
}