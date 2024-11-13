//
// Created by Marco Speziali on 08/07/2017.
//

#ifndef TINYFS_GENERAL_H
#define TINYFS_GENERAL_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "hashing/functions.h"
#include "../../lib/datastructures/hashtable.h"
#include "../types/tree.h"

#define STRCMP_COMP (int (*)(void *, void *))strcmp
#define INODE_HASHTABLE_INIT(hashtable) HASHTABLE_INIT_OPEN(hashtable, MAX_CHILDREN, hashingfunction)

void freeValue(void *value);
char *strconcat(char *s1, char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1); //+1 for the zero-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
int _strcmp_qsort (const void * a, const void * b ) {
    const char *pa = *(const char**)a;
    const char *pb = *(const char**)b;

    return strcmp(pa,pb);
}

inode_t *movetopath(listnode_t *path, inode_t *root, char *dirsep) {
    listnode_t *pathiter = path;
    inode_t *iterator = root;

    if (strcmp(pathiter->value, dirsep)) {
        pathiter = pathiter->next;
    }

    while (pathiter != NULL && iterator != NULL) {
        if ((iterator)->f_type == F && pathiter->next != NULL) {
            return NULL;
        }

        (iterator) = hashtableget((iterator)->children, pathiter->value, STRCMP_COMP);
        pathiter = pathiter->next;
    }

    return iterator;
}

#define RETURN_FREE(__components, __inode)                                      \
    do {                                                                        \
        if (__components != NULL) {                                             \
            listclear(__components, NULL);                                      \
            free(__components);                                                 \
        }                                                                       \
        inode_t *node = __inode == NULL ? NULL : __inode;                       \
        if (node != NULL) {                                                     \
            free((node)->filename);                                             \
            free((node)->f_content);                                            \
            if ((node)->children != NULL) {                                     \
                hashtableclear((node)->children, free, freeValue);              \
            }                                                                   \
            free((node)->children);                                             \
            free(node);                                                         \
        }                                                                       \
    } while(0);                                                                 \
    return

char *readline() {
    char *line = NULL;
    size_t size;

    if (getline(&line, &size, stdin) == -1) {
        return NULL;
    }
    else {
        return line;
    }
}

#endif //TINYFS_GENERAL_H
