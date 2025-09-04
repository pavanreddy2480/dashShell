#include <iostream>
#include <unistd.h>     // must come first for kill()
#include <signal.h>     // SIGINT, SIGTSTP, SIGCHLD
#include <sys/types.h>  // int
#include <sys/wait.h>   // waitpid, WNOHANG
#include <iostream>
#include "signal.h"
#include <readline/readline.h>

extern "C" int kill(int, int);

using namespace std;

void signalHandler(int signum) {
    if (signum == SIGINT) {
        if (killpid > 0) {
            kill(killpid, SIGINT);
            cout << "\n[Shell] Foreground process " << killpid << " interrupted" << endl;
        }
        else {
            // No child: just reset the line
            cout << "\n"; 
            rl_on_new_line();   // from readline
            rl_replace_line("", 0);
            rl_redisplay();
        }
    }
    else if (signum == SIGTSTP) {
        if (killpid > 0) {
            kill(killpid, SIGTSTP);
            cout << "\n[Shell] Foreground process " << killpid << " stopped" << endl;
        }
    }
    // else if (signum == SIGCHLD) {
    //     int status;
    //     int pid;
    //     while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    //         cout << "[Shell] Child process " << pid << " terminated" << endl;
    //     }
    // }
}