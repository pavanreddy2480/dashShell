#include "parser.h"
#include <cstring>

vector<Command> Parser::parse(const string &line) {
    vector<Command> commands;

    char *cstr=strdup(line.c_str());
    char *cmd=strtok(cstr,"|");
    while(cmd){
        commands.push_back(parseCommand(string(cmd)));
        cmd=strtok(NULL,"|");
    }

    free(cstr);
    return commands;
}

Command Parser::parseCommand(const string &cmd){
    Command result;auto tokens=split_tokens(cmd);
    cout<<"tokens : "<<endl;
    for(int i=0;i<tokens.size();i++){
        cout<<tokens[i]<<" ";
        if(tokens[i]=="<"&&i+1<tokens.size()){result.inputFile=tokens[i+1];i++;}
        else if(tokens[i]==">"&&i+1<tokens.size()){result.outputFile=tokens[i+1];result.append=false;i++;}
        else if(tokens[i]==">>"&&i+1<tokens.size()){result.outputFile=tokens[i+1];result.append=true;i++;}
        else result.argv.push_back(tokens[i]);

    }
    cout<<endl;
    for(auto &i:result.argv) cout<<i<<" ";
    cout<<endl;
    cout<<result.inputFile<<endl;
    cout<<result.outputFile<<endl;
    return result;
}

vector<string> Parser::split_tokens(const string &cmd){
    vector<string> tokens;char *cstr=strdup(cmd.c_str());
    char *tok=strtok(cstr," \t");
    while(tok){tokens.push_back(tok);tok=strtok(NULL," \t");}
    free(cstr);return tokens;
}