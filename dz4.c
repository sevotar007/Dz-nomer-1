#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct{
    uint64_t** graph;
    uint64_t size;
    bool isWeighted;
    bool isDirected;
} graph;

graph* create_graph(uint64_t size){
    graph* new_graph = (graph*)malloc(sizeof(graph));
    new_graph->size = size;
    new_graph->isWeighted = false;
    new_graph->graph = (uint64_t**)malloc(size*sizeof(uint64_t*));
    for (uint64_t i = 0; i < size; ++i)
        new_graph->graph[i] = (uint64_t*)calloc(size, sizeof(uint64_t));
    return new_graph;
}

void add_edge(graph* current_graph, uint64_t from, uint64_t to){
    current_graph->graph[from][to] = 1;
    if (!current_graph->isDirected) {
        current_graph->graph[to][from] = 1;
    }
}

uint64_t find_minimal_power(graph* current_graph){
    uint64_t pos = -1 , power = -1;
    for (uint64_t i = 0; i < current_graph->size; ++i){
        uint64_t count_power = 0;
        for (uint64_t j = 0; j < current_graph->size; ++j){
            if (i == j && current_graph->graph[i][j]) count_power += 2;
            else {
                if (current_graph->graph[i][j]) {
                    ++count_power;
                }
            }
        }
        //printf("%" PRIu64 " %" PRIu64 "\n", i, count_power);
        if (count_power < power) {
            power = count_power;
            pos = i;
        }
    }
    //return pos;
    return power;
}

void delete_graph(graph* current_graph){
    for (uint64_t i = 0; i < current_graph->size; ++i)
        free(current_graph->graph[i]);
    free(current_graph->graph);
    free(current_graph);
}

bool check_direction(graph* current_graph){
    for (uint64_t i = 0; i < current_graph->size; ++i){
        for (uint64_t j = i; j < current_graph->size; ++j){
            if (current_graph->graph[i][j] != current_graph->graph[j][i]){
                return true;
            }
        }
    }
    return false;
}

void input_graph(graph* current_graph){
    printf("Input matrix:");
    for (uint64_t i = 0; i < current_graph->size; ++i)
        for (uint64_t j = 0; j < current_graph->size; ++j){
            scanf("%" SCNu64 "", &current_graph->graph[i][j]);
            if (current_graph->graph[i][j]>>1)
                current_graph->isWeighted = true;
        }
    current_graph->isDirected = check_direction(current_graph);
}

uint64_t print_dot(FILE* fileptr, graph* current_graph){
    if (current_graph->isDirected) fprintf(fileptr, "di");
	fprintf(fileptr, "graph graphname {\n");
    uint64_t lines = 0;
    for (uint64_t i = 0; i < current_graph->size; ++i){
        uint64_t j = (current_graph->isDirected) ? 0 : i;
        for (; j < current_graph->size; ++j){
            if (current_graph->graph[i][j]){
                fprintf(fileptr, "\t%" PRIu64 " -", i);
                if (current_graph->isDirected)
                    fputc('>', fileptr);
                else
                    fputc('-', fileptr);
                fprintf(fileptr,  " %" PRIu64 "", j);
                if(current_graph->isWeighted)
                    fprintf(fileptr, " [label=%" PRIu64 "]", current_graph->graph[i][j]);
                fprintf(fileptr, ";\n");
                ++lines;
            }
        }
    }
    fprintf(fileptr, "}");
    return lines;
}

uint64_t dfs(graph* current_graph, bool* color,uint64_t pos){
    uint64_t quantity_of_lines = 1;
    color[pos] = true;
    for (uint64_t i = 0; i < current_graph->size; ++i) {
        if (current_graph->graph[pos][i] && !color[i]){
            quantity_of_lines += dfs(current_graph, color, i);
        }
    }
    return quantity_of_lines;
}

bool check_connecions(graph* current_graph) {
    bool* color = (bool*)calloc(current_graph->size, sizeof(bool));
    uint64_t quantity_of_lines = dfs(current_graph,color,0);
    free(color);
    if (quantity_of_lines < current_graph->size)
        return false;
    else 
        return true;
}

int main(int argc, char *argv[]){
    printf("Quantity of centers:\n");
    uint64_t quantity;
    scanf("%" SCNu64 "", &quantity);
    graph* graph = create_graph(quantity);
    input_graph(graph);
    if (check_connecions(graph)) {
        printf("Qraph is connecting\n");//
    } else {
        printf("Qraph is not connecting\n");
    }
    uint64_t add_edges, from, to;
    puts("How much add edges?");
    scanf("%" SCNu64 "", &add_edges);
    for (uint64_t i = 0; i < add_edges; ++i){
        scanf("%" SCNu64 " %"SCNu64 "", &from, &to);
        add_edge(graph, from, to);
        puts("Edge is added in graph");
    }
    printf("Minimal power of vertex is %" PRIu64 "\n", find_minimal_power(graph));
    char* file = (char*) malloc(256*sizeof(char));
    printf("File name(minimum 4 symbols):\n");
    scanf("%s",file);
    FILE* f = fopen(file, "w");
    print_dot(f, graph);
    fclose(f);

    char* term = (char*)calloc(500, sizeof(char));

    strcat(term, "dot -Tpng ");
    strcat(term, file);
    strcat(term, " -o ");
    uint64_t filelen = strlen(file);
    file[filelen - 1] = 'g';
    file[filelen - 2] = 'n';
    file[filelen - 3] = 'p';
    file[filelen - 4] = '.';
    strcat(term, file);
    printf("%s\n", term);
    //system(term);
    free(term);
    delete_graph(graph);
    return 0;
}
