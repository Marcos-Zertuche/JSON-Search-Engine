#ifndef Cleaner_H
#define Cleaner_H

#include <stddef.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_set>

#include "./stemmer/porter2_stemmer.h"

using namespace std;

// This class is used to store fileName and frequency of a word
class Cleaner
{
private:
    unordered_set< string> stopwordsLibrary;

public:
    Cleaner &operator=(const Cleaner &other)
    {
        if (this != &other)
        {
            // Copy stopwords library
            stopwordsLibrary = other.stopwordsLibrary;
        }
        return *this;
    }

    void makeStopWordsMap()
    {
        // STOPWORDS FROM https://www.ranks.nl/stopwords
        char fileName[] = "Stopwords/stopwords.csv";
        ifstream f1(fileName);

        // Check if opened
        if (!f1.is_open())
        {
            cerr << "Failed to open file: STOPWORDS\n";
        }
        string stopWord;
        string line;

        while (getline(f1, line))
        {
            stringstream currLine(line);
            getline(currLine, stopWord);

            stopwordsLibrary.insert(stopWord);
        }
        //  for(auto &sw : stopwordsLibrary)
        // {
        //     cout << sw.first << '\n';
        // }

        f1.close();
    }
    string cleanWord(string &w)
    {
        while (!isalpha(w[0]))
        {
            w = w.substr(1);
        }
        return w;
    };
    // Passes each word in here to check if it is a stopword
    // If true is returned, then we do not add it to the Tree and go to next word
    // If false, we put word in the stemmer
    bool stopWords(string wordToCheck)
    {

        if (stopwordsLibrary.find(wordToCheck) == stopwordsLibrary.end())
        {
            return false;
        }

        return true;
    }

    // After we get rid of stopwords, we pass all words to this function to put it in our tree
    string stemWord(string toStem)
    {
        // STRING FOR STEMMER Only
        Porter2Stemmer::trim(toStem);
        Porter2Stemmer::stem(toStem);
        return toStem;
    }
};

#endif