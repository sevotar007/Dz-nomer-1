#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct n{
    int val;
    struct n* next;
} node;

typedef struct{  
    node* start;
    node* end;
    int size;
} query;

node* create_node (int val){
    node* new_node = (node*) malloc(sizeof(node));
    new_node->val = val;
    new_node->next = NULL;
    return new_node;
}

query* createQuery(void){
    query* new_query = (query*) malloc(sizeof(query));
    new_query->start = new_query->end = NULL;
    new_query->size = 0;
    return new_query;
}

void queryPush(int val,query* query){
    node* tmp = create_node(val);
    if(!query->size){
        query->start = query->end = tmp;
    } else {
        query->end->next = tmp;
        query->end = tmp;
    }
    ++query->size;
}

int queryPop(query* query){
    if (!query->size){
        printf("Error:query is empty\n");
        return 0;
    }
    int result = query->start->val;
    node* tmp = query->start;
    query->start = query->start->next;
    free(tmp);
    --query->size;
    return result;
}

bool queryIsEmpty(query* query){
    if (query->size){
        return false;
    } else {
        return true;
    }
}

int querySize(query* query){
    return query->size;
}

// end realization of query

void help(void){
    printf("Commands for work with query:\n");
    printf("1 - push in query integer value.\n");
    printf("2 - pop from query integer value.\n");
    printf("3 - size of query.\n");
    printf("4 - query is empty.\n");
    printf("0 - exit from programm.\n");
}

int main(void) {
    query* main_query = createQuery();
    help();
    int input = 1, value;
    while(input){
        printf("Input command: ");
        scanf("%d",&input);
        switch(input){
            case 0: break;
            case 1:
                printf("Input value: ");
                scanf("%d", &value);
                queryPush(value, main_query);
                break;
            case 2:
                printf("Value from query: %d\n", queryPop(main_query));
                break;
            case 3:
                printf("Size of query: %d\n",querySize(main_query));
                break;
            case 4:
                printf("Query is ");
                if (queryIsEmpty(main_query)){
                    printf("empty\n");
                } else {
                    printf("not empty\n");
                }
                break;
            default:
                help();
        }
    }
    return 0;
