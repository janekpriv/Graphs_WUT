#include<stdlib.h>
#include<stdio.h>

#include "graph.h"

void get_input(Graph*g, int nodes_count){

    int first_node;
    int second_node;
    int edge_counter = 0;
    int c = 0;
    int i;

    printf("Now add edges by entering ids of nodes in order\n");
    printf("f.e: 1 2 will result in 1->2 edge\nto stop adding edges type '-1 -1'\n");

    while(1){

        if(edge_counter>=(nodes_count-1)*nodes_count/2 && g->type == UNDIRECTED){
            printf("you have reached maximum number of edges\n");
            break;
        }else if(edge_counter>=(nodes_count-1)*nodes_count&& g->type == DIRECTED){
            printf("you have reached maximum number of edges\n");
            break;
        }
        scanf("%d %d", &first_node, &second_node);
        if(first_node == -1){
            break;
        }   

        if (first_node < 0 || first_node >= nodes_count || second_node < 0 || second_node >= nodes_count) {
            fprintf(stderr, "Invalid node index. Must be between 0 and %d.\n", nodes_count - 1);
            continue;
        }
        Node node_1, node_2;
        if ((i = contains(first_node, g->nodes, c)) == -1){
            node_1 = create_Node(first_node);
            g->nodes[c] = node_1;
            g->nodes[c++]->links =malloc(((g->n)-1)* sizeof(struct Node *));
        }else{
            node_1 = g->nodes[i];
        }
        if ((i = contains(nodes_count, g->nodes, c)) == -1){
            node_2 = create_Node(second_node);
            g->nodes[c] =node_2;
            g->nodes[c++]->links =malloc(((g->n)-1)* sizeof(struct Node *));
        }else{
            node_2 = g->nodes[i];
        }
        if(node_1 && node_2){
            link_nodes(node_1, node_2);

            if(g->type == UNDIRECTED){
                link_nodes(node_2, node_1);
        
            }
        }
        edge_counter++;
    }
    


}