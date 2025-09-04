#include"auto.h"
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<string>
#include<sstream>
#include<readline/readline.h>
#include<readline/history.h>
using namespace std;

static vector<string>collectMatches(const string&prefix,bool searchPath){
    vector<string>results;
    if(searchPath){
        char*pathEnv=getenv("PATH");
        if(!pathEnv) return results;
        string pathStr(pathEnv);
        stringstream ss(pathStr);
        string dir;
        while(getline(ss,dir,':')){
            DIR*dp=opendir(dir.c_str());
            if(!dp) continue;
            struct dirent*ent;
            while((ent=readdir(dp))!=NULL){
                string name(ent->d_name);
                if(name.rfind(prefix,0)==0) results.push_back(name);
            }
            closedir(dp);
        }
    }else{
        DIR*dp=opendir(".");
        if(!dp) return results;
        struct dirent*ent;
        while((ent=readdir(dp))!=NULL){
            string name(ent->d_name);
            if(name.rfind(prefix,0)==0) results.push_back(name);
        }
        closedir(dp);
    }
    return results;
}

static int startPos=0;

static char*completionGenerator(const char*text,int state){
    static vector<string>candidates;
    static size_t idx;
    if(state==0){
        candidates.clear();
        idx=0;
        candidates=collectMatches(text,startPos==0);
    }
    if(idx<candidates.size()) return strdup(candidates[idx++].c_str());
    return NULL;
}

char**myCompletion(const char*text,int start,int end){
    rl_attempted_completion_over=1;
    startPos=start;
    return rl_completion_matches(text,completionGenerator);
}

void setupAutocomplete(){
    rl_attempted_completion_function=myCompletion;
}