#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include "signal.h"

extern "C" int kill(int,int);
using namespace std;

void signalHandler(int signum){
    if(signum==SIGINT){
        if(killpid>0){
            kill(killpid,SIGINT);
            cout<<"\n[Shell] Foreground process "<<killpid<<" interrupted"<<endl;
        }else{
            cout<<"\n";
            rl_on_new_line();
            rl_replace_line("",0);
            rl_redisplay();
        }
    }else if(signum==SIGTSTP){
        if(killpid>0){
            kill(killpid,SIGTSTP);
            cout<<"\n[Shell] Foreground process "<<killpid<<" stopped"<<endl;
        }
    }
}