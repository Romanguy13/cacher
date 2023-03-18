#ifndef LIST_H_INCLUDE_
#define LIST_H_INCLUDE_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct ListObj *List;

List newList(void);

void freeList(List *pL);

int length(List L);

int indexC(List L);

void *front(List L);

void *get(List L);

void clear(List L);

void set(List L, void *x);

void moveFront(List L);

void moveNext(List L);

void append(List L, void *x);

void deleteFront(List L);

void deleteBack(List L);

void delete (List L);

void printList(FILE *out, List L);

#endif
