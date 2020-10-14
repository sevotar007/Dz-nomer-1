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


bool check_connecions(qraph* current_qraph){
    for (uint64_t i = 0; i < current_qraph->size; ++i){
        uint64_t lines = 0;
        for (uint64_t j = 0; j < current_qraph->size; ++j){
            if(current_qraph->qraph[i][j])
                ++lines;
        }
        if(!lines)
            return false;
    }
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
    //printf("%s\n", term);
    system(term);
    free(term);
    delete_qraph(qraph);
    return 0;
}
