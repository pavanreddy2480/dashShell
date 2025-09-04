#ifndef LS_H
#define LS_H

#include <string>
#include <vector>

class LS {
public:
    static void run(const std::vector<std::string>& args);

private:
    std::string getPermissions(mode_t mode);
    void listFile(const std::string &path, bool showAll, bool longFmt);
    void listDir(const std::string &dir, bool showAll, bool longFmt);
    void ls(const std::vector<std::string>& args);
};

#endif