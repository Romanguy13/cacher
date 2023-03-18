/*
    This is an implementation of a linked list that I based on my previous work for another class.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "List.h"

// private Node type
typedef struct NodeObj *Node;

// private NodeObj type
typedef struct NodeObj {
    void *data;
    Node next;
    Node prev;
} NodeObj;

// private ListObj type
typedef struct ListObj {
    Node front;
    Node back;
    Node curs;
    int length;
    int indx;
} ListObj;

// --- constructors destructors ---

// Creates and returns a new empty List.
List newList(void) {
    List new = (List) malloc(sizeof(ListObj));
    new->front = NULL;
    new->back = NULL;
    new->curs = NULL;
    new->length = 0;
    new->indx = -1;
    return new;
}

Node newNode(void *x) {
    Node new = (Node) malloc(sizeof(NodeObj));
    new->data = x;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

void freeNode(Node *pN) {
    if (pN != NULL && *pN != NULL) {
        free(*pN);
        *pN = NULL;
    }
}

void freeList(List *pL) {
    if (pL != NULL && *pL != NULL) {
        clear(*pL);
        free(*pL);
        *pL = NULL;
    }
}

// --- access ---

// Length of list L
int length(List L) {
    return L->length;
}

// current index of list L cursor, or -1
int indexC(List L) {
    if (L->curs == NULL) {
        return -1;
    }
    return L->indx;
}

// returns front item of list L
void *front(List L) {
    if (length(L) <= 0) {
        fprintf(stderr, "List Error: front() on empty list\n");
        exit(EXIT_FAILURE);
    }
    return L->front->data;
}

// return the item that the cursor is pointing to, or NULL
void *get(List L) {
    if (length(L) <= 0) {
        fprintf(stderr, "List Error: get() on empty list\n");
        exit(EXIT_FAILURE);
    }
    if (indexC(L) < 0) {
        fprintf(stderr, "List Error: get() with no cursor location\n");
        exit(EXIT_FAILURE);
    }
    if (L->curs == NULL) {
        return NULL;
    }
    return L->curs->data;
}

// --- manipulation ---

// clears list L
void clear(List L) {
    while (length(L) > 0) {
        deleteFront(L);
    }
}

// sets the element of cursor to x
void set(List L, void *x) {
    if (length(L) <= 0) {
        fprintf(stderr, "List Error: set() on empty list\n");
        exit(EXIT_FAILURE);
    }
    if (indexC(L) < 0) {
        fprintf(stderr, "List Error: set() with no cursor location\n");
        exit(EXIT_FAILURE);
    }
    L->curs->data = x;
}

// moves cursor to index 0 of list L
void moveFront(List L) {
    if (L->front != NULL) {
        L->curs = L->front;
        L->indx = 0;
    }
}

// increments cursor on list L
// if cursor is already at the last element, cursor index gets set to -1
void moveNext(List L) {
    if (L->curs != NULL) {
        L->curs = L->curs->next;
        L->indx++;
        if (L->indx >= L->length) {
            L->indx = -1;
        }
    }
}

// append element to list L
void append(List L, void *x) {
    Node to_add = newNode(x);
    if (length(L) == 0) {
        L->front = to_add;
        L->back = to_add;
        L->length++;
    } else {
        L->back->next = to_add;
        to_add->prev = L->back;
        L->back = to_add;
        L->length++;
    }
}

// delete the front element from list L
void deleteFront(List L) {
    if (length(L) <= 0) {
        fprintf(stderr, "List Error: deleteFront() on empty list\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 1) {
        freeNode(&L->front);
        L->length = 0;
        L->curs = NULL;
        L->indx = -1;
        L->front = NULL;
        L->back = NULL;
    } else {
        if (indexC(L) == 0) {
            L->curs = NULL;
            L->indx = -1;
        }
        L->front = L->front->next;
        freeNode(&L->front->prev);
        L->front->prev = NULL;
        L->length--;
        L->indx--;
    }
}

// delete back element of list L
void deleteBack(List L) {
    if (length(L) <= 0) {
        fprintf(stderr, "List Error: deleteBack() on empty list\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 1) {
        freeNode(&L->back);
        L->length = 0;
        L->curs = NULL;
        L->indx = -1;
        L->front = NULL;
        L->back = NULL;
    } else {
        if (indexC(L) == length(L) - 1) {
            L->curs = NULL;
            L->indx = -1;
        }
        L->back = L->back->prev;
        freeNode(&L->back->next);
        L->back->next = NULL;
        L->length--;
    }
}

// delete item at cursor in list L
void delete (List L) {
    if (length(L) <= 0) {
        fprintf(stderr, "List Error: delete() on empty list\n");
        exit(EXIT_FAILURE);
    }
    if (indexC(L) < 0) {
        fprintf(stderr, "List Error: delete() with no cursor location\n");
        exit(EXIT_FAILURE);
    }
    if (indexC(L) == 0) {
        deleteFront(L);
    } else if (indexC(L) == length(L) - 1) {
        deleteBack(L);
    } else {
        L->curs->prev->next = L->curs->next;
        L->curs->next->prev = L->curs->prev;
        freeNode(&L->curs);
        L->curs = NULL;
        L->indx = -1;
        L->length--;
    }
}

// prints lists to specified output IO stream
void printList(FILE *out, List L) {
    if (L == NULL || length(L) == 0) {
        return;
    }
    moveFront(L);
    fprintf(out, "%s", (char *) get(L));
    moveNext(L);
    while (indexC(L) != -1) {
        fprintf(out, " %s", (char *) get(L));
        moveNext(L);
    }
    fprintf(out, "\n");
}
