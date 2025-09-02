#ifndef IO_REDIRECTION_H
#define IO_REDIRECTION_H

#include <string>
#include <vector>
#include "parser.h"   // Command structure

class IO {
public:
    // Executes a command with optional I/O redirection
    static void execute(const Command &cmd);
};

#endif // IO_REDIRECTION_H