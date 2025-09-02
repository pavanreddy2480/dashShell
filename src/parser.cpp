#include "parser.h"
#include <cstring>

vector<Command> Parser::parse(const string &line) {
    vector<Command> commands;
    stringstream ss(line);
    string segment;

    while (getline(ss, segment, '|')) {
        // trim leading/trailing whitespace
        segment.erase(0, segment.find_first_not_of(" \t\n\r"));
        segment.erase(segment.find_last_not_of(" \t\n\r") + 1);

        if (!segment.empty()) {
            commands.push_back(parseCommand(segment));
        }
    }

    return commands;
}

Command Parser::parseCommand(const string &cmd){
    Command result;auto tokens=split_tokens(cmd);
    for(int i=0;i<tokens.size();i++){
        if(tokens[i]=="<"&&i+1<tokens.size()){result.inputFile=tokens[i+1];i++;}
        else if(tokens[i]==">"&&i+1<tokens.size()){result.outputFile=tokens[i+1];result.append=false;i++;}
        else if(tokens[i]==">>"&&i+1<tokens.size()){result.outputFile=tokens[i+1];result.append=true;i++;}
        else result.argv.push_back(tokens[i]);

    }
    return result;
}

vector<string> Parser::split_tokens(const string &cmd){
    vector<string> tokens;char *cstr=strdup(cmd.c_str());
    char *tok=strtok(cstr," \t");
    while(tok){tokens.push_back(tok);tok=strtok(NULL," \t");}
    free(cstr);return tokens;
}