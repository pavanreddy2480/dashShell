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

static vector<string>get_files(const string&prefix){
    vector<string>matches;
    DIR*dir=opendir(".");
    if(!dir)return matches;
    struct dirent*entry;
    while((entry=readdir(dir))!=NULL){
        string name(entry->d_name);
        if(name.rfind(prefix,0)==0)matches.push_back(name);
    }
    closedir(dir);
    return matches;
}

static vector<string>get_commands(const string&prefix){
    vector<string>matches;
    char*path=getenv("PATH");
    if(!path)return matches;
    string pathStr(path);stringstream ss(pathStr);string dirPath;
    while(getline(ss,dirPath,':')){
        DIR*dir=opendir(dirPath.c_str());
        if(!dir)continue;
        struct dirent*entry;
        while((entry=readdir(dir))!=NULL){
            string name(entry->d_name);
            if(name.rfind(prefix,0)==0)matches.push_back(name);
        }
        closedir(dir);
    }
    return matches;
}

static int word_start=0;

static char*completion_generator(const char*text,int state){
    static vector<string>matches;
    static size_t match_index;
    if(state==0){
        matches.clear();match_index=0;
        if(word_start==0)matches=get_commands(text);
        else matches=get_files(text);
    }
    if(match_index<matches.size())return strdup(matches[match_index++].c_str());
    else return NULL;
}

char**my_completion(const char*text,int start,int end){
    rl_attempted_completion_over=1;
    word_start=start;
    return rl_completion_matches(text,completion_generator);
}

void setup_autocomplete(){
    rl_attempted_completion_function=my_completion;
}