#include "fg.h"

using namespace std;

void SYSCMD::executeCommand(vector<string> &args) {
    if (args.empty()) return;

    bool background = false;

    // Check if last arg is "&" (background process)
    if (args.back() == "&") {
        background = true;
        args.pop_back();
    }

    // Convert vector<string> → char* array for execvp
    vector<char*> argv;
    for (auto &arg : args) argv.push_back((char*)arg.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        // --- Child process ---
        if (execvp(argv[0], argv.data()) < 0) {
            perror("execvp failed");
            exit(1);
        }
    } else {
        // --- Parent process ---
        if (background) {
            cout << "Started background process with PID: " << pid << endl;
        } else {
            int status;
            waitpid(pid, &status, 0); // wait for foreground process
        }
    }
}