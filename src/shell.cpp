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
#include <editline/readline.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <string>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

#include "parser.h"
#include "user.h"
#include "signal.h"
#include "commands.h"
#include "io.h"
#include "pipes.h"

using namespace std;
using namespace std;

void operate(string &shell_home,char* cwd){
    string prompt=buildPrompt(shell_home);
    char* input=readline(prompt.c_str());

    if(!input) exit(0);
    if(*input) add_history(input);

    if(strcmp(input,"exit")==0){free(input);exit(0);}

    string line(input);free(input);

    auto parsed = Parser::parse(line);
    for(auto &i:parsed){
        for(auto j:i.argv)cout<<j<<" ";
        cout<<endl;
    }
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
        operate(shell_home,cwd);
    }

    return 0;
}