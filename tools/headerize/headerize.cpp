//
// Created by Giulio Zausa on 15/05/18.
//

#include "headerize.h"
using namespace std;

void printUsage() {
    cout << "headerize: generate a C header from bytes" << endl;
    cout << "usage: headerize inputfile outputfile" << endl;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printUsage();
        return 1;
    }

    ifstream inFile;
    inFile.open(argv[1], ios::binary);
    if (!inFile.is_open()) {
        cout << "error reading: " << argv[1] << endl;
        return 1;
    }

    ofstream outFile;
    outFile.open(argv[2], ios::trunc);
    if (!outFile.is_open()) {
        cout << "error writing: " << argv[2] << endl;
        return 1;
    }

    outFile << "/*" << endl;
    outFile << " *  " << argv[1] << endl;
    outFile << " *  Copyright © 2018 Giulio Zausa, Alessio Marotta" << endl;
    outFile << " */" << endl;
    outFile << endl;
    outFile << "#ifndef " << argv[1] << "_H" << endl;
    outFile << "#define " << argv[1] << "_H" << endl;
    outFile << endl;

    outFile << "char file_" << argv[1] << "[] = { ";

    fpos<mbstate_t> inSize = 0;
    inFile.seekg(0, ios::end);
    inSize = inFile.tellg();
    inFile.seekg(0, ios::beg);
    char memblock;
    for (size_t i = 0; i < inSize; i++){
        inFile.read(&memblock, 1);
        outFile << "0x" << hex << setfill('0') << setw(2) << (int)memblock << ", ";
    }
    inFile.close();

    outFile << " };" << endl;

    outFile << endl;
    outFile << "#endif " << endl;
    outFile.close();

    return 0;
}
