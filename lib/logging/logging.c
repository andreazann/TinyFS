//
// Created by Marco Speziali on 02/07/2017.
//
#include "logging.h"
#include <time.h>

// Returns the local date/time formatted as 2014-03-19 11:11:52
char* __getFormattedTime(void) {
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Must be static, otherwise won't work
    static char _retval[20];
    strftime(_retval, sizeof(_retval), "%Y-%m-%d %H:%M:%S", timeinfo);

    return _retval;
}
