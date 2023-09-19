# JSON Search Engine
---
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

Before termination, the DSTrees and Unordered_maps containing all data are saved in Memory to implement persistence (improving speed for reopening the program and expanding the library)


## Class Information
[See ClassInfo.md](https://github.com/Marcos-Zertuche/JSON-Search-Engine/blob/main/ClassInfo.md)

## Performance

### Creating the Library
*All results taken with clean memory*
| Total Documents  | Time (sec)| Documents/Sec |
| ------------ | ------------ | ------------ |
|  10 | 0.033 | 305.357 |
| 100 | 0.230 | 434.246 |
| 1000 | 2.316 | 431.76 |
| 10000 | 22.970 | 435.335 |
| 25000 | 51.7584 | 482.99 |
| 50000 | 102.3 | 488.76 |



### Search

| Number of Documents | Search | Results | Time (sec) |
| ----- | ------- | ----- | ----- |
| 50000 | reuters | 29406 | 50.166 |
| 50000 | economic growth | 2307 | 1.344|
| 50000 | ORG:fed | 489 | 0.016 |
| ----- | ------- | ----- | ----- |
| 25000 | reuters | 14232 | 12.001 |
| 25000 | economic growth | 1094 | 0.314 |
| 25000 | ORG:fed | 354 | 0.008 |
| ----- | ------- | ----- | ----- |
| 10000 | reuters | 4968 | 1.476 |
| 10000 | economic growth | 489 | 0.071 |
| 10000 | ORG:fed | 239 | 0.004 |
| ----- | ------- | ----- | ----- |
| 1000 | reuters | 430 | 0.013 |
| 1000 | economic growth | 56 | 0.004 |
| 1000 | ORG:fed | 5 | 0.001|
| ----- | ------- | ----- | ----- |
| 100 | reuters | 40 | 0.0023 |
| 100 | economic growth | 10 | 0.0034 |
| 100 | ORG:fed | 1 | 0.0002 |
| ----- | ------- | ----- | ----- |
| 10 | reuters | 1 | 0.0003 |
| 10 | economic growth | 0 | 0 |
| 10 | ORG:fed | 0 | 0 |





