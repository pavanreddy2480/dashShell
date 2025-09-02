#ifndef SYSCMD_HPP
#define SYSCMD_HPP

#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

class SYSCMD {
public:
    static void executeCommand(std::vector<std::string> &args);
};

#endif // SYSCMD_HPP