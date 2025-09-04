#ifndef AUTO_H
#define AUTO_H
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>

void setupAutocomplete();

char** my_completion(const char* text, int start, int end);

#endif