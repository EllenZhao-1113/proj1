/* Ellen Zhao
 * Tries.h
 * COMP15
 * Spring 2019
 * Interface for Tries
 */

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

#ifndef _TRIES_H_
#define _TRIES_H_

struct TrieNode {
    TrieNode * children[4];
    bool endWord;
    ~TrieNode();
};

class Tries{
    public:
    //Constructor
    Tries();
    //Destructor
    ~Tries();

    //read  in commands from the text  file and does them
    void readCommands(string file);
    //changes outputfile value
    void setOutputFile(string file);

    private:
    TrieNode * root;    //root of trie
    string outputFile;  //name of output text file
    string sequence;    //sequence of query match
    string beginSeq;    //
    int queryCount;     //common chars; used for query
    int min;            //length of shortest difference for query
    int count;          //final query count
    int anotherWord;    //how many other words are in the same trie branch

    //holds all the possible commands
    void allCommands(string command);
    //INSERT:
    //runs insert command
    void insert(string command);
    //creates a new TrieNode and sets children to null
    TrieNode* newTrieNode();
    //recursively goes through tree and call setChild
    void helperInsert(TrieNode *node, string command);
    //adds node and sets endWord
    void setChild(TrieNode *node, string command, int index);

    //QUERY:
    //runs query command
    void queryCommand(string command);
    //sets values for all variable need for query and calls other query funcions
    bool query(string command);
    //recurses through trie till sequence char isn't in trie
    TrieNode* helperQuery(TrieNode *node, string command);
    //finds the shortest match for sequence
    void findMatch (TrieNode *node, string currSeq, int currentCount);

    //PREFIX:
    //runs prefix command
    void prefixQuery(string command);
    //recurses through trie till end of sequence and prints matching sequences
    void helperPrefix(TrieNode *node, string command, string sequence);

    //REMOVE:
    //runs remove command
    void remove(string command);
    //recursively goes to the leaf of the trie and deletes
    bool helperRemove(TrieNode *node, TrieNode *last, string command, int index);
    //checks if we're done deleting
    bool usingDelete(TrieNode *node, TrieNode *last, string command, int index);
    //deletes node
    bool deleteChild(TrieNode *node, TrieNode *last, string command, int index);

    //NUMBER OF NODES:
    //runs numNodes command
    int numNodes(string command);
    //recursively goes through the whole trie while keeping count
    int helperNumNodes(TrieNode *node);

    //PRINT:
    //runs print command
    void printAll(string command);
    //recursively traverses whole trie to print sequence
    void helperPrintAll(TrieNode *node, string sequence);

    //NEW OUTPUT FILE:
    //runs new outfile file command
    void newFile(string command);

    //DELETES TRIE:
    //runs delete Trie command
    void deleteTrie(string command);
    //recursively deletes trie
    void helperDelete();
};
