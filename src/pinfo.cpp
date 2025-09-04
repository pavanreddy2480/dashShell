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

void PINFO::handlePinfo(const vector<string>& args){
    int targetPid=(args.size()==1)?getpid():stoi(args[1]);
    struct proc_bsdinfo procInfo;
    if(proc_pidinfo(targetPid,PROC_PIDTBSDINFO,0,&procInfo,sizeof(procInfo))<=0){
        cerr<<"pinfo: process with PID "<<targetPid<<" not found\n";
        return;
    }

    cout<<"pid -- "<<targetPid<<"\n";

    string procStatus;
    switch(procInfo.pbi_status){
        case 1: procStatus="Running"; break;
        case 2: procStatus="Sleeping"; break;
        case 3: procStatus="Stopped"; break;
        case 4: procStatus="Zombie"; break;
        default: procStatus="Unknown"; break;
    }
    int fgGroup=tcgetpgrp(STDIN_FILENO);
    if(getpgid(targetPid)==fgGroup) procStatus+="+";
    cout<<"Process Status -- "<<procStatus<<"\n";

    task_t task;
    long residentMem=-1;
    if(task_for_pid(mach_task_self(),targetPid,&task)==KERN_SUCCESS){
        task_basic_info taskInfo;
        mach_msg_type_number_t infoCount=TASK_BASIC_INFO_COUNT;
        if(task_info(task,TASK_BASIC_INFO,(task_info_t)&taskInfo,&infoCount)==KERN_SUCCESS){
            residentMem=taskInfo.resident_size;
        }
    }
    if(residentMem!=-1) cout<<"memory -- "<<residentMem<<" bytes (Resident Memory)\n";
    else cout<<"memory -- [Unavailable]\n";

    char exePath[1024];
    if(proc_pidpath(targetPid,exePath,sizeof(exePath))>0){
        string pathStr=exePath;
        const char* homeDir=getenv("HOME");
        if(homeDir && pathStr.find(homeDir)==0) pathStr.replace(0,strlen(homeDir),"~");
        cout<<"Executable Path -- "<<pathStr<<"\n";
    }else{
        cout<<"Executable Path -- [Unavailable]\n";
    }
}