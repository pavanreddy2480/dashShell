#include "ls.h"
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <iomanip>
#include <iostream>
using namespace std;

void MyLS::run(const vector<string>& args){
    MyLS ls;ls.my_ls(args);
}

string MyLS::getPermissions(mode_t mode){
    string perms;
    if(S_ISREG(mode))perms+="-";
    else if(S_ISDIR(mode))perms+="d";
    else if(S_ISLNK(mode))perms+="l";
    else perms+="?";
    perms+=(mode&S_IRUSR)?"r":"-";
    perms+=(mode&S_IWUSR)?"w":"-";
    perms+=(mode&S_IXUSR)?"x":"-";
    perms+=(mode&S_IRGRP)?"r":"-";
    perms+=(mode&S_IWGRP)?"w":"-";
    perms+=(mode&S_IXGRP)?"x":"-";
    perms+=(mode&S_IROTH)?"r":"-";
    perms+=(mode&S_IWOTH)?"w":"-";
    perms+=(mode&S_IXOTH)?"x":"-";
    return perms;
}

void MyLS::listFile(const string& path,bool showAll,bool longFmt){
    struct stat st;
    if(stat(path.c_str(),&st)==-1){perror("stat");return;}
    string name=path;size_t pos=path.find_last_of('/');
    if(pos!=string::npos)name=path.substr(pos+1);
    if(!showAll&&name[0]=='.')return;
    if(longFmt){
        cout<<getPermissions(st.st_mode)<<" "<<setw(2)<<st.st_nlink<<" ";
        struct passwd* pw=getpwuid(st.st_uid);
        struct group* gr=getgrgid(st.st_gid);
        cout<<(pw?pw->pw_name:to_string(st.st_uid))<<" ";
        cout<<(gr?gr->gr_name:to_string(st.st_gid))<<" ";
        cout<<setw(6)<<st.st_size<<" ";
        char timebuf[80];strftime(timebuf,sizeof(timebuf),"%b %d %H:%M",localtime(&st.st_mtime));
        cout<<timebuf<<" "<<name<<endl;
    }else{
        cout<<name<<endl;
    }
}

void MyLS::listDir(const string& dir,bool showAll,bool longFmt){
    auto dp=opendir(dir.c_str());
    if(!dp){perror(("ls: cannot access "+dir).c_str());return;}
    struct dirent* entry;
    while((entry=readdir(dp))!=NULL){
        string name=entry->d_name;
        string fullPath=dir+"/"+name;
        if(name=="."||name=="..") {if(showAll)listFile(fullPath,showAll,longFmt);}
        else listFile(fullPath,showAll,longFmt);
    }
    closedir(dp);
}

void MyLS::my_ls(const vector<string>& args){
    bool showAll=false,longFmt=false;vector<string>paths;
    for(size_t j=1;j<args.size();j++){
        const auto& arg=args[j];
        if(!arg.empty()&&arg[0]=='-'){
            for(size_t i=1;i<arg.size();i++){
                if(arg[i]=='a')showAll=true;
                else if(arg[i]=='l')longFmt=true;
            }
        }else{
            paths.push_back(arg);
        }
    }
    if(paths.empty())paths.push_back(".");
    for(size_t i=0;i<paths.size();i++){
        struct stat st;
        if(stat(paths[i].c_str(),&st)==-1){perror(("ls: cannot access "+paths[i]).c_str());continue;}
        if(S_ISDIR(st.st_mode)){
            if(paths.size()>1)cout<<paths[i]<<":"<<endl;
            listDir(paths[i],showAll,longFmt);
            if(i!=paths.size()-1)cout<<endl;
        }else{
            listFile(paths[i],showAll,longFmt);
        }
    }
}