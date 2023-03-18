#include "List.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int compulsoryMisses = 0;
int capacityMisses = 0;

bool addFifo(List cache, List evicted, void *line, int cacheSize);

bool addLru(List cache, List evicted, void *line, int cacheSize);

bool addClock(List cache, List evicted, List refBit, void *line, int cacheSize, int *clockHand);

int contains(List cache, void *line);

int main(int argc, char *argv[]) {
    int cacheSize = 0;
    char *policy = NULL;
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-N") == 0) {
            cacheSize = atoi(argv[i + 1]);
            i++;
        } else {
            policy = argv[i];
        }
    }
    if (policy == NULL) {
        fprintf(stderr, "Error: No policy specified\n");
        exit(1);
    } else if (cacheSize == 0) {
        fprintf(stderr, "Error: No cache size specified\n");
        exit(1);
    }
    if (strcmp(policy, "-F") != 0 && strcmp(policy, "-L") != 0 && strcmp(policy, "-C") != 0) {
        fprintf(stderr, "Error: Invalid policy specified\n");
        exit(1);
    }

    List lines = newList();
    List evicted = newList();
    List cache = newList();
    List refBit = newList();

    int clockHand = 0;

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, stdin) != NULL) {
        char *newLine = malloc(sizeof(char) * (strlen(line) + 1));
        strcpy(newLine, line);
        newLine[strlen(line) - 1] = '\0';
        append(lines, (void *) newLine);
    }
    // add each line to the cache

    moveFront(lines);
    while (indexC(lines) >= 0) {
        bool hit = false;
        if (strcmp(policy, "-F") == 0) {
            hit = addFifo(cache, evicted, (void *) get(lines), cacheSize);
        } else if (strcmp(policy, "-L") == 0) {
            hit = addLru(cache, evicted, get(lines), cacheSize);
        } else if (strcmp(policy, "-C") == 0) {
            hit = addClock(cache, evicted, refBit, get(lines), cacheSize, &clockHand);
        }
        if (!hit) {
            printf("MISS\n");
        } else {
            printf("HIT\n");
        }
        moveNext(lines);
    }
    printf("%d %d\n", compulsoryMisses, capacityMisses);
    freeList(&lines);
    freeList(&evicted);
    freeList(&cache);
    freeList(&refBit);

    return 0;
}

bool addFifo(List cache, List evicted, void *line, int cacheSize) {
    if (length(cache) == 0) {
        append(cache, line);
        compulsoryMisses++;
        return false;
    } else if (contains(cache, line) != -1) {
        return true;
    } else if (length(cache) < cacheSize) {
        append(cache, line);
        compulsoryMisses++;
        return false;
    } else {
        if (contains(evicted, line) != -1) {
            capacityMisses++;
        } else {
            compulsoryMisses++;
            append(evicted, front(cache));
        }
        deleteFront(cache);
        append(cache, line);
        return false;
    }
}

bool addLru(List cache, List evicted, void *line, int cacheSize) {
    if (length(cache) == 0) {
        append(cache, line);
        compulsoryMisses++;
        return false;
    } else if (contains(cache, line) != -1) {
        moveFront(cache);
        while (indexC(cache) >= 0) {
            if (strcmp((char *) get(cache), (char *) line) == 0) {
                break;
            }
            moveNext(cache);
        }
        void *temp = get(cache);
        delete (cache);
        append(cache, temp);
        return true;
    } else if (length(cache) < cacheSize) {
        append(cache, line);
        compulsoryMisses++;
        return false;
    } else {
        if (contains(evicted, line) != -1) {
            capacityMisses++;
        } else {
            compulsoryMisses++;
            append(evicted, front(cache));
        }
        deleteFront(cache);
        append(cache, line);
        return false;
    }
}

void increment(int *clockHand, int cacheSize) {
    if (*clockHand == cacheSize - 1) {
        *clockHand = 0;
    } else {
        *clockHand = *clockHand + 1;
    }
}

bool addClock(List cache, List evicted, List refBit, void *line, int cacheSize, int *clockHand) {
    if (length(cache) == 0) {
        append(cache, line);
        append(refBit, (void *) 0);
        compulsoryMisses++;
        return false;
    } else if (contains(cache, line) != -1) {
        moveFront(cache);
        moveFront(refBit);
        while (indexC(cache) >= 0) {
            if (strcmp((char *) get(cache), (char *) line) == 0) {
                break;
            }
            moveNext(cache);
            moveNext(refBit);
        }
        set(refBit, (void *) 1);
        return true;
    } else if (length(cache) < cacheSize) {
        append(cache, line);
        append(refBit, (void *) 0);
        compulsoryMisses++;
        return false;
    } else {
        while (true) {
            moveFront(cache);
            moveFront(refBit);
            for (int i = 0; i < *clockHand; i++) {
                moveNext(cache);
                moveNext(refBit);
            }
            if (get(refBit) == (void *) 1) {
                set(refBit, (void *) 0);
                increment(clockHand, cacheSize);
            } else {
                if (contains(evicted, line) != -1) {
                    capacityMisses++;
                } else {
                    compulsoryMisses++;
                    append(evicted, get(cache));
                }
                set(cache, line);
                set(refBit, (void *) 0);
                increment(clockHand, cacheSize);
                return false;
            }
        }
    }
}

int contains(List cache, void *line) {
    if (length(cache) == 0) {
        return -1;
    }
    moveFront(cache);
    while (indexC(cache) >= 0) {
        if (strcmp((char *) get(cache), (char *) line) == 0) {
            return indexC(cache);
        }
        moveNext(cache);
    }
    moveFront(cache);
    return -1;
}
