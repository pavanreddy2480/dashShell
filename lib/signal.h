#ifndef SIGNAL_H
#define SIGNAL_H

#include <sys/types.h>

extern volatile pid_t killpid;  // declaration

void signalHandler(int signum);

#endif