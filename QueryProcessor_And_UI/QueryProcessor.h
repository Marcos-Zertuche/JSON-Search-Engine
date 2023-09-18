#ifndef QueryProcessor_H
#define QueryProcessor_H

// Normal includes for reading files
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

// Class Includes
#include "../DSTree.h"
#include "../DocumentParser/DocumentParser.h"


#include <unordered_map>
#include <map>

using namespace std;

class QueryProcessor
{
private:
    DocumentParser dp;
    Cleaner clean;

    // Pointers to maps
    DSTree<string, unordered_map<string, int>> *wordsLibrary;
    DSTree<string, unordered_set<string>> *personsLibrary;
    DSTree<string, unordered_set<string>> *orgsLibrary;
    unordered_map<string, string> *uuid_paths_map;
    unordered_map<string, int> *uuids_totalWords;

    // Vectors for ordering by frequency (greatest to least)
    vector<pair<string, double>> uuids_frequencies;

    // SIZE FOR COMPARISON
    int resultSize;

    // KEEP IT TO 15 outputs
    int resultsCounter;

    // Regular map to return documents
    unordered_map<string, int> results;

    // FOR ACTIVE SEARCH
    vector<string> includes;
    vector<string> excludes;
    vector<string> persons;
    vector<string> orgs;

public:
    QueryProcessor();
    QueryProcessor(const string &folder);
    void search(vector<string> &exclude, vector<string> &include, vector<string> &persons, vector<string> &orgs);
    void search_include(vector<string> &);
    void search_ignore(vector<string> &);
    void search_persons(vector<string> &);
    void search_orgs(vector<string> &);
    void getSearchResult();
    void orderResultsWord(string &);
    void orderResultsPerson(string&);
    void orderResultsOrg(string&);

    static bool sortbysec(pair<string, double> &a, pair<string, double> &b);
    void beginPrint();
    void printSearchResults(const string &, double &);
    void tester();

    // FOR ACTIVE SEARCH
    void start();

    //Persistence
    void storeData();

    int getSize();
};

#endif