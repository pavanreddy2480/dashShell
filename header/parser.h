// parser.h
#include <bits/stdc++.h>
#ifndef PARSER_H
#define PARSER_H
using namespace std;

struct Command {
    vector<string> argv;
    string inputFile="";
    string outputFile="";
    bool append=false;
};

class Parser {
public:
    static vector<Command> parse(const string &line);

private:
    static Command parseCommand(const string &cmd);
    static vector<string> split_tokens(const string &cmd);
};
#endif
