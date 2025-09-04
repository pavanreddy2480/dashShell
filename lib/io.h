#ifndef IO_REDIRECTION_H
#define IO_REDIRECTION_H

#include <string>
#include <vector>
#include "parser.h"

class IO {
public:
    static void execute(const Command &cmd);
};

#endif