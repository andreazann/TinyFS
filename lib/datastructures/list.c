//
// Created by Marco Speziali on 01/07/2017.
//

#include "list.h"
#include <stdlib.h>

listnode_t *__listadd(list_t *list, void *value) {
    listnode_t *node = NEW(listnode_t);
    node->value = value;
    return listaddnode(list, node);
}

listnode_t *listaddnode(list_t *list, listnode_t *node) {
    if (list->head == NULL) {
        node->prev = NULL;
        node->next = NULL;
        list->head = list->tail = node;
    }
    else {
        node->prev = list->tail;
        node->next = NULL;
        list->tail->next = node;
        list->tail = node;
    }

    list->count++;

    return node;
}


listnode_t *__listfind(list_t *list, void *value, comp cmp) {
    listnode_t *iter = (list)->head;
    listnode_t *node = NULL;

    if (iter != NULL) {
        do {
            if (cmp(value, iter->value) == 0) {
                (node) = iter;
                break;
            }

            iter = iter->next;
        } while(iter != NULL);
    }

    return node;
}

int __listindexof(list_t *list, void *value, comp cmp) {
    listnode_t *iter = (list)->head;
    int index = 0;

    if (iter == NULL) {
        (index) = -1;
    }
    else {
        do {
            if (cmp(value, iter->value) == 0) {
                break;
            }

            index++;
            iter = iter->next;
        } while(iter != NULL);

        index = -1;
    }

    return index;
}

listnode_t *listat(list_t *list, int index) {
    if (list->count <= index || index < 0) {
        return NULL;
    }

    listnode_t *iter = (list)->head;

    if (iter != NULL) {
        int i;
        for (i = 0; i <= index; i++) {
            iter = iter->next;
        }
    }

    return iter;
}

listnode_t *__listdelete(list_t *list, void *value, comp cmp) {
    listnode_t *victim = listfind(list, value, cmp);
    return listdeletenode(list, victim);
}

listnode_t *listdeletenode(list_t *list, listnode_t * node) {
    if (node == list->head) {
        list->head->prev = NULL;
        list->head = list->head->next;
    }
    else if (node == list->tail) {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    else {
        listnode_t *prev = node->prev;
        listnode_t *next = node->next;
        prev->next = next;
        next->prev = prev;
    }
    list->count--;

    return node;
}

void listclear(list_t *list, void (*freeContent)(void *)) {
    listnode_t *iterator = list->head, *next;

    while (iterator != NULL) {
        next = iterator->next;

        if (freeContent != NULL) {
            freeContent(iterator->value);
        }

        free(iterator);
        iterator = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}