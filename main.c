#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include "graph_generator.h"

int main(int argc, char **argv){

    // int min = 0;
    // int max = 10;

    // srand(time(NULL));

    // int number_of_nodes = rand() % (max - min +1)+ min;

    // int number_of_edges = rand() % (number_of_nodes*number_of_nodes - number_of_nodes) + number_of_nodes;

    // graph *g  = create_graph(number_of_nodes);
    
    // //nested forloop for more than one connections, -> number of edges from each node
    // for(int i = 0; i<number_of_nodes; i++){
    //     int random_node = rand() % (number_of_nodes +1);
    //     add_edge(g, i, random_node);
    //     add_edge(g, random_node, i); 
    // }
    

    int nodes_count;
    int first_node;
    int second_node;
    int edge_counter = 0;

    printf("Graph generator\n----------------\nChoose graph properties: \n");
    printf("How many nodes should graph have: ");
    scanf("%d", &nodes_count);
    graph *g  = create_graph(nodes_count);
    printf("Now add edges by entering ids of nodes in order\n");
    printf("f.e: 1 2 will result in 1->2 edge\nto stop adding edges type '-1 -1'\n");

    while(1){

        if(edge_counter>=(nodes_count-1)*nodes_count){
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

        if(!check_if_edge_exists(g, first_node, second_node)){
        add_edge(g, first_node, second_node);
        }else{
           fprintf(stderr, "this edge already exists, try adding another one\n");
           continue;
        }
        edge_counter++;
    }
    print_graph(g);
    
    return 0;
}