/* Ellen Zhao
 * Tries.cpp
 * COMP15
 * Spring 2019
 * Implementation for Tries functions
 */

#include "Tries.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

//Constructor
Tries::Tries() {
    TrieNode * node = newTrieNode();
    root = node;
    queryCount = 0;
    min = 100;
    anotherWord = 0;
    count = 0;
}

//Deconstructor
Tries::~Tries(){
    delete root;
    root = NULL;
}

//Desctuctor for TrieNodes
TrieNode::~TrieNode() {
  for (int i = 0; i < 4; i++) {
    if (this->children[i] != NULL) {
        delete this->children[i];
        this->children[i] = NULL;
    }
  }
}

//Input: none
//Returns TrieNode pointer
//Does: creates a new TrieNode and sets it children
// to null
TrieNode* Tries::newTrieNode() {
    TrieNode * trie = new TrieNode;
    trie->endWord = false;
    for (int i = 0; i < 4; i++) {
        trie->children[i] = NULL;
    }
    return trie;
}

//Input: string
//Returns: nothing
//Does: Reads the commands from a file and calls the
// correspinding functions to actually do the command
void Tries::readCommands(string file){
    ifstream infile;
    infile.open(file);
    while (!infile.eof()) {
        string command;
        getline(infile, command);
        allCommands(command);
    }
    infile.close();
}

//Input: string
//Returns: nothing
//Does: collection of all the command functions
void Tries::allCommands(string command){
     insert(command);
     queryCommand(command);
     prefixQuery(command);
     printAll(command);
     numNodes(command);
     remove(command);
     newFile(command);
     deleteTrie(command);
}

//Input: string
//Returns: nothing
//Does: checks if the command starts with i,
// if it does, it takes out the first to characters
// to leave the sequence and the calls helper insert
// to recursively add it to the trie
void Tries::insert(string command){
    if (command[0] == 'i') {
        ofstream outfile;
        outfile.open(outputFile, fstream::app);
        command.erase(0,2);
        //checks if the sequence has already been inserted before
        bool inserted = query(command);
        string sequence = command;
        if (inserted == true) {
            outfile << sequence << " was previously inserted" << endl;
        }
        else {
            //resursively inserts
            helperInsert(root, command);
            outfile << sequence << " inserted" << endl;
        }
        outfile.close();
    }
}

//Input: TrieNode pointer, string
//Returns: nothing
//Does: recursively goes through the trie, and checks the first
// char in the string and adds a trienode to the corresponding
// child and then calls itself to do it for the next char
void Tries::helperInsert(TrieNode* node, string command) {
    //command empty = every char of sequence has been inserted
    if (command.empty() == true) {
        return;
    }
    else {
        //goes through trie and adds TrieNode
        if (command[0] == 'A') {
            setChild(node, command, 0);
        }
        if (command[0] == 'C') {
            setChild(node, command, 1);
        }
        if (command[0] == 'G') {
            setChild(node, command, 2);
        }
        if (command[0] == 'T') {
            setChild(node, command, 3);
        }
    }
}

//Input:TrieNode pointer, string, integer
//Return: nothing
//Does: checks if the character has already been added, if it hasn't
// it makes a new node, and removes the first char of the string
// and then recursively does this till it reaches the end of the string
void Tries::setChild(TrieNode *node, string command, int index) {
    //checks if char exists or not
    //if it doesn't, calls newTrieNode to make a new Trienode
    if (node->children[index] == NULL) {
        TrieNode* child = newTrieNode();
        //sets the NULL node to exist
        node->children[index] = child;
    }
    //sets last node of sequence to endWord = true
    if (command.size() == 1) {
        node->children[index]->endWord = true;
        command.clear();
        return;
    }
    //recursive part
    command.erase(0,1);
    helperInsert(node->children[index], command);
}

//Input: string
//Return: nothing
//Does: calls query and prints the information to the file
void Tries::queryCommand(string command) {
    if (command[0] == 'q') {
        command.erase(0,2);
        sequence.clear();
        beginSeq.clear();
        string sequence1 = command;
        //query does all the recursing and finding
        query(command);
        //puts match in output
        ofstream outfile;
        outfile.open(outputFile, fstream::app);
        if (count == 0 or root == NULL) {
            outfile << sequence1 << ": No match found" << endl;
        }
        else {
            outfile << "Match found: " << beginSeq + sequence << " "
                    << count << "%" << endl;
        }
        outfile.close();
        sequence.clear();
        beginSeq.clear();
    }
}

//Input: string
//Returns: bool
//Does: calls helperQuery which gets the last common node and
// keeps count of common sequence chars and then calls findMatch
// to check all the branching sequences from the last common node
// to find the smallest and first alphabetically
bool Tries::query(string command){
    queryCount = 0; //number of common characters (aka. numerator)
    min = 100;      //the smallest sequence's number of chars diff from input seq
    int currentCount = -1; //counts difference of char for all possible matches
    string currSeq;        //sequence of current match
    string sequence1 = command;
    //gets the last common char's node
    TrieNode * last = helperQuery(root, command);
    //looks at the branch sequences for last common node
    //saves match to min and sequence
    findMatch(last, currSeq, currentCount);
    //finds percentage
    double count1 = 0.00;
    int size = sequence1.size();
    //when query sequence is longer than match sequence
    if (size-queryCount > min) {
        count1 = (queryCount*100.00)/(size);
    }
    //when query sequence is shorter than match sequence
    else {
        count1 = (queryCount*100.00)/(queryCount + min);
    }
    count = round(count1);
    if (count == 100) {
        return true;
    }
    return false;
}

//Input: TrieNode pointer and a string
//Returns: TrieNode pointer (of last common character)
//Does: recursively goes through the trie looking at the
// sequence inputted till it doesn't match the trie
// keeps count of common char in sequence and saves sequence
// returns the pointer to the node of the last common char
TrieNode* Tries::helperQuery(TrieNode *node, string command) {
    TrieNode * last = node;
    if (command[0] == 'A' and node->children[0] != NULL) {
        command.erase(0,1);
        queryCount++;
        beginSeq.push_back('A');
        return helperQuery(node->children[0], command);
    }
    if (command[0] == 'C' and node->children[1] != NULL) {
        command.erase(0,1);
        queryCount++;
        beginSeq.push_back('C');
        return helperQuery(node->children[1], command);
    }
    if (command[0] == 'G' and node->children[2] != NULL) {
        command.erase(0,1);
        queryCount++;
        beginSeq.push_back('G');
        return helperQuery(node->children[2], command);
    }
    if (command[0] == 'T' and node->children[3] != NULL) {
        command.erase(0,1);
        queryCount++;
        beginSeq.push_back('T');
        return helperQuery(node->children[3],  command);
    }
    return last;
}

//Input: TrieNode pointer, string, and integer
//Returns:string
//Does:goes through the children of the last node in common of
// the sequence inputted and the trie, looking for the sequence
// in the trie that is the shortest and alphabetically first
void Tries::findMatch(TrieNode *node, string currSeq, int currentCount) {
    currentCount++;
    string seq1, seq2, seq3, seq4;
    //when it reaches the end of a word, updates min and sequence
    if (node->endWord == true) {
        if (currentCount < min) {
            min = currentCount;
            sequence = currSeq;
        }
    }
    //recurses through the branches of trie
    //adds chars to currSeq
    for (int i = 0; i < 4; i++) {
        if (node->children[i] != NULL) {
            if (i == 0) {
                seq1.push_back('A');
                findMatch(node->children[i], currSeq+seq1, currentCount);
            }
            if (i == 1) {
                seq2.push_back('C');
                findMatch(node->children[i], currSeq+seq2, currentCount);
            }
            if (i == 2) {
                seq3.push_back('G');
                findMatch(node->children[i], currSeq+seq3, currentCount);
            }
            if (i == 3) {
                seq4.push_back('T');
                findMatch(node->children[i], currSeq+seq4, currentCount);
            }
        }
    }
}

//Input: string
//Returns: nothing
//Does: checks if command starts wiht pq
// then calls helper funtion
void Tries::prefixQuery(string command){
    if (command[0] == 'p' and command[1] == 'q') {
        string sequence;
        command.erase(0,3);
        //recurses
        helperPrefix(root,command, command);
    }
}

//Input: TrieNode pointer and two strings
//Returns: nothing
//Does: recursively goes through the trie, till it reaches the
// last char in the sequence and then call helperprintall to
// print all sequences with that staring sequence
void Tries::helperPrefix(TrieNode *node, string command, string sequence) {
    //char not in string = node is null
    if (node == NULL) {
        ofstream outfile;
        outfile.open(outputFile, fstream::app);
        outfile << sequence << ": No match found" << endl;
        outfile.close();
        return;
    }
    else {
        //empty string = prefix sequence is in trie, calls printAll
        if (command.empty() == true) {
            helperPrintAll(node, sequence);
            return;
        }
        //recurive part
        if (command[0] == 'A') {
            command.erase(0,1);
            return helperPrefix(node->children[0], command, sequence);
        }
        if (command[0] == 'C') {
            command.erase(0,1);
            return helperPrefix(node->children[1],command, sequence);
        }
        if (command[0] == 'G') {
            command.erase(0,1);
            return helperPrefix(node->children[2], command, sequence);
        }
        if (command[0] == 'T') {
            command.erase(0,1);
            return helperPrefix(node->children[3], command, sequence);
        }
    }
}

//Input: string
//Returns: nothing
//Does: calls query and helperRemove and prints out
// if it isn't a match and if it is, that the string is removed
void Tries::remove(string command){
    if (command[0] == 'r') {
        int index = 0;
        command.erase(0,2);
        ofstream outfile;
        outfile.open(outputFile, fstream::app);
        //checks if sequence exists
        bool match = query(command);
        if (match == true) {
            //removes sequence
            bool removed = helperRemove(root, NULL, command, index);
            anotherWord = 0;
            if (removed == true) {
                outfile << command << " removed" << endl;
                return;
            }
        }
        else {
            outfile << command << ": No match found" << endl;
        }
    }
}

//Input: TrieNode pointer, string, and integer
//Returns: boolean
//Does: recursively goes down the trie, till it is at the the
// leaf then calls delete function if the node only if the node
// doesn't have children or is the end to a different sequence
bool Tries::helperRemove(TrieNode *node, TrieNode* last,
                         string command, int index) {
    //if command is empty, it means everything was deleted
    if (command.empty() == true) {
        return true;
    }
    int size = command.size();
    //means we are at the leaf (aka. can delete)
    if (index == size) {
        if (node != NULL) {
            if (node->endWord == true and anotherWord != 0 and node != NULL) {
                return true;
            }
            //deletes node
            bool deleted = deleteChild(node, last, command, index-1);
            if (deleted == true) {
                command.pop_back();
                //recurses through trie to the new leaf
                return helperRemove(root, NULL, command, 0);
            }
            else {
              return true;
            }
        }
    }
    else {
        //recursive part
        if (command[index] == 'A') {
            index++;
            return helperRemove(node->children[0], node, command, index);
        }
        if (command[index] == 'C') {
            index++;
            return helperRemove(node->children[1], node, command, index);
        }
        if (command[index] == 'G') {
            index++;
            return helperRemove(node->children[2], node, command, index);
        }
        if (command[index] == 'T') {
            index++;
            return helperRemove(node->children[3], node, command, index);
        }
    }
    return false;
}

//Input: two TrieNode pointers, string, and an integer
//Returns: a boolean
//Does: checks if node needs to be deleted on the basis if
// the node is apart of another node or of the node has
bool Tries::usingDelete(TrieNode *node, TrieNode *last,
                        string command, int index) {
    //when node is the end of another word, doesn't delete
    if (node->endWord == true and anotherWord != 0) {
        return true;
    }
    //deletes node
    bool deleted = deleteChild(node, last, command, index-1);
    if (deleted == true) {
        command.pop_back();
        //recurses through trie to the new leaf
        return helperRemove(root, NULL, command, 0);
    }
    return true;
}

//Input: TrieNode pointer, string, integer
//Returns: boolean
//Does: checks if the last char of the sequence is a leaf, if it is, then it
// deletes it and sets it it's place in the parent to NULL
// also keeps track if there is another word in the sequence
bool Tries::deleteChild(TrieNode *node, TrieNode *last, string command, int index) {
    //if node is a leaf
    if (node->children[0] == NULL and node->children[1] == NULL and
        node->children[2] == NULL and node->children[3] == NULL) {
        if (node->endWord == true) {
            anotherWord++;
        }
        delete node;
        node = NULL;
        //sets parent's child to NULL
        if (command[index] == 'A') {
            last->children[0] = NULL;
        }
        if (command[index] == 'C') {
            last->children[1] = NULL;
        }
        if (command[index] == 'G') {
            last->children[2] = NULL;
        }
        if (command[index] == 'T') {
            last->children[3] = NULL;
        }
        return true;
    }
    else {
        //resets endWord
        if (node->endWord == true) {
            node->endWord = false;
            return false;
        }
    }
    return false;
}

//Input: none
//Returns: integer
//Does: calls helperNumNodes
int Tries::numNodes(string command){
    int number = 0;
    if (command[0] == 'n') {
        command.clear();
        number = helperNumNodes(root)-1;
        ofstream outfile;
        outfile.open(outputFile, fstream::app);
        if (number == -1) {
          number++;
        }
        outfile << "There are " << number << " nodes in the database" << endl;
        outfile.close();
    }
    return number;
}

//Input: TrieNode pointer
//Returns: integer
//Does: recursively goes through the trie and counts
// each node that is in it with root = 0
int Tries::helperNumNodes(TrieNode * node) {
    int count = 0;
    if (node == NULL) {
        return count;
    }
    else {
        count = 1 + helperNumNodes(node->children[0]) +
        helperNumNodes(node->children[1]) + helperNumNodes(node->children[2]) +
        helperNumNodes(node->children[3]);
    }
    return count;
}

//Input: string
//Returns: nothing
//Does: checks if command starts with p
// then calls helper function to print into a file
void Tries::printAll(string command) {
    int commandSize = command.size();
    if (command[0] == 'p' and commandSize == 1) {
        string sequence;
        command.erase(0,1);
        helperPrintAll(root, sequence);
        sequence.clear();
    }
}

//Input: TrieNode pointer and a string
//Returns: Nothing
//Does: goes through a node's children using a four loop and recursively
// goes through all the nodes, if the node is the end of a word,
// it prints the sequence into a file
void Tries::helperPrintAll(TrieNode *node, string sequence) {
    string subseq1, subseq2, subseq3, subseq4;
    //prints sequence
    if (node->endWord == true) {
        ofstream outfile;
        outfile.open(outputFile, fstream::app);
        outfile << sequence << endl;
        outfile.close();
    }
    //recurses through trie and adds letters to sequence
    for (int i = 0; i < 4; i++) {
        if (node->children[i] != NULL) {
            if (i == 0) {
                subseq1.push_back('A');
                helperPrintAll(node->children[i], sequence+subseq1);
            }
            if (i == 1) {
                subseq2.push_back('C');
                helperPrintAll(node->children[i], sequence+subseq2);
            }
            if (i == 2) {
                subseq3.push_back('G');
                helperPrintAll(node->children[i], sequence+subseq3);
            }
            if (i == 3) {
                subseq4.push_back('T');
                helperPrintAll(node->children[i], sequence+subseq4);
            }
        }
    }
}

//Input: string
//Returns: nothing
//Does: checks if command starts with an f, if it does
// calls setOutputFile and prints corresponding statement into file
void Tries::newFile(string command) {
    if (command[0] == 'f') {
        command.erase(0,2);
        setOutputFile(command);
        ofstream outfile;
        outfile.open(outputFile, fstream::app);
        outfile << "Output file has been changed to " << command << endl;
        outfile.close();
  }
}

//Input: string
//Returns: nothing
//Does: changes the private variable outputFile to something new
// clears new outputfile so it's empty
void Tries::setOutputFile(string file) {
    outputFile = file;
    ofstream outfile;
    outfile.open(outputFile, std::ofstream::out | std::ofstream::trunc);
    outfile.close();
}

//Input: string
//Returns: nothing
//Does: checks if command is to delete, and deletes by calling helperDelete
void Tries::deleteTrie(string command) {
  if (command[0] == 'd') {
    command.clear();
    helperDelete();
    numNodes("n");
  }
}

//Input: TrieNode pointer
//Returns: nothing
//Does: recursively deletes the trie
void Tries::helperDelete() {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        delete root->children[i];
        root->children[i] = NULL;
    }
}



     
