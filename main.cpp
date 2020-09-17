/* Ellen Zhao
 * main.cpp
 * COMP15
 * Fall 2019
 * driver for the Tries
 *
 */
#include <iostream>
#include <fstream>
#include <string>
#include "Tries.h"

using namespace std;
void arguementError(int arguement, char* files[]);

int main(int argc, char* argv[]){
    arguementError(argc, argv);
    Tries trie;
    trie.setOutputFile(argv[2]);
    ofstream outfile;
    outfile.open(argv[2], std::ofstream::out | std::ofstream::trunc);
    outfile.close();
    trie.readCommands(argv[1]);
    return 0;
}

//Input: integer (number of arguemnts)
//Returns: nothing
//Does: cerr message when number of arguements is wrong
// or if file cannot be opened
void arguementError(int arguement, char* files[]) {
    if (arguement > 3) {
        cerr << "Usage: ./SeqMatch [query file] [output file]" << endl;
        exit(1);
    }
    ifstream infile;
    infile.open(files[1]);
    if (!infile.is_open()) {
        cerr << "ERROR: File cannot be opened" << endl;
        exit(1);
    }
    infile.close();
    ifstream infile2;
    infile2.open(files[2]);
    if (!infile2.is_open()) {
       cerr << "ERROR: File cannot be opened" << endl;
        exit(1);
    }
    infile2.close();
}
