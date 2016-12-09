/**
 * Small program to help parse through the arrays copied from DAVE to
 * a csv format to open in Excel.
 */

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]){
    using namespace std;
        // Usage: program.exe [source filepath] [destination filepath] [array_name]
    if(argc < 3){
        string prog(argv[0]);
        cerr << "Usage: " << prog.substr(prog.find_last_of('\\')+1) << " [filepath] [destination filepath] [array_name]" << endl;
        return 0;
    }

    clog << "Opening files (" << argv[1] << " and " << argv[2] << ") ...\n";

    ifstream src(argv[1]);
    ofstream dest(argv[2]);
    if(!src.is_open()){
        cerr << "\tCould not open " << argv[1] << " for reading. Aborting..." << endl;
        return 0;
    }
    if(!dest.is_open()){
        cerr << "\tCould not open " << argv[2] << " for reading. Aborting..." << endl;
        return 0;
    }

    clog << "\tParsing data...\n";
        // Expected format: target_name[N] data_type value
    string dump, target_name(argv[3]);
    dest
        << target_name << '\n'
        << "Data point,Value (V)\n"
        ;
    size_t counter = 0;
    while(getline(src, dump)){
        stringstream line;
        line << dump;
        line >> ws;
        line >> dump;
        if(
            dump.substr(0, target_name.size()) == target_name &&
            dump[target_name.size()] == '['
        ){
            float value = 0.0f;
            line >> dump >> value;
            dest << (counter++) << ',' << value << '\n';
        }
    }
    clog << "\tFound " << counter << " matches.\n";

    clog << "Completed parsing. Exiting program..." << endl;
    src.close();
    dest.close();
    return 0;
}