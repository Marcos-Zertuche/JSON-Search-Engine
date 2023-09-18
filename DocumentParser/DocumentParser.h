#ifndef DocumentParser_H
#define DocumentParser_H

// Normal includes for reading files
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <chrono>

// JSON Includes
#include "../rapidjson/document.h"
#include "../rapidjson/istreamwrapper.h"

// Class Includes
#include "../IndexHandler/IndexHandler.h"
#include "../CleanerClass/Cleaner.h"
#include "../DSTree.h"

using namespace std;
using namespace rapidjson;

class DocumentParser
{
private:
    IndexHandler ih;
    Cleaner clean;

    // Word-Frequency Map
    unordered_map<string, unordered_map<string, int>> word_frequency_map;

    int total_documents{0};
public:
    DocumentParser(){

    };
    DocumentParser(const string &folder, Cleaner &c)
    {
       auto startDocParse = chrono::high_resolution_clock::now();

        checkMemory();
        clean = c;
         
        openFolder(folder);

        auto endDocParse = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsedDocParse = endDocParse - startDocParse;
        cout << "Total Documents: " << total_documents << "\n" 
        << "Elapsed time for creating library ::: " << elapsedDocParse.count() << " seconds\n"
        << "DOCS PARSED PER SECOND: " << (1.00 * total_documents )/ elapsedDocParse.count() << "\n\n";

    }
    DocumentParser &operator=(const DocumentParser &rhs)
    {
        if (this != &rhs)
        {
            ih = rhs.ih;
        }
        return *this;
    }

    void readJsonFile(const string &fileName); // Reads Individual JSON File
    void openFolder(const string &path);       // Opens folder

    // Getters for trees
    DSTree<string, unordered_map<string, int>> *words();
    DSTree<string, unordered_set<string>> *persons();
    DSTree<string, unordered_set<string>> *organizations();

    // Pull Titles by using uuid
    unordered_map<string, string> *id_path();

    // Pull uuids with their word count
    unordered_map<string, int> *uuid_totalWords();

    // Pull word frequency map
    unordered_map<string, unordered_map<string, int>> *wordFrequencies();

    // Check vectors in maps
    void testMaps();
    void checkUUIDmap();

    //-----PERSISTENCE
    void checkMemory(); //<-- Reads saved data
    void storeData(); //<--- Stores data
};

#endif