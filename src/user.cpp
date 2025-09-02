#include "user.h"
#include <unistd.h>
#include <pwd.h>
#include <limits.h>
#include <string>
using namespace std;

string buildPrompt(string shell_home) {
    char hostname[1000];
    gethostname(hostname,sizeof(hostname));

    struct passwd *pw=getpwuid(getuid());
    string username=pw->pw_name;

    char cwd[10000];
    getcwd(cwd,sizeof(cwd));

    string cdStr(cwd);

    if(cdStr==shell_home) cdStr="~";
    else if(cdStr.find(shell_home+"/")==0) cdStr.replace(0,shell_home.size(),"~");

    return username+"@"+hostname+":"+cdStr+" > ";
}