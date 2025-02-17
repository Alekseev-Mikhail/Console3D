//
// Created by mikhail on 2/16/25.
//

#ifndef DEBUG_H
#define DEBUG_H

void debug_Init();

void debug_Dispose();

void Log(const char *format, ...);

void LogAbort(const char *format, ...);

#endif //DEBUG_H
