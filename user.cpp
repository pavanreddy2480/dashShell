#include "fcntl.h"
#include "sys/wait.h"
#include "vector"
#include <cstdlib>
#include <iostream>
#include <string>
#include <type_traits>
#include <unistd.h>
#include <vector>
#include <pwd.h>        // for getpwuid()
#include <limits.h>   
using namespace std;


string buildPrompt(string shell_home) {
    char hostname[1000];
    gethostname(hostname, sizeof(hostname));

    // username
    struct passwd *pw = getpwuid(getuid());
    string username = pw ? pw->pw_name : "user";

    // cwd
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    string cwdStr = cwd;
    if(!shell_home.size()) shell_home=cwdStr;
    // replace only relative to shell_home
    if (cwdStr == shell_home) {
        cwdStr = "~";
    } else if (cwdStr.find(shell_home + "/") == 0) {
        cwdStr.replace(0, shell_home.size(), "~");
    }

    string prompt = username + "@" + hostname + ":" + cwdStr + " > ";
    return prompt;
}
