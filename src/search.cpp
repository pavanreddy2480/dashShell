#include "search.h"
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include <unistd.h>
using namespace std;

static bool doSearch(const string& dir,const string& target){
    DIR* dp=opendir(dir.c_str());
    if(!dp) return false;
    struct dirent* ent;
    while((ent=readdir(dp))!=nullptr){
        string entryName=ent->d_name;
        if(entryName=="."||entryName=="..") continue;
        if(entryName==target){
            closedir(dp);
            return true;
        }
        if(ent->d_type==DT_DIR){
            string subPath=dir+"/"+entryName;
            if(doSearch(subPath,target)){
                closedir(dp);
                return true;
            }
        }
    }
    closedir(dp);
    return false;
}

void SEARCH::handleSearch(const vector<string>& args){
    if(args.size()!=2){
        cerr<<"Usage: search <filename_or_folder>"<<endl;
        return;
    }
    string target=args[1];
    cout<<(doSearch(".",target)?"True":"False")<<endl;
}