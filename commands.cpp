#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "ls.cpp"
#include "pinfo.cpp"
#include "search.cpp"
#include "fg.cpp"
using namespace std;

class customCommands {
public:
    static string prevDir; // to handle "cd -"

    static void cmd(vector<string> &s, char buff[],string home_dir) {
        if (s.empty()) return;

        if (s[0] == "pwd") {
            if (getcwd(buff, 1000)) {
                if(string(buff)==home_dir) cout<<'~'<<endl;
                else cout << buff << endl;
            }
        } 
        
        else if (s[0] == "exit") {
            cout << "Shell is going to exit\n";
            exit(0);
        } 
        
        else if (s[0] == "cd") {
            if (s.size() > 2) {
                cout << "Invalid arguments\n";
                return;
            }

            string targetDir;
            if (s.size() == 1) {
                // no args → go to $HOME
                char *home = getenv("HOME");
                targetDir = home ? home : "/";
            } 
            else {
                string arg = s[1];

                if (arg == "-") {
                    if (prevDir.empty()) {
                        cout << "No previous directory set\n";
                        return;
                    }
                    targetDir = prevDir;
                } 
                else if (arg == "~") {
                    targetDir = home_dir;
                } 
                else if (arg == ".") {
                    return; // stay same dir
                } 
                else {
                    targetDir = arg;
                }
            }

            // store current dir before changing
            char cwd[1000];
            getcwd(cwd, sizeof(cwd));
            string currDir = cwd;

            if (chdir(targetDir.c_str()) == -1) {
                cout << "Please provide a valid path\n";
            } else {
                prevDir = currDir; // update previous
                getcwd(buff, 1000);
                // cout << buff << endl; // print absolute path after cd
            }
        } 
        
        else if (s[0] == "echo") {
            // print everything after echo with spaces preserved
            for (size_t i = 1; i < s.size(); i++) {
                cout << s[i];
                if (i != s.size() - 1) cout << " ";
            }
            cout << endl;
        }

        else if(s[0]=="ls"){
            MyLS::run(s);
        }
        else if(s[0]=="pinfo"){
            PINFO::handlePinfo(s);
        }
        else if(s[0]=="search"){
            SEARCH::handleSearch(s);
        }
        else{
            SYSCMD::executeCommand(s);
        }
    }
};

string customCommands::prevDir = ""; // initializ