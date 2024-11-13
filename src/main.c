#include <stdio.h>
#include "utils/command.h"
#include "types/tree.h"
#include "../lib/paths.h"
#include "../lib/logging/logginglevels.h"

#define LOGGING_LEVEL NO_LOG

#include "../lib/logging/logging.h"
#include "utils/general.h"

#define MAX_CHILDREN 1024
#define DIR_SEPARATOR '/'
#define DIR_SEPARATOR_STR "/"

#define PRINT_SUCCESS printf("ok\n")
#define PRINT_UNSUCCESS printf("no\n")
#define PRINT_FILE_CONTENT(content) printf("contenuto %s\n", ((content) == NULL ? "" : (content)))
#define PRINT_WRITTEN_CHARS(number) printf("ok %d\n", (int)(number))
#define PRINT_FIND(path) printf("ok %s\n", path)

int delete_r(inode_t *node);
int find_r(char *filename, inode_t *node, list_t *holder);

int hashingfunction(void *key, int index) {
    return doublehashing(key, index, MAX_CHILDREN);
}

void exec_create(char *path);
void exec_create_d(char *path);
void exec_read(char *path);
void exec_write(char *path, char *content);
void exec_delete(char *path);
void exec_delete_r(char *path);
void exec_find(char *filename);
void exec_exit();

itree_t *filestructure;

int main(int argc, char *argv[]) {
    ITREE_INIT_OPEN(filestructure, D, DIR_SEPARATOR_STR, MAX_CHILDREN, hashingfunction);

    bool shouldexit = false;
    int i = 0;
    do {
        char *input = readline();
        command_t currentcmd = buildcommand(input);

        LOG_D("Processing command number %d: [name: %s, arguments: %s]",
              i,
              currentcmd.name,
              currentcmd.argc == 1 ? currentcmd.argv[0] :
              currentcmd.argc == 2 ? strconcat(currentcmd.argv[0], strconcat(", ", currentcmd.argv[1])) : "(none)"
        );

        switch (currentcmd.id) {
            case CREATE:
                exec_create(currentcmd.argv[0]);
                break;
            case CREATE_DIR:
                exec_create_d(currentcmd.argv[0]);
                break;
            case READ:
                exec_read(currentcmd.argv[0]);
                break;
            case WRITE:
                exec_write(currentcmd.argv[0], currentcmd.argv[1]);
                break;
            case DELETE:
                exec_delete(currentcmd.argv[0]);
                break;
            case DELETE_R:
                exec_delete_r(currentcmd.argv[0]);
                break;
            case FIND:
                exec_find(currentcmd.argv[0]);
                break;
            case EXIT:
                exec_exit();
                shouldexit = true;
                break;
            case UNKNOWN:
                LOG_E("Unknown command named: %s", currentcmd.name);
                exit(-1);
        }

        if (currentcmd.id != EXIT) {
            free(currentcmd.argv);
        }

        i++;
    } while (!shouldexit);

    return 0;
}

void exec_create(char *path) {
    list_t *components = pathcomponents(path, DIR_SEPARATOR);
    char *filename = listdeletenode(components, components->tail)->value;

    LOG_D("Trying to create file at path: %s", path);

    inode_t *chosen = movetopath(components->head, filestructure->root, DIR_SEPARATOR_STR);

    if (chosen == NULL) {
        LOG_E("Cannot create file at path: %s; One ore more path components do not exist.", path);
        PRINT_UNSUCCESS;
        return;
    }

    if (hashtablecontains(chosen->children, filename, STRCMP_COMP)) {
        LOG_E("Cannot create file at path: %s; The file already exists.", path);
        PRINT_UNSUCCESS;
        RETURN_FREE(components, NULL);
    }

    inode_t *newfile;
    INODE_ALLOC_FILE(newfile, filename, chosen);

    if (hashtableput(chosen->children, newfile->filename, newfile, STRCMP_COMP) == NULL) {
        LOG_E("Cannot create file at path: %s; Not enough space in the container.", path);
        RETURN_FREE(components, newfile);
    }

    LOG_I("Successfully created file at path: %s", path);

    PRINT_SUCCESS;
    RETURN_FREE(components, NULL);
}

void exec_create_d(char *path) {
    list_t *components = pathcomponents(path, DIR_SEPARATOR);
    char *dirname = listdeletenode(components, components->tail)->value;

    LOG_D("Trying to create directory at path: %s", path);

    inode_t *chosen = movetopath(components->head, filestructure->root, DIR_SEPARATOR_STR);

    if (chosen == NULL) {
        LOG_E("Cannot create directory at path: %s; One ore more path components do not exist.", path);
        PRINT_UNSUCCESS;
        RETURN_FREE(components, NULL);
    }

    if (hashtablecontains(chosen->children, dirname, STRCMP_COMP)) {
        LOG_E("Cannot create directory at path: %s; The directory already exists.", path);
        PRINT_UNSUCCESS;
        RETURN_FREE(components, NULL);
    }

    inode_t *newdir;
    INODE_ALLOC_DIR(newdir, dirname, chosen);

    if (hashtableput(chosen->children, newdir->filename, newdir, STRCMP_COMP) == NULL) {
        LOG_E("Cannot create directory at path: %s; Not enough space in the container.", path);
        PRINT_UNSUCCESS;
        RETURN_FREE(components, newdir);
    }

    LOG_I("Successfully created file at path: %s", path);

    PRINT_SUCCESS;
    RETURN_FREE(components, NULL);
}

void exec_read(char *path) {
    list_t *components = pathcomponents(path, DIR_SEPARATOR);

    LOG_D("Trying to read file at path: %s", path);

    inode_t *chosen = movetopath(components->head, filestructure->root, DIR_SEPARATOR_STR);

    if (chosen == NULL) {
        LOG_E("Cannot read file at path: %s; The file does not exists.", path);
        PRINT_UNSUCCESS;
        RETURN_FREE(components, NULL);
    }

    LOG_I("Successfully read file at path: %s", path);

    PRINT_FILE_CONTENT(chosen->f_content);
    RETURN_FREE(components, NULL);
}

void exec_write(char *path, char *content) {
    list_t *components = pathcomponents(path, DIR_SEPARATOR);

    LOG_D("Trying to write \"%s\" at path: %s", content, path);

    inode_t *chosen = movetopath(components->head, filestructure->root, DIR_SEPARATOR_STR);

    if (chosen == NULL || chosen->f_type == D) {
        LOG_E("Cannot write file at path: %s; The file does not exists.", path);
        PRINT_UNSUCCESS;
        RETURN_FREE(components, NULL);
    }

    chosen->f_content = strdup(content);

    LOG_I("Successfully wrote \"%s\" at path: %s", content, path);

    PRINT_WRITTEN_CHARS(strlen(content));
    RETURN_FREE(components, NULL);
}

void exec_delete(char *path) {
    list_t *components = pathcomponents(path, DIR_SEPARATOR);

    LOG_D("Trying to delete file/directory at path: %s", path);

    inode_t *chosen = movetopath(components->head, filestructure->root, DIR_SEPARATOR_STR);

    if (chosen == NULL) {
        LOG_E("Cannot delete file/directory at path: %s; The file/directory does not exists.", path);
        PRINT_UNSUCCESS;
        return;
    }

    if (chosen->f_type == D && chosen->children->occupied > 0) {
        LOG_E("Cannot delete file/directory at path: %s; The directory has children.", path);
        PRINT_UNSUCCESS;
        RETURN_FREE(components, NULL);
    }

    inode_t *deleted = hashtableremove(chosen->parent->children, chosen->filename, STRCMP_COMP);

    if (deleted != NULL) {
        LOG_I("Successfully deleted file at path: %s", path);
        PRINT_SUCCESS;
    }
    else {
        LOG_E("Could not delete file at path: %s", path);
        PRINT_UNSUCCESS;
    }

    RETURN_FREE(components, deleted);
}

void exec_delete_r(char *path) {
    list_t *components = pathcomponents(path, DIR_SEPARATOR);

    LOG_D("Trying to delete file/directory at path: %s with all it's children (if any).", path);

    inode_t *chosen = movetopath(components->head, filestructure->root, DIR_SEPARATOR_STR);

    if (chosen == NULL) {
        LOG_E("Cannot delete file/directory at path: %s; The file/directory does not exists.", path);
        PRINT_UNSUCCESS;
        return;
    }

    int deleted = delete_r(chosen);

    LOG_I("Successfully deleted file at path: %s with it's %d children", path, deleted);

    PRINT_SUCCESS;
    RETURN_FREE(components, NULL);
}

void exec_find(char *filename) {
    LOG_D("Trying to find file/directory named: %s", filename);

    inode_t *root = filestructure->root;

    list_t *resources;
    LIST_ALLOC(resources);

    int found = find_r(filename, root, resources);

    if (found == 0) {
        LOG_E("No file/directory named: %s where found.", filename);
        PRINT_UNSUCCESS;
        free(resources);
        return;
    }

    LOG_I("Found %d files/directories named: %s where found.", found, filename);

    char **paths = NEW_ARRAY(char *, resources->count);

    listnode_t *iter;
    int index = 0;
    LIST_FOREACH(resources, iter) {
        inode_t *resource = (inode_t *)iter->value;

        char *path = "";

        while (resource != filestructure->root) {
            path = strconcat(strconcat(DIR_SEPARATOR_STR, resource->filename), path);
            resource = resource->parent;
        }

        paths[index] = path;
        index++;
    }

    qsort(paths, (size_t) resources->count, sizeof(char *), _strcmp_qsort);

    int i;
    for (i = 0; i < resources->count; ++i) {
        PRINT_FIND(paths[i]);
    }

    free(resources);
    free(paths);
}

void exec_exit() {
    LOG_I("Exiting...");
}

void freeValue(void *value) {
    inode_t *node = value;
    delete_r(node);
}

int delete_r(inode_t *node) {
    if (node == NULL) {
        return 0;
    }

    if (node->children->occupied == 0) {
        inode_t *victim = hashtableremove(node->parent->children, node->filename, STRCMP_COMP);

        free(victim->children);
        free(victim->filename);
        free(victim->f_content);
        free(victim);
        return 1;
    }

    inode_t *child;
    int deleted = 0;
    HASHTABLE_FOREACH_VALUE(node->children, child, {
        if (child->f_type == F) {
            inode_t *victim = hashtableremove(node->children, child->filename, STRCMP_COMP);
            free(victim->filename);
            free(victim->f_content);
            free(victim);

            deleted++;
        }
        else {
            deleted += delete_r(child);
        }
    });

    inode_t *victim = hashtableremove(node->parent->children, node->filename, STRCMP_COMP);
    free(victim->children);
    free(victim->filename);
    free(victim->f_content);
    free(victim);

    return deleted + 1;
}

int find_r(char *filename, inode_t *node, list_t *holder) {
    int found = 0;

    if (strcmp(node->filename, filename) == 0) {
        listadd(holder, node);
        found++;
    }

    if (node->f_type == F) {
        return 0;
    }

    if (node->children->occupied == 0) {
        return 0;
    }

    keyvaluepair_t *pair;
    HASHTABLE_FOREACH(node->children, pair, {
        inode_t *child = (inode_t *)pair->value;

        if (strcmp(child->filename, filename) == 0) {
            listadd(holder, child);
            found++;
        }
        else if (child->f_type == D) {
            found += find_r(filename, child, holder);
        }
    });

    return found;
}