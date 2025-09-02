#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <vector>

class History {
public:
    History();

    // Add a command to history
    void add(const std::string &cmd);

    // Print last num commands (default 10)
    void print(int num = 10) const;

    // Load history from file
    void load();

    // Save history to file
    void save() const;

private:
    std::vector<std::string> commands;
    const int MAX_HISTORY = 20;
    std::string historyFile;

    void overwriteOldest(const std::string &cmd);
};

#endif