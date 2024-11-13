//
// Created by Marco Speziali on 02/07/2017.
//

#ifndef TINYFS_LOGGING_H
#define TINYFS_LOGGING_H

#include <stdio.h>
#include <libgen.h>
#include "logginglevels.h"

char *__getFormattedTime();

#define FORMATTED_TIME __getFormattedTime()

#define __LOG__(format, loglevel, ...) printf("%s %-5s [%s] [%s:%d] " format "\n", FORMATTED_TIME, loglevel, __func__, basename(__FILE__), __LINE__, ## __VA_ARGS__)

#ifdef LOGGING_LEVEL
    #if LOGGING_LEVEL == DEBUG_LEVEL

        #define LOG_D(format, ...) __LOG__(format, DEBUG, ## __VA_ARGS__)
        #define LOG_I(format, ...) __LOG__(format, INFO, ## __VA_ARGS__)
        #define LOG_W(format, ...) __LOG__(format, WARNING, ## __VA_ARGS__)
        #define LOG_E(format, ...) __LOG__(format, ERROR, ## __VA_ARGS__)

    #elif LOGGING_LEVEL == INFO_LEVEL

        #define LOG_D(format, ...)
        #define LOG_I(format, ...) __LOG__(format, INFO, ## __VA_ARGS__)
        #define LOG_W(format, ...) __LOG__(format, WARNING, ## __VA_ARGS__)
        #define LOG_E(format, ...) __LOG__(format, ERROR, ## __VA_ARGS__)

    #elif LOGGING_LEVEL == WARNING_LEVEL

        #define LOG_D(format, ...)
        #define LOG_I(format, ...)
        #define LOG_W(format, ...) __LOG__(format, WARNING, ## __VA_ARGS__)
        #define LOG_E(format, ...) __LOG__(format, ERROR, ## __VA_ARGS__)

    #elif LOGGING_LEVEL == ERROR_LEVEL

        #define LOG_D(format, ...)
        #define LOG_I(format, ...)
        #define LOG_W(format, ...)
        #define LOG_E(format, ...) __LOG__(format, ERROR, ## __VA_ARGS__)

    #elif LOGGING_LEVEL == NO_LOG

        #define LOG_D(format, ...)
        #define LOG_I(format, ...)
        #define LOG_W(format, ...)
        #define LOG_E(format, ...)

    #endif
#else

    #define LOG_D(format, ...)
    #define LOG_I(format, ...)
    #define LOG_W(format, ...)
    #define LOG_E(format, ...)

#endif




#endif //TINYFS_LOGGING_H
