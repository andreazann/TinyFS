//
// Created by Marco Speziali on 01/07/2017.
//

#ifndef TINYFS_HASHTABLE_H
#define TINYFS_HASHTABLE_H

#include "../macros.h"
#include "list.h"
#include <stdlib.h>

typedef struct keyvaluepair_s {
    void *key;
    void *value;
    bool deleted;
} keyvaluepair_t;

#define KEYVALUEPAIR(key, value) (keyvaluepair_t) { (key), (value), false }

#define KEYVALUEPAIR_ALLOC(kp, _key, _value)                                \
    (kp) = NEW(keyvaluepair_t);                                             \
    (kp)->key = strdup(_key);                                               \
    (kp)->value = (_value);                                                 \
    (kp)->deleted = false;

typedef int (*closedhashingfunction)(void *);
typedef int (*openhashingfunction)(void *, int);
typedef enum hashingmode_e { CLOSED, OPEN } hashingmode_t;

typedef struct hashtable_s {
    list_t **closedbucket;
    keyvaluepair_t **openbucket;

    int bucketsize;
    int occupied;

    hashingmode_t mode;

    closedhashingfunction closedhashingfunction;
    openhashingfunction openhashingfunction;
} hashtable_t;

#define HASHTABLE_INIT_OPEN(ht, _size, hashingfn)                           \
    (ht) = NEW(hashtable_t);                                                \
    (ht)->bucketsize = (_size);                                             \
    (ht)->occupied = 0;                                                     \
    (ht)->mode = OPEN;                                                      \
    (ht)->openbucket = NEW_ARRAY(keyvaluepair_t*, (ht)->bucketsize);        \
    (ht)->openhashingfunction = (hashingfn);                                \
    (ht)->closedbucket = NULL;                                              \
    (ht)->closedhashingfunction = NULL;

#define HASHTABLE_INIT_CLOSED(ht, _size, hashingfn)                         \
    (ht) = NEW(hashtable_t);                                                \
    (ht)->bucketsize = (_size);                                             \
    (ht)->occupied = 0;                                                     \
    (ht)->mode = CLOSED;                                                    \
    (ht)->closedbucket = NEW_ARRAY(list_t*, (ht)->bucketsize);              \
    (ht)->closedhashingfunction = (hashingfn);                              \
    (ht)->openbucket = NULL;                                                \
    (ht)->openhashingfunction = NULL;

#define HASHTABLE_LOAD_FACTOR(ht) DOUBLE((DOUBLE((ht)->occupied) / DOUBLE((ht)->bucketsize)))

#define HASHTABLE_FOREACH(ht, __pair, __code)               \
    do {                                                    \
        if (ht->occupied == 0) {                            \
            break;                                          \
        }                                                   \
                                                            \
        if (ht->mode == OPEN) {                             \
            int __i;                                        \
            for (__i = 0; __i < ht->bucketsize; __i++) {    \
                keyvaluepair_t *__p = ht->openbucket[__i];  \
                if (__p != NULL && !__p->deleted) {         \
                    __pair = ht->openbucket[__i];           \
                    do __code while(0);                     \
                }                                           \
            }                                               \
        }                                                   \
        else {                                              \
            int __i;                                        \
            for (__i = 0; __i < ht->bucketsize; __i++) {    \
                list_t *__l = ht->closedbucket[__i];        \
                if (__l != NULL && __l->count != 0) {       \
                    listnode_t *__n;                        \
                                                            \
                    LIST_FOREACH(__l, __n) {                \
                        __pair = __n->value;                \
                        do __code while(0);                 \
                    }                                       \
                }                                           \
            }                                               \
        }                                                   \
    } while(0)

#define HASHTABLE_FOREACH_KEY(ht, __key, __code)            \
    do {                                                    \
        keyvaluepair_t *__pair_in;                          \
        HASHTABLE_FOREACH(ht, __pair_in, {                  \
            __key = __pair_in->key;                         \
            do __code while(0);                             \
        });                                                 \
    } while(0)

#define HASHTABLE_FOREACH_VALUE(ht, __val, __code)          \
    do {                                                    \
        keyvaluepair_t *__pair_in;                          \
        HASHTABLE_FOREACH(ht, __pair_in, {                  \
            __val = __pair_in->value;                       \
            do __code while(0);                             \
        });                                                 \
    } while(0)


typedef int (*comparator)(void *, void *);

void hashtableclear(hashtable_t *table, void (*freeKey)(void *), void (*freeValue)(void *));
bool hashtablecontains(hashtable_t *table, void *key, comparator cmp);
void *hashtableput(hashtable_t *table, void *key, void *value, comparator cmp);
void *hashtableget(hashtable_t *table, void *key, comparator cmp);
void *hashtableremove(hashtable_t *table, void *key, comparator cmp);

#endif //TINYFS_HASHTABLE_H