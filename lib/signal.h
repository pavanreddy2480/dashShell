#ifndef SIGNAL_H
#define SIGNAL_H

#include <sys/types.h>

extern volatile int killpid;

void signalHandler(int signum);

#endif