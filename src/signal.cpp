#include "signal.h"
#include <iostream>
#include <csignal>   // for SIGINT, SIGTSTP, SIGCHLD
#include <cstdlib>   // for exit()

using namespace std;

void signalHandler(int signum) {
    if (signum == SIGINT) {
        cout << "\nINTERRUPT SIGNAL (SIGINT) RECEIVED" << endl;
    }
    else if (signum == SIGTSTP) {
        cout << "\nSTOP SIGNAL (SIGTSTP) RECEIVED - exiting..." << endl;
        exit(signum);
    }
    else if (signum == SIGCHLD) {
        cout << "\nCHILD PROCESS TERMINATED (SIGCHLD)" << endl;
    }
}