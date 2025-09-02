#include "io.h"
#include <iostream>
#include <unistd.h>     // fork, execvp, dup2
#include <fcntl.h>      // open
#include <sys/wait.h>   // waitpid

using namespace std;

void IO::execute(const Command &cmd) {
    if (cmd.argv.empty()) return;
    if(!(cmd.inputFile.size() || cmd.outputFile.size())) return;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        // ---- Input redirection ----
        if (!cmd.inputFile.empty()) {
            int fd = open(cmd.inputFile.c_str(), O_RDONLY);
            if (fd < 0) {
                perror(("cannot open input file: " + cmd.inputFile).c_str());
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // ---- Output redirection ----
        if (!cmd.outputFile.empty()) {
            int fd;
            if (cmd.append) {
                fd = open(cmd.outputFile.c_str(),
                          O_WRONLY | O_CREAT | O_APPEND, 0644);
            } else {
                fd = open(cmd.outputFile.c_str(),
                          O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            if (fd < 0) {
                perror(("cannot open output file: " + cmd.outputFile).c_str());
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        // ---- Exec system command ----
        vector<char*> args;
        for (const auto &a : cmd.argv)
            args.push_back(const_cast<char*>(a.c_str()));
        args.push_back(nullptr);

        if (execvp(args[0], args.data()) < 0) {
            perror(("exec failed: " + cmd.argv[0]).c_str());
            exit(1);
        }
    } else {
        // ---- Wait for child ----
        int status;
        waitpid(pid, &status, 0);
    }
}