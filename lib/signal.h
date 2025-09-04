#ifndef SIGNAL_H
#define SIGNAL_H

#include <sys/types.h>

extern volatile int killpid;  // declaration

void signalHandler(int signum);

#endif