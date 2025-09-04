#include "commands.h"
#include "ls.h"
#include "pinfo.h"
#include "search.h"
#include "fg.h"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

bool customCommands::isBuiltin(const string& cmd) {
    static const vector<string> builtins = {"pwd","exit","cd","echo","ls","pinfo","search"};
    return find(builtins.begin(), builtins.end(), cmd) != builtins.end();
}

string customCommands::prevDir = "";
void customCommands::cmd(vector<string>&s,char buff[],string home_dir){
    if(s.empty())return;
    if(s[0]=="pwd"){if(getcwd(buff,1000))cout<<buff<<endl;}
    else if(s[0]=="exit"){cout<<"Shell is going to exit\n";exit(0);}
    else if(s[0]=="cd"){
        if(s.size()>2){cout<<"Invalid arguments\n";return;}
        string targetDir;
        if(s.size()==1){char*home=getenv("HOME");targetDir=home;}
        else{
            string arg=s[1];
            if(arg=="-"){if(prevDir.empty()){cout<<"No previous directory set\n";return;}targetDir=prevDir;}
            else if(arg=="~"){targetDir=string(getenv("HOME"));}
            else if(arg==".")return;
            else targetDir=arg;
        }
        char cwd[1000];getcwd(cwd,sizeof(cwd));string currDir=cwd;
        if(chdir(targetDir.c_str())==-1)cout<<"Please provide a valid path\n";
        else{prevDir=currDir;getcwd(buff,1000);}
    }
    else if(s[0]=="echo"){
        for(size_t i=1;i<s.size();i++){cout<<s[i];if(i!=s.size()-1)cout<<" ";}
        cout<<endl;
    }
    else if(s[0]=="ls")LS::run(s);
    else if(s[0]=="pinfo")PINFO::handlePinfo(s);
    else if(s[0]=="search")SEARCH::handleSearch(s);
    else SYSCMD::executeCommand(s);
}