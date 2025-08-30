#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

class PINFO {
public:
    static void handlePinfo(const vector<string> &s) {
    pid_t pid = (s.size() == 1) ? getpid() : stoi(s[1]);

    // --- STATUS ---
    string statPath = "/proc/" + to_string(pid) + "/stat";
    cout<<statPath<<endl;
    ifstream statFile(statPath);
    if (!statFile) {
        cerr << "pinfo: process with PID " << pid << " not found\n";
        return;
    }

    string statLine;
    getline(statFile, statLine);
    statFile.close();

    istringstream iss(statLine);
    int pid_read;
    string comm;
    char state;
    iss >> pid_read >> comm >> state;

    int proc_pgid = getpgid(pid);
    int fg_pgid = tcgetpgrp(STDIN_FILENO);
    string status(1, state);
    if (proc_pgid == fg_pgid) status += "+";

    cout << "pid -- " << pid << "\n";
    cout << "Process Status -- " << status << "\n";

    // --- MEMORY ---
    string statmPath = "/proc/" + to_string(pid) + "/statm";
    ifstream memFile(statmPath);
    long mem = 0;
    if (memFile) memFile >> mem;
    cout << "memory -- " << mem << " {Virtual Memory}\n";

    // --- EXECUTABLE PATH ---
    string exePath = "/proc/" + to_string(pid) + "/exe";
    char exe[PATH_MAX];
    ssize_t len = readlink(exePath.c_str(), exe, sizeof(exe)-1);
    if (len != -1) {
        exe[len] = '\0';
        string path = exe;

        const char* home = getenv("HOME");
        if (home && path.find(home) == 0) {
            path.replace(0, strlen(home), "~");
        }
        cout << "Executable Path -- " << path << "\n";
    }
}
};