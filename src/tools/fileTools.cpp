#include "ProtosCloudServer/tools/fileTools.h"

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

namespace fileTools {

string read(const string& filePath) {
    ifstream in(filePath, ios::in | ios::binary);
    in.exceptions(ifstream::failbit | ifstream::badbit);
    ostringstream contents;
    contents << in.rdbuf();
    in.close();
    return contents.str();
}

void write(const string& content, const string& filePath) {
    ofstream out(filePath, ios::out | ios::binary);
    out.exceptions(ofstream::failbit | ofstream::badbit);
    out << content;
    out.close();
}

}
