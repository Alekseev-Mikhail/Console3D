//
// Created by mikhail on 2/16/25.
//

#include "debug.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char FILE_NAME[] = "debug.txt";
static FILE *file;

void debug_Init() {
    file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        fprintf(stderr, "ERROR: unable to open file %s\n", FILE_NAME);
        abort();
    }
}

void debug_Dispose() {
    fclose(file);
}

void Log(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);
}

void LogAbort(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);
    debug_Dispose();
    abort();
}
