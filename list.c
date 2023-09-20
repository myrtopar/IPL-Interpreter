#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"

listptr node_address(listptr list, char *variable)
{

    int cond;
    
    if(list == NULL){
        return NULL;
    }

    cond = strcmp(variable, list->name);

    if(cond == 0){
        return list;    //επιστρεφει δεικτη στον κομβο 
    }
    else{
        return node_address(list->next, variable);
    }
}

void add_listnode(listptr* p, char *variable, int n)              //παιρνει πρωτο ορισμα την διευθυνση της αρχης της λιστας
{    
    if(*p == NULL){
        *p = malloc(sizeof(struct listnode));
        strcpy((*p)->name, variable);
        (*p)->value = n;
        (*p)->next = NULL;
        
    }
    else{
        add_listnode(&(*p)->next, variable, n);
    }
}

void free_list(listptr list)
{
    if(list != NULL){
        free_list(list->next);
        free(list);
    }
}


void push(blockptr *ptr, long checkpoint, int tab_count)
{

    blockptr temp;
    temp = *ptr;                               //η διευθυνση της κεφαλης της λιστας πριν την εισαγωγη φυλασσεται στο templist
    *ptr = malloc(sizeof(struct blocknode));   //δημιουργειται νεος κομβος λιστας blockptr και μπαινει η διευθυνση του νεου κομβου στο περιεχομενο του ptr
    
    if(*ptr != NULL){
        (*ptr)->start = checkpoint;
        (*ptr)->tabs = tab_count;
        (*ptr)->next = temp;                   //στο μελος next του κομβου που δειχνει ο ptr βαζω τη διευθυνση της παλιας κεφαλης της λιστας
    
    }
    else{   //stack overflow
        printf("Error! Stack overflow. Could not allocate memory to create a new node");
        flag_error = 1;
    }
}

void pop(blockptr *ptr)
{    //διαγραφει τον πρωτο κομβο της λιστας
    if(*ptr != NULL){
        blockptr temp;

        temp = *ptr;
        *ptr = (temp->next);
        free(temp);
    }
    else{   //stack underflow
        printf("Error! Stack underflow. The stack is already empty\n");
        flag_error = 1;
    }
}