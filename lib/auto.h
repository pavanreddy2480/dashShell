#ifndef AUTO_H
#define AUTO_H
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>

// Setup autocomplete (call once in main)
void setup_autocomplete();

// Completion callback for readline
char** my_completion(const char* text, int start, int end);

#endif