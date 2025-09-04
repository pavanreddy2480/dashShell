#ifndef CUSTOMCOMMANDS_H
#define CUSTOMCOMMANDS_H

#include <string>
#include <vector>

class customCommands {
public:
    static std::string prevDir; 
    static void cmd(std::vector<std::string> &s, char buff[], std::string home_dir);
    static bool isBuiltin(const std::string& cmd);
};

#endif