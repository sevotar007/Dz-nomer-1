#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct{
    uint64_t** qraph;
    uint64_t size;
    bool isWeighted;
    bool isDirected;
} qraph;

qraph* create_qraph(uint64_t size){
    qraph* new_qraph = (qraph*)malloc(sizeof(qraph));
    new_qraph->size = size;
    new_qraph->isWeighted = false;
    new_qraph->qraph = (uint64_t**)malloc(size*sizeof(uint64_t*));
    for (uint64_t i = 0; i < size; ++i)
        new_qraph->qraph[i] = (uint64_t*)calloc(size, sizeof(uint64_t));
    return new_qraph;
}

void add_edge(qraph* current_qraph, uint64_t from, uint64_t to){
    current_qraph->qraph[from][to] = 1;
    if (!current_qraph->isDirected) {
        current_qraph->qraph[to][from] = 1;
    }
}

uint64_t find_minimal_power(qraph* current_qraph){
    uint64_t pos = -1 , power = -1;
    for (uint64_t i = 0; i < current_qraph->size; ++i){
        uint64_t count_power = 0;
        for (uint64_t j = 0; j < current_qraph->size; ++j){
            if (i == j && current_qraph->qraph[i][j]) count_power += 2;
            else {
                if (current_qraph->qraph[i][j]) {
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

void delete_qraph(qraph* current_qraph){
    for (uint64_t i = 0; i < current_qraph->size; ++i)
        free(current_qraph->qraph[i]);
    free(current_qraph->qraph);
    free(current_qraph);
}

bool check_direction(qraph* current_qraph){
    for (uint64_t i = 0; i < current_qraph->size; ++i){
        for (uint64_t j = i; j < current_qraph->size; ++j){
            if (current_qraph->qraph[i][j] != current_qraph->qraph[j][i]){
                return true;
            }
        }
    }
    return false;
}

void input_qraph(qraph* current_qraph){
    printf("Input matrix:");
    for (uint64_t i = 0; i < current_qraph->size; ++i)
        for (uint64_t j = 0; j < current_qraph->size; ++j){
            scanf("%" SCNu64 "", &current_qraph->qraph[i][j]);
            if (current_qraph->qraph[i][j]>>1)
                current_qraph->isWeighted = true;
        }
    current_qraph->isDirected = check_direction(current_qraph);
}

uint64_t print_dot(FILE* fileptr, qraph* current_qraph){
    if (current_qraph->isDirected) fprintf(fileptr, "di");
	fprintf(fileptr, "graph graphname {\n");
    uint64_t lines = 0;
    for (uint64_t i = 0; i < current_qraph->size; ++i){
        uint64_t j = (current_qraph->isDirected) ? 0 : i;
        for (; j < current_qraph->size; ++j){
            if (current_qraph->qraph[i][j]){
                fprintf(fileptr, "\t%" PRIu64 " -", i);
                if (current_qraph->isDirected)
                    fputc('>', fileptr);
                else
                    fputc('-', fileptr);
                fprintf(fileptr,  " %" PRIu64 "", j);
                if(current_qraph->isWeighted)
                    fprintf(fileptr, " [label=%" PRIu64 "]", current_qraph->qraph[i][j]);
                fprintf(fileptr, ";\n");
                ++lines;
            }
        }
    }
    fprintf(fileptr, "}");
    return lines;
}

uint64_t dfs(qraph* current_qraph, bool* color,uint64_t pos){
    uint64_t quantity_of_lines = 1;
    color[pos] = true;
    for (uint64_t i = 0; i < current_qraph->size; ++i) {
        if (current_qraph->qraph[pos][i] && !color[i]){
            quantity_of_lines += dfs(current_qraph, color, i);
        }
    }
    return quantity_of_lines;
}

bool check_connecions(qraph* current_qraph) {
    bool* color = (bool*)calloc(current_qraph->size, sizeof(bool));
    uint64_t quantity_of_lines = dfs(current_qraph,color,0);
    free(color);
    if (quantity_of_lines < current_qraph->size)
        return false;
    else 
        return true;
}

int main(int argc, char *argv[]){
    printf("Quantity of centers:\n");
    uint64_t quantity;
    scanf("%" SCNu64 "", &quantity);
    qraph* qraph = create_qraph(quantity);
    input_qraph(qraph);
    if (check_connecions(qraph)) {
        printf("Qraph is connecting\n");//
    } else {
        printf("Qraph is not connecting\n");
    }
    uint64_t add_edges, from, to;
    puts("How much add edges?");
    scanf("%" SCNu64 "", &add_edges);
    for (uint64_t i = 0; i < add_edges; ++i){
        scanf("%" SCNu64 " %"SCNu64 "", &from, &to);
        add_edge(qraph, from, to);
        puts("Edge is added in graph");
    }
    printf("Minimal power of vertex is %" PRIu64 "\n", find_minimal_power(qraph));
    char* file = (char*) malloc(256*sizeof(char));
    printf("File name(minimum 4 symbols):\n");
    scanf("%s",file);
    FILE* f = fopen(file, "w");
    print_dot(f, qraph);
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
    delete_qraph(qraph);
    return 0;
}
