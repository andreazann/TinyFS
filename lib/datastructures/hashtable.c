//
// Created by Marco Speziali on 01/07/2017.
//

#include <string.h>
#include "hashtable.h"

#define CMP_EQUAL(cmp, x, y) (cmp(x, y) == 0)
typedef int (*_cmp)(void *, void *);

static _cmp keyscmp;
int listcmp(void *keypair, void *y) {
    if (keyscmp != NULL) {
        return keyscmp(((keyvaluepair_t *)keypair)->key, y);
    }
    else {
        exit(-1);
    }
}

void (*__freeKey)(void *); void (*__freeValue)(void *);
void listcontentclear(void *elem) {
    keyvaluepair_t *pair = (elem);
    __freeKey(pair->key);
    __freeKey(pair->value);
    free(elem);
}

void hashtableclear(hashtable_t *table, void (*freeKey)(void *), void (*freeValue)(void *)) {
    __freeKey = freeKey;
    __freeValue = freeValue;

    int i;
    for (i = 0; i < table->bucketsize; ++i) {
        if (table->mode == CLOSED) {
            if (table->closedbucket[i] != NULL) {
                listclear(table->closedbucket[i], listcontentclear);
                free(table->closedbucket[i]);
            }
        }
        else {
            if (table->openbucket[i] != NULL) {
                keyvaluepair_t *pair  = table->openbucket[i];
                freeKey(pair->key);
                freeValue(pair->value);
                free(pair);

                table->openbucket[i] = NULL;
            }
        }
    }

    table->occupied = 0;
}

bool hashtablecontains(hashtable_t *table, void *key, comparator cmp) {
    return hashtableget(table, key, cmp) != NULL;
}

void *hashtableput(hashtable_t *table, void *key, void *value, comparator cmp) {
    if (HASHTABLE_LOAD_FACTOR(table) == 1) {
        return NULL;
    }

    if (table->mode == CLOSED) {
        int index = table->closedhashingfunction(key);

        if (table->closedbucket[index] == NULL) {
            LIST_ALLOC(table->closedbucket[index]);
        }

        keyvaluepair_t *pair;
        KEYVALUEPAIR_ALLOC(pair, key, value);

        if (listfind(table->closedbucket[index], key, listcmp) != NULL) {
            return NULL;
        }

        listadd(table->closedbucket[index], (void *)pair);
    }
    else {
        int index = 0;
        int possibleIndex = table->openhashingfunction(key, index);

        while (table->openbucket[possibleIndex] != NULL && !table->openbucket[possibleIndex]->deleted) {
            if (CMP_EQUAL(cmp, table->openbucket[possibleIndex]->key, key)) {
                return NULL;
            }

            index++;
            possibleIndex = table->openhashingfunction(key, index);
        }

        KEYVALUEPAIR_ALLOC(table->openbucket[possibleIndex], key, value);
    }

    table->occupied++;

    return value;
}

void *hashtableget(hashtable_t *table, void *key, comparator cmp) {
    if (table->mode == CLOSED) {
        int index = table->closedhashingfunction(key);

        if (table->closedbucket[index] == NULL) {
            return NULL;
        }

        listnode_t *iterator = table->closedbucket[index]->head;

        while (iterator != NULL) {
            if (CMP_EQUAL(cmp, key, ((keyvaluepair_t *)iterator->value)->key)) {
                return ((keyvaluepair_t *)iterator->value)->value;
            }

            iterator = iterator->next;
        }
    }
    else {
        int index = 0;
        int possibleIndex = table->openhashingfunction(key, index);

        while (table->openbucket[possibleIndex] != NULL) {
            if (!table->openbucket[possibleIndex]->deleted) {
                if (CMP_EQUAL(cmp, table->openbucket[possibleIndex]->key, key)) {
                    return table->openbucket[possibleIndex]->value;
                }
            }

            index++;
            possibleIndex = table->openhashingfunction(key, index);
        }
    }

    return NULL;
}

void *hashtableremove(hashtable_t *table, void *key, comparator cmp) {
    if (table->mode == CLOSED) {
        int index = table->closedhashingfunction(key);

        if (table->closedbucket[index] == NULL) {
            return NULL;
        }

        keyscmp = cmp;
        listnode_t *node = listdelete(table->closedbucket[index], key, listcmp);
        keyscmp = NULL;

        if (node != NULL) {
            void *tempValue = node->value;

            free(node->value);
            free(node);

            table->occupied--;

            return tempValue;
        }

        return NULL;
    }
    else {
        int index = 0;
        int possibleIndex = table->openhashingfunction(key, index);

        while (table->openbucket[possibleIndex] != NULL) {
            if (!table->openbucket[possibleIndex]->deleted) {
                if (CMP_EQUAL(cmp, table->openbucket[possibleIndex]->key, key)) {
                    void *temp = table->openbucket[possibleIndex]->value;

                    table->openbucket[possibleIndex]->key = NULL;
                    table->openbucket[possibleIndex]->value = NULL;
                    table->openbucket[possibleIndex]->deleted = true;

                    table->occupied--;

                    return temp;
                }
            }

            index++;
            possibleIndex = table->openhashingfunction(key, index);
        }

        return NULL;
    }
}