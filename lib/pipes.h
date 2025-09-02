#pragma once
#include "parser.h"

class Pipes {
public:
    static void run(const std::vector<Command>& cmds, char buff[], std::string home_dir);
};