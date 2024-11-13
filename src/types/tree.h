//
// Created by Marco Speziali on 01/07/2017.
//

#ifndef TINYFS_TYPES_H
#define TINYFS_TYPES_H

#include "../../lib/datastructures/hashtable.h"

typedef enum filetype_e {
    F = 0 << 2,
    D = 1 << 2
} filetype_t;

typedef struct inode_s {
    struct inode_s *parent;

    char *filename;
    filetype_t f_type;

    char *f_content;

    hashtable_t *children;
} inode_t;

#define INODE_ALLOC_DIR(__dir, __name, __parent)                    \
    (__dir) = NEW(inode_t);                                         \
    (__dir)->parent = (__parent);                                   \
    (__dir)->filename = strdup(__name);                             \
    (__dir)->f_content = NULL;                                      \
    (__dir)->f_type = D;                                            \
    INODE_HASHTABLE_INIT((__dir)->children);

#define INODE_ALLOC_FILE(__file, __name, __parent)                  \
    (__file) = NEW(inode_t);                                        \
    (__file)->parent = (__parent);                                  \
    (__file)->filename = strdup(__name);                            \
    (__file)->f_content = NULL;                                     \
    (__file)->f_type = F;                                           \
    (__file)->children = NULL;

typedef struct itree_s {
    inode_t *root;
} itree_t;

#define ITREE_INIT_OPEN(__tree, __filetype, __filename, __maxchildren, __hashingfn)                 \
    do {                                                                                            \
        (__tree) = NEW(itree_t);                                                                    \
        (__tree)->root = NEW(inode_t);                                                              \
        (__tree)->root->f_type = (__filetype);                                                      \
        (__tree)->root->filename = (__filename);                                                    \
        (__tree)->root->parent = NULL;                                                              \
        (__tree)->root->f_content = NULL;                                                           \
        HASHTABLE_INIT_OPEN(filestructure->root->children, MAX_CHILDREN, __hashingfn);              \
    } while(0)

#define ITREE_INIT_CLOSED(__tree, __filetype, __filename, __maxchildren, __hashingfn)               \
    do {                                                                                            \
        (__tree) = NEW(itree_t);                                                                    \
        (__tree)->root = NEW(inode_t);                                                              \
        (__tree)->root->f_type = (__filetype);                                                      \
        (__tree)->root->filename = (__filename);                                                    \
        (__tree)->root->parent = NULL;                                                              \
        (__tree)->root->f_content = NULL;                                                           \
        HASHTABLE_INIT_CLOSED(filestructure->root->children, MAX_CHILDREN, __hashingfn);            \
    } while(0)

#endif //TINYFS_TYPES_H
