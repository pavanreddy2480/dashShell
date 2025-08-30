#include "fcntl.h"
#include "sys/wait.h"
#include "vector"
#include <iostream>
using namespace std;

void signalHandler(int signum) {
    if (signum == SIGINT) {
        cout << "\nINTERRUPT SIGNAL _STDINT_ RECEIVED" << endl;
    }
    if (signum == SIGTSTP) {
        exit(signum);
    }
    if (signum == SIGCHLD) {
        cout << "\nCHILD GOT TERMINATED" << endl;
    }
}