# JSON Search Engine
## Overview
This JSON Search Engine is built to compile a library of JSON Documents via user inputs, and allow the user to perform a search that replicates a normal search engine through the terminal. The user can search words, people, and organizations, and also has the ability to omit words from the search. The results are returned in order by relativity score, prioritizing persons and organizations determined by users, followed by frequency of search words in the body of the JSON Document.

## How it works:
First, the user will run `./supersearch ./<folder-containing-JSON-files>*` in their terminal to begin the program. 

The program will take time to parse through each JSON Document, creating DSTree (a personal implementation of a key-element AVL Tree) for words, persons, and organizations that we will use for search. Additionally, there are unordered_maps created that use the JSON Document UUID as a key, one for keeping track of total words and one for the path to the file.

Once these are created, a menu appears on the UI, prompting the user for a search. The search can have 4 types of entries:
    1. Search by word `word`
    2. Search by person `PERSON:<name>`
    3. Search by organization `ORG:<name>`
    4. Omit by word `-word`

After user input, the top 10 results are ordered by relativity score. Persons and Org hits add 1 to the score, and word searches add the frequency percentage ( word frequency / total words in document) to the relativity score.

The user can have as many inputs as they need. When concluding, the user will type `QUIT`, terminating the program. 

Before termination, the DSTrees and Unordered_maps containing all data are saved in Memory to implement persistence (improving speed for reopening the program and expaning the library)



## Class Information

## Performance
#

