// parser.cpp
#include <bits/stdc++.h>
using namespace std;

struct Command {
    vector<string> argv;   // command + args
    string inputFile="";
    string outputFile="";
    bool append=false;
};

class Parser {
public:
    static vector<Command> parse(const string &line) {
        vector<Command> commands;

        // split pipeline
        char *cstr = strdup(line.c_str());
        char *cmd = strtok(cstr, "|");
        while (cmd) {
            commands.push_back(parseCommand(string(cmd)));
            cmd = strtok(NULL, "|");
        }
        free(cstr);

        return commands;
    }

private:
    static Command parseCommand(const string &cmd) {
        Command result;
        auto tokens = split_tokens(cmd);

        for (size_t i = 0; i < tokens.size(); i++) {
            if (tokens[i] == "<" && i + 1 < tokens.size()) {
                result.inputFile = tokens[i + 1];
                i++;
            } else if (tokens[i] == ">" && i + 1 < tokens.size()) {
                result.outputFile = tokens[i + 1];
                result.append = false;
                i++;
            } else if (tokens[i] == ">>" && i + 1 < tokens.size()) {
                result.outputFile = tokens[i + 1];
                result.append = true;
                i++;
            } else {
                result.argv.push_back(tokens[i]);
            }
        }

        return result;
    }

    static vector<string> split_tokens(const string &cmd) {
        vector<string> tokens;
        char *cstr = strdup(cmd.c_str());

        const char *delim = " \t";
        char *tok = strtok(cstr, delim);
        while (tok) {
            tokens.push_back(tok);
            tok = strtok(NULL, delim);
        }

        free(cstr);
        return tokens;
    }
};
