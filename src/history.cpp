#include "history.h"
#include <iostream>
#include <fstream>
#include <cstdlib>   // for getenv
#include <algorithm> // for max

History::History() {
    const char *home = getenv("HOME");
    if (home) {
        historyFile = std::string(home) + "/.myshell_history";
    } else {
        historyFile = ".myshell_history"; // fallback
    }
    load();
}

void History::add(const std::string &cmd) {
    if (cmd.empty()) return;

    if ((int)commands.size() < MAX_HISTORY) {
        commands.push_back(cmd);
    } else {
        overwriteOldest(cmd);
    }
    save();
}

void History::overwriteOldest(const std::string &cmd) {
    // remove oldest (front) and push new command
    commands.erase(commands.begin());
    commands.push_back(cmd);
}

void History::print(int num) const {
    if (commands.empty()) {
        std::cout << "No history available.\n";
        return;
    }

    num = std::min(num, (int)commands.size());
    int start = commands.size() - num;
    for (int i = start; i < (int)commands.size(); i++) {
        std::cout << commands[i] << "\n";
    }
}

void History::load() {
    commands.clear();
    std::ifstream in(historyFile);
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) {
            if ((int)commands.size() < MAX_HISTORY)
                commands.push_back(line);
            else
                overwriteOldest(line);
        }
    }
}

void History::save() const {
    std::ofstream out(historyFile, std::ios::trunc);
    for (const auto &cmd : commands) {
        out << cmd << "\n";
    }
}