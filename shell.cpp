#include "fcntl.h"
#include "sys/wait.h"
#include "vector"
#include <cstdlib>
#include <iostream>
#include <string>
#include <type_traits>
#include <unistd.h>
#include <vector>
#include <pwd.h>        // for getpwuid()
#include <limits.h>     // for PATH_MAX
#include <editline/readline.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"
#include "user.cpp"
#include "signal.cpp"
#include "commands.cpp"
#include "io.cpp"
using namespace std;

const char *commands[] = {
    "help",
    "exit",
    "list",
    "show",
    "run",
    "history",
    NULL
};

char *command_generator(const char *text, int state) {
    static int list_index, len;
    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    const char *name;
    while ((name = commands[list_index++])) {
        if (strncmp(name, text, len) == 0) {
            return strdup(name);  // return match
        }
    }
    return NULL;
}

char **custom_completion(const char *text, int start, int end) {
    (void)start; // unused
    (void)end;   // unused
    return rl_completion_matches(text, command_generator);
}

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);
    signal(SIGCHLD, signalHandler);

    rl_attempted_completion_function = custom_completion;

    cout<< R"(
     _           _     
  __| | __ _ ___| |__  
 / _` |/ _` / __| '_ \ 
| (_| | (_| \__ \ | | |
 \__,_|\__,_|___/_| |_|
                                      
    )"<<endl;

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    string shell_home = cwd; 

    while (true) {
        string prompt = buildPrompt(shell_home);
        char *input = readline(prompt.c_str());

        if (!input) {
            printf("\nExit\n");
            break;  // Ctrl+D
        }

        if (*input) {
            add_history(input);  // allows Up/Down arrows
        }

        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        string line(input);
        free(input);

        auto parsed = Parser::parse(line);
        for(auto i:parsed){
            // customCommands::cmd(i.argv,cwd,shell_home);
            IO::execute(i);
        }
        // TODO: execute parsed commands here
    }

    return 0;
}