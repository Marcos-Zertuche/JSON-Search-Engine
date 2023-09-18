# Class Info 
---

## Cleaner Class
  Cleaner makes an unordered_set of stopwords from stopwords.csv in the constructor. Then in both the DocParser and QueryProcessor, the program calls the Cleaner functions stopwords and stemword, respectively. Stopwords returns true when the passed in word matches a stopword in the stopwords. Stemword stems the word to make searching more precise.

## Document Parser
  The document parser makes the three trees for words, persons, and organizations, as well as two maps for uuid and total words in that doc and uuid and path to the file. This is what is used in the query processor to find all the results needed. 
## Index Handler
 IndexHandler is the center of the search engine. Does not have many high-level functions, but it stores the trees and maps in here, and their getters and setters to return the maps and trees using the memory address and pointers in the DocParser and QueryProcessor. 

## QueryProcessor and UI
  Takes in user input and parses it into a vector, then from there, the input is passed into the correct search vectors based on what type it is (include, person, exclude, org), and search the correct tree for each. Then the that include all of the entries are documents , and those are put into a results map. From that map, the uuid’s that will be returned are put in a vector in order from highest frequency to lowest. After that, the uuid is used to find the path needed to print out what the user would see in the terminal. 

## Memory
    Implements persistence in the system, allowing the JSON Search Engine to be ran without having a data set in terminal (as long as the memory has previous history)

## DSTree
Key-element AVL tree implementation. Used to store words, persons, and organizations in the key as strings, and the comparables being unordered_maps or unordered_sets for storing the uuids. 

## Testers and Catch2
Used to test individual components of search engine.