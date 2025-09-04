#include<fcntl.h>
#include<sys/wait.h>
#include<cstdlib>
#include<iostream>
#include<string>
#include<unistd.h>
#include<pwd.h>
#include<limits.h>
#include<csignal>
#include<readline/readline.h>
#include<readline/history.h>

#include"parser.h"
#include"user.h"
#include"signal.h"
#include"commands.h"
#include"io.h"
#include"pipes.h"
#include"auto.h"
// #include"history.h"
volatile pid_t killpid = -1;

using namespace std;


// History history;

string history_file;

void operate(string&shell_home,char*cwd){
    string prompt=buildPrompt(shell_home);
    char*input=readline(prompt.c_str());
    if(!input)exit(0);
    if(*input){add_history(input);stifle_history(20);write_history(history_file.c_str());}
    string line(input);
    free(input);
    if(line=="exit"){
        exit(0);
    }

    if(line.rfind("history",0)==0){
        int num=20;string cmd;
        istringstream iss(line);
        iss>>cmd;
        if(iss>>num){}
        HIST_ENTRY**hist_list=history_list();
        if(hist_list){
            int start=max(0,history_length-num);
            for(int i=start;i<history_length;i++){
                cout<<i+1<<" "<<hist_list[i]->line<<endl;
            }
        }
        return;
    }

    istringstream ss(line);
    string command;
    while(getline(ss,command,';')){
        if(command.empty())continue;
        auto parsed=Parser::parse(command);
        if(parsed.empty())continue;
        if(parsed.size()>1){
            Pipes::run(parsed,cwd,shell_home);
        }else{
            if(parsed[0].inputFile.empty()&&parsed[0].outputFile.empty())
                customCommands::cmd(parsed[0].argv,cwd,shell_home);
            IO::execute(parsed[0]);
        }
    }
    
}
int main(){
    signal(SIGINT,signalHandler);
    signal(SIGTSTP,signalHandler);
    signal(SIGCHLD,signalHandler);
    // signal(SIGTSTP, SIG_IGN);

    setup_autocomplete();

    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    string shell_home=cwd;

    history_file=shell_home+"/.myshell_history";

    cout<<R"(
     _           _     
  __| | __ _ ___| |__  
 / _` |/ _` / __| '_ \ 
| (_| | (_| \__ \ | | |
 \__,_|\__,_|___/_| |_|

    )"<<endl;

    read_history(history_file.c_str());
    stifle_history(20);
    
    while(true)operate(shell_home,cwd);
    return 0;
}