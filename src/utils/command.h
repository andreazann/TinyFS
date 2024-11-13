//
// Created by Marco Speziali on 01/07/2017.
//

#ifndef TINYFS_COMMAND_H
#define TINYFS_COMMAND_H

#include <string.h>
#include <stdlib.h>
#include "../../lib/macros.h"

typedef enum commandid_e {
    CREATE = 0 << 1,
    CREATE_DIR = 1 << 1,
    READ = 2 << 1,
    WRITE = 3 << 1,
    DELETE = 4 << 1,
    DELETE_R = 5 << 1,
    FIND = 6 << 1,
    EXIT = 7 << 1,
    UNKNOWN = -1 << 1
} commandid_t;

#define PARSE_COMMAND_ID(command_name_str) \
    (strcmp((command_name_str), "create")        == 0) ? CREATE      :       \
    (strcmp((command_name_str), "create_dir")    == 0) ? CREATE_DIR  :       \
    (strcmp((command_name_str), "read")          == 0) ? READ        :       \
    (strcmp((command_name_str), "write")         == 0) ? WRITE       :       \
    (strcmp((command_name_str), "delete")        == 0) ? DELETE      :       \
    (strcmp((command_name_str), "delete_r")      == 0) ? DELETE_R    :       \
    (strcmp((command_name_str), "find")          == 0) ? FIND        :       \
    (strcmp((command_name_str), "exit")          == 0) ? EXIT        :       \
    UNKNOWN

typedef struct command_s {
    commandid_t id;
    char *name;
    int argc;
    char **argv;
} command_t;

command_t buildcommand(char *commandstr) {
    command_t command;

    command.name = strtok(commandstr, " ");
    command.id = PARSE_COMMAND_ID(command.name);

    if (command.id == UNKNOWN) {
        command.argc = -1;
        command.argv = NULL;
        return command;
    }

    if (command.id == EXIT) {
        command.argc = 0;
        return command;
    }

    if (command.id == WRITE) {
        command.argc = 2;
        command.argv = NEW_ARRAY(char *, command.argc);
        command.argv[0] = strtok(NULL, " ");
        command.argv[1] = strtok(NULL, "\"");
    }
    else {
        command.argc = 1;
        command.argv = NEW_ARRAY(char *, command.argc);
        command.argv[0] = strtok(NULL, " ");
    }

    return command;
}

#endif //TINYFS_COMMAND_H
