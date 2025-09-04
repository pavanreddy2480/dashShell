#include "fg.h"
#include "signal.h"
using namespace std;


void SYSCMD::executeCommand(vector<string>& args) {
    if(args.empty()) return;
    bool background=false;
    if(args.back()=="&"){
        background=true;
        args.pop_back();
    }
    vector<char*>argv;
    for(auto& arg:args) argv.push_back((char*)arg.c_str());
    argv.push_back(nullptr);

    int pid=fork();
    if(pid<0){
        perror("fork failed");
        return;
    }
    if(pid==0){
        if(execvp(argv[0],argv.data())<0){
            perror("execvp failed");
            exit(1);
        }
    }
    else{
        if(background){
            cout<<"Started background process with PID: "<<pid<<endl;
        }
        else{
            killpid=pid;
            int status;
            waitpid(pid, &status, WUNTRACED);
            killpid=-1;
        }
    }
}