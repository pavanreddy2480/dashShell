#include "pipes.h"
#include "commands.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
using namespace std;

void Pipes::run(const vector<Command>& cmds, char buff[], string home_dir) {
    int n = cmds.size();
    int pipefds[2 * (n - 1)];

    for (int i = 0; i < n - 1; i++) {
        if (pipe(pipefds + i*2) < 0) {
            perror("pipe");
            return;
        }
    }

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // setup input from previous pipe
            if (i != 0) {
                dup2(pipefds[(i-1)*2], STDIN_FILENO);
            }

            // setup output to next pipe
            if (i != n-1) {
                dup2(pipefds[i*2+1], STDOUT_FILENO);
            }

            // close all pipe fds
            for (int j = 0; j < 2*(n-1); j++) close(pipefds[j]);

            // handle input redirection
            if (!cmds[i].inputFile.empty()) {
                int fd = open(cmds[i].inputFile.c_str(), O_RDONLY);
                if (fd < 0) { perror("open input"); exit(1); }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            // handle output redirection
            if (!cmds[i].outputFile.empty()) {
                int flags = O_WRONLY | O_CREAT | (cmds[i].append ? O_APPEND : O_TRUNC);
                int fd = open(cmds[i].outputFile.c_str(), flags, 0644);
                if (fd < 0) { perror("open output"); exit(1); }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            // built-in or external command
            if (!cmds[i].argv.empty()) {
                if (cmds[i].argv[0] == "cd" || cmds[i].argv[0] == "pwd" || cmds[i].argv[0] == "echo" ||
                    cmds[i].argv[0] == "ls" || cmds[i].argv[0] == "pinfo" || cmds[i].argv[0] == "search") {
                    // run built-ins directly
                    customCommands::cmd(cmds[i].argv, buff, home_dir);
                    exit(0);
                } else {
                    // external command
                    vector<char*> argv;
                    for (auto &arg : cmds[i].argv) argv.push_back(const_cast<char*>(arg.c_str()));
                    argv.push_back(nullptr);
                    execvp(argv[0], argv.data());
                    perror("execvp");
                    exit(1);
                }
            }
            exit(0);
        }
        else if (pid < 0) {
            perror("fork");
            return;
        }
    }

    // parent closes all pipes
    for (int j = 0; j < 2*(n-1); j++) close(pipefds[j]);

    // wait for children
    for (int i = 0; i < n; i++) wait(NULL);
}