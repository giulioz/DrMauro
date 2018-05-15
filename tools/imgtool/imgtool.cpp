//
// Created by Giulio Zausa on 15/05/18.
//

#include "imgtool.h"
using namespace std;

void printUsage() {
    cout << "imgtool: generate a C header from bytes" << endl;
    cout << "usage: imgtool ..." << endl;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printUsage();
        return 0;
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

    return 0;
}
