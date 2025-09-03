#include "search.h"
using namespace std;
namespace fs=std::filesystem;

void SEARCH::handleSearch(const vector<string>& args){
    if(args.size()!=2){
        cerr<<"Usage: search <filename_or_folder>"<<endl;
        return;
    }
    try{
        for(auto const& entry:fs::recursive_directory_iterator(".")){
            if(entry.path().filename()==args[1]){
                cout<<"True"<<endl;
                return;
            }
        }
    }catch(const fs::filesystem_error& e){
        cerr<<"Error: "<<e.what()<<endl;
        return;
    }

    cout<<"False"<<endl;
}