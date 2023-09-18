#ifndef IndexHandler_H
#define IndexHandler_H

#include "../DSTree.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>

using namespace std;

class IndexHandler
{
private:
    DSTree<string, unordered_map<string, int>> words;    // WILL BE VECTOR OF WORDINFO //Holds word and UUID in + frequency in doc
    DSTree<string, unordered_set<string>> persons;       // Holds persons and UUID person appears in
    DSTree<string, unordered_set<string>> organizations; // Holds orgs and UUID org appears in
    unordered_map<string, string> UUID_PATH;             // UUID , PATH

    // Stores uuid with number of words in the doc
    unordered_map<string, int> uuid_totalWords;

public:
    IndexHandler();
    void addToWordTree(string, string, int);
    void addToPersonTree(string, string);
    void addToOrgTree(string, string);
    void addToUuid_PathMap(string, string);
    void addToUuid_totalWords(string , int );

    // GETTERS
    DSTree<string, unordered_map<string, int>> *getWordsTree();
    DSTree<string, unordered_set<string>> *getPersonsTree();
    DSTree<string, unordered_set<string>> *getOrganizationsTree();
    unordered_map<string, string> *getUuidPathMap();

    unordered_map<string, int> *getUuid_totalWords();

    // // Persistence
    void checkMemory();
    void checkWordsMemory();
    void checkPersonsMemory();
    void checkOrgsMemory();
    void checkUUID_PathsMemory();
    void checkUUID_WordCountMemory();

    // // Save
    void storeData();
    void saveWordsTree();
    void savePersonsTree();
    void saveOrgsTree();
    void saveUuidPathMap();
    void saveUuidWordCountMap();


};

#endif