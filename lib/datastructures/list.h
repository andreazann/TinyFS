//
// Created by Marco Speziali on 01/07/2017.
//

#ifndef TINYFS_LIST_H
#define TINYFS_LIST_H

#include "../macros.h"

typedef struct listnode_s {
    struct listnode_s *prev;
    struct listnode_s *next;

    void *value;
} listnode_t;

#define NODE_ALLOC(node, p, n, v)                        \
    do {                                                    \
        (node) = NEW(listnode_t);                         \
        (node)->prev = (p);                                 \
        (node)->next = (n);                                 \
        (node)->value = (v);                                \
    } while(0)

typedef struct list_s {
    struct listnode_s *head;
    struct listnode_s *tail;
    int count;
} list_t;

#define LIST_ALLOC(list)                                    \
    do {                                                    \
        (list) = NEW(list_t);                               \
        (list)->head = NULL;                                \
        (list)->tail = NULL;                                \
        (list)->count = 0;                                  \
    } while(0)

#define LIST_INIT(list)                                     \
    do {                                                    \
        (list)->head = NULL;                                \
        (list)->tail = NULL;                                \
        (list)->count = 0;                                  \
    } while(0)

#define LIST_FOREACH(list, elem) \
        for ((elem) = (list)->head; (elem) != NULL; (elem) = (elem)->next)

typedef int (*comp)(void *, void *);

listnode_t *__listadd(list_t *list, void *value);
#define listadd(list, value) __listadd(list, (void *)value)

listnode_t *listaddnode(list_t *list, listnode_t *node);

listnode_t *__listfind(list_t *list, void *value, comp cmp);
#define listfind(list, value, cmp) __listfind(list, (void *)value, cmp)

int __listindexof(list_t *list, void *value, comp cmp);
#define listindexof(list, value, cmp) __listindexof(list, (void *)value, cmp)

listnode_t *listat(list_t *list, int index);

listnode_t *__listdelete(list_t *list, void *value, comp cmp);
#define listdelete(list, value, cmp) __listdelete(list, (void *)value, cmp)
listnode_t *listdeletenode(list_t *list, listnode_t * node);

void listclear(list_t *list, void (*freeContent)(void *));

#endif //TINYFS_LIST_H
