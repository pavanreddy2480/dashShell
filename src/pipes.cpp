#include "pipes.h"
#include "commands.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
using namespace std;

void Pipes::run(const vector<Command>&cmds,char buff[],string home_dir){
    int n=cmds.size();
    if(n==0)return;
    int fd[n-1][2];
    for(int i=0;i<n-1;i++){
        if(pipe(fd[i])<0){perror("pipe");return;}
    }
    for(int i=0;i<n;i++){
        pid_t pid=fork();
        if(pid==0){
            if(i!=0)dup2(fd[i-1][0],STDIN_FILENO);
            if(i!=n-1)dup2(fd[i][1],STDOUT_FILENO);
            for(int j=0;j<n-1;j++){close(fd[j][0]);close(fd[j][1]);}
            if(!cmds[i].inputFile.empty()){
                int in_fd=open(cmds[i].inputFile.c_str(),O_RDONLY);
                if(in_fd<0){perror("open input");exit(1);}
                dup2(in_fd,STDIN_FILENO);close(in_fd);
            }
            if(!cmds[i].outputFile.empty()){
                int flags=O_WRONLY|O_CREAT|(cmds[i].append?O_APPEND:O_TRUNC);
                int out_fd=open(cmds[i].outputFile.c_str(),flags,0644);
                if(out_fd<0){perror("open output");exit(1);}
                dup2(out_fd,STDOUT_FILENO);close(out_fd);
            }
            if(!cmds[i].argv.empty()){
                vector<string>args=cmds[i].argv;
                if(args[0]=="cd"||args[0]=="pwd"||args[0]=="echo"||args[0]=="ls"||args[0]=="pinfo"||args[0]=="search"){
                    customCommands::cmd(args,buff,home_dir);exit(0);
                }else{
                    vector<char*>argv;
                    for(auto&arg:args)argv.push_back(const_cast<char*>(arg.c_str()));
                    argv.push_back(nullptr);
                    execvp(argv[0],argv.data());
                    perror("execvp failed");exit(1);
                }
            }
            exit(0);
        }else if(pid<0){perror("fork");return;}
    }
    for(int j=0;j<n-1;j++){close(fd[j][0]);close(fd[j][1]);}
    for(int i=0;i<n;i++)wait(NULL);
}