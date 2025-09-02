#include "fcntl.h"
#include "sys/wait.h"
#include "vector"
#include <cstdlib>
#include <iostream>
#include <string>
#include <type_traits>
#include <unistd.h>
#include <vector>
#include <pwd.h>
#include <limits.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "parser.h"
#include "user.h"
#include "signal.h"
#include "commands.h"
#include "io.h"
#include "pipes.h"
#include "auto.h"
#include "history.h"

using namespace std;

History history; 
string history_file = string(getenv("HOME")) + "/.myshell_history";

void operate(string &shell_home, char* cwd) {
    string prompt = buildPrompt(shell_home);
    char* input = readline(prompt.c_str());

    if (!input) exit(0);  // Ctrl+D
    if (*input) {
        add_history(input);         // add to readline buffer
        history.add(input);         // add to our persistent tracker
    }

    string line(input);
    free(input);

    if (line == "exit") {
        // Save before exiting
        write_history(history_file.c_str());
        exit(0);
    }

    if (line.rfind("history", 0) == 0) {
        int num = 10;
        string cmd;
        istringstream iss(line);
        iss >> cmd;
        if (iss >> num) {
            history.print(num);
        } else {
            history.print();
        }
        return;
    }

    auto parsed = Parser::parse(line);

    if (parsed.size() > 1) {
        Pipes::run(parsed, cwd, shell_home);
    } else if (!parsed.empty()) {
        if (!parsed[0].inputFile.size() && !parsed[0].outputFile.size()) {
            customCommands::cmd(parsed[0].argv, cwd, shell_home);
        }
        IO::execute(parsed[0]);
    }
}

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);
    signal(SIGCHLD, signalHandler);

    setup_autocomplete();

    cout << R"(
     _           _     
  __| | __ _ ___| |__  
 / _` |/ _` / __| '_ \ 
| (_| | (_| \__ \ | | |
 \__,_|\__,_|___/_| |_|
                                      
    )" << endl;

    // load history from previous sessions into readline + ours
    read_history(history_file.c_str());
    stifle_history(20);  // limit to 20
    {
        // also push them into our custom History object
        // After read_history(...)
        int hist_len = history_length;  // number of entries
        for (int i = 1; i <= hist_len; i++) {
            HIST_ENTRY* entry = history_get(i);
            if (entry && entry->line) {
                history.add(entry->line);
            }
        }
    }

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    string shell_home = cwd;

    while (true) {
        operate(shell_home, cwd);
    }

    return 0;
}