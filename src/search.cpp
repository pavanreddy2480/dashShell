#include "search.h"
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include <unistd.h>
using namespace std;

bool searchRecursive(const string& dir,const string& target){
    DIR* d=opendir(dir.c_str());
    if(!d) return false;
    struct dirent* entry;
    while((entry=readdir(d))!=nullptr){
        string name=entry->d_name;
        if(name=="."||name=="..") continue;
        if(name==target){
            closedir(d);
            return true;
        }
        if(entry->d_type==DT_DIR){
            string subdir=dir+"/"+name;
            if(searchRecursive(subdir,target)){
                closedir(d);
                return true;
            }
        }
    }
    closedir(d);
    return false;
}

void SEARCH::handleSearch(const vector<string>& args){
    if(args.size()!=2){
        cerr<<"Usage: search <filename_or_folder>"<<endl;
        return;
    }
    string target=args[1];
    if(searchRecursive(".",target)) cout<<"True"<<endl;
    else cout<<"False"<<endl;
}