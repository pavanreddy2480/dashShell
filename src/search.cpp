#include "search.h"
using namespace std;
namespace fs = std::filesystem;

void SEARCH::handleSearch(const vector<string>& args) {
    if (args.size() != 2) {
        cerr << "Usage: search <filename_or_folder>" << endl;
        return;
    }

    string target = args[1];
    bool found = false;

    try {
        for (auto const& entry : fs::recursive_directory_iterator(".")) {
            if (entry.path().filename() == target) {
                found = true;
                break;
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error: " << e.what() << endl;
        return;
    }

    cout << (found ? "True" : "False") << endl;
}