#include "pinfo.h"
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <libproc.h>
#include <limits.h>
#include <mach/mach.h>

using namespace std;

long getMemory(int pid){
    task_t task;
    if(task_for_pid(mach_task_self(),pid,&task)!=KERN_SUCCESS){
        return -1;
    }
    task_basic_info info;
    mach_msg_type_number_t infoCount=TASK_BASIC_INFO_COUNT;
    if(task_info(task,TASK_BASIC_INFO,(task_info_t)&info,&infoCount)!=KERN_SUCCESS){
        return -1;
    }
    return info.resident_size;
}

void PINFO::handlePinfo(const vector<string>& s){
    int pid=(s.size()==1)?getpid():stoi(s[1]);
    struct proc_bsdinfo proc;
    if(proc_pidinfo(pid,PROC_PIDTBSDINFO,0,&proc,sizeof(proc))<=0){
        cerr<<"pinfo: process with PID "<<pid<<" not found\n";
        return;
    }

    cout<<"pid -- "<<pid<<"\n";

    string status;
    switch(proc.pbi_status){
        case 1: status="Running"; break;
        case 2: status="Sleeping"; break;
        case 3: status="Stopped"; break;
        case 4: status="Zombie"; break;
        default: status="Unknown"; break;
    }
    int fg_pgid=tcgetpgrp(STDIN_FILENO);
    if(getpgid(pid)==fg_pgid){
        status+="+";
    }
    cout<<"Process Status -- "<<status<<"\n";

    long mem=getMemory(pid);
    if(mem!=-1){
        cout<<"memory -- "<<mem<<" bytes (Resident Memory)\n";
    }
    else{
        cout<<"memory -- [Unavailable]\n";
    }

    char pathbuf[1024];
    if(proc_pidpath(pid,pathbuf,sizeof(pathbuf))>0){
        string path=pathbuf;
        const char* home=getenv("HOME");
        if(home&&path.find(home)==0){
            path.replace(0,strlen(home),"~");
        }
        cout<<"Executable Path -- "<<path<<"\n";
    }
    else{
        cout<<"Executable Path -- [Unavailable]\n";
    }
}