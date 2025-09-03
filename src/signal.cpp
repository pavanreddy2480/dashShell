#include <csignal>
#include <iostream>
#include <unistd.h>
using namespace std;

volatile sig_atomic_t fgPid=-1;

void signalHandler(int signum){
    if(signum==SIGINT){
        if(fgPid>0){
            kill(fgPid,SIGINT);
            cout<<"\n[Shell] Foreground process "<<fgPid<<" interrupted (SIGINT)"<<endl;
        }
    }
    else if(signum==SIGTSTP){
        if(fgPid>0){
            kill(fgPid,SIGTSTP);
            cout<<"\n[Shell] Foreground process "<<fgPid<<" stopped (SIGTSTP)"<<endl;
        }
    }
    else if(signum==SIGCHLD){
        int status;
        int pid;
        while((pid=waitpid(-1,&status,WNOHANG))>0){
            cout<<"[Shell] Child process "<<pid<<" terminated"<<endl;
        }
    }
}