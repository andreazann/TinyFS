//
// Created by Marco Speziali on 01/07/2017.
//

#ifndef TINYFS_PATHS_H
#define TINYFS_PATHS_H

#include <stdlib.h>
#include <string.h>
#include "datastructures/list.h"

list_t *pathcomponents(const char *path, const char dirsep) {
    char separator[2];
    separator[0] = dirsep;
    separator[1] = '\0';

    char *temppath = NEW_ARRAY(char, strlen(path) + 1);
    strcpy(temppath, path);

    list_t *list;
    LIST_ALLOC(list);
    listadd(list, separator);

    char *component = strtok(temppath, separator);
    listadd(list, component);

    while ((component = strtok(NULL, separator)) != NULL) {
        listadd(list, component);
    }

    free(temppath);

    return list;
}

#endif //TINYFS_PATHS_H
