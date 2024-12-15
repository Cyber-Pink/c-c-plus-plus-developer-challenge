#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stdlib.h>

// Enum to keep track of the status of the functions run. (made a different one specific for lists)
typedef enum {
    LIST_NO_ERROR,
    LIST_MEMORY_ALLOCATION_FAILED,
    LIST_NOT_INITIALIZED,
    LIST_INVALID_ARGUMENT,
    LIST_ERROR_UNKNOWN
} LIST_STATUS;




typedef struct Node_struct
{
    //Node holds data of any type, so the type of data needs to be void pointer.
    void *data;

    //The pointer to the next Node on the chain of the linked list
    struct Node_struct *next_node;

}Node_type;


typedef struct 
{
    Node_type *list_start;
    Node_type *list_end;
    unsigned int List_size;

}List;


LIST_STATUS Create_List(List **list);

LIST_STATUS Add_Node(List *list, void *data);

LIST_STATUS Destroy_List(List *list);

#endif
