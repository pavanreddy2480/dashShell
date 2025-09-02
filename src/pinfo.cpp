#include "pinfo.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <libproc.h>
#include <limits.h>
#include <mach/mach.h>
#include <mach/mach_init.h>
#include <mach/task.h>
#include <mach/task_info.h>
#include <mach/mach_types.h>

using namespace std;

long getMemory(pid_t pid) {
    task_t task;
    if (task_for_pid(mach_task_self(), pid, &task) != KERN_SUCCESS) {
        return -1; // cannot access
    }

    task_basic_info info;
    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
    if (task_info(task, TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS) {
        return -1;
    }

    return info.resident_size; // bytes
}

void PINFO::handlePinfo(const std::vector<std::string> &s) {
    pid_t pid = (s.size() == 1) ? getpid() : stoi(s[1]);

    struct proc_bsdinfo proc;
    if (proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &proc, sizeof(proc)) <= 0) {
        std::cerr << "pinfo: process with PID " << pid << " not found\n";
        return;
    }

    std::cout << "pid -- " << pid << "\n";

    // Process status
    char state = proc.pbi_status;
    std::string status;
    switch(state){
        case SIDL: status="Idle"; break;
        case SRUN: status="Running"; break;
        case SSLEEP: status="Sleeping"; break;
        case SSTOP: status="Stopped"; break;
        case SZOMB: status="Zombie"; break;
        default: status="Unknown"; break;
    }
    pid_t fg_pgid = tcgetpgrp(STDIN_FILENO);
    if(getpgid(pid) == fg_pgid) status+="+";
    std::cout << "Process Status -- " << status << "\n";

    // Memory
    long mem = getMemory(pid);
    if(mem != -1)
        std::cout << "memory -- " << mem << " bytes (Resident Memory)\n";
    else
        std::cout << "memory -- [Unavailable]\n";

    // Executable path
    char pathbuf[PATH_MAX];
    if(proc_pidpath(pid, pathbuf, sizeof(pathbuf)) > 0){
        std::string path = pathbuf;
        const char* home = getenv("HOME");
        if(home && path.find(home) == 0){
            path.replace(0, strlen(home), "~");
        }
        std::cout << "Executable Path -- " << path << "\n";
    } else {
        std::cout << "Executable Path -- [Unavailable]\n";
    }
}