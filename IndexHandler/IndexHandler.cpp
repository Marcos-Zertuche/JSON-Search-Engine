#include "IndexHandler.h"

IndexHandler::IndexHandler()
{
}

void IndexHandler::addToWordTree(string w, string uuid, int freq)
{

    // If we do not find word
    if (words.find(w) == nullptr)
    {
        words.insert(w);
    }

    auto ptr = words.find(w);

    pair<string, int> searchPair = {uuid, freq};

    ptr->insert(searchPair);

    // www.techiedelight.com/check-vector-contains-given-element-cpp/
}

void IndexHandler::addToPersonTree(string person, string uuid)
{
    if (persons.find(person) == nullptr)
    {
        persons.insert(person);
    }

    auto tempPTR = persons.find(person);

    tempPTR->insert(uuid);
}

void IndexHandler::addToOrgTree(string org, string uuid)
{
    if (organizations.find(org) == nullptr)
    {
        organizations.insert(org);
    }

    auto tempPTR = organizations.find(org);

    tempPTR->insert(uuid);
}

void IndexHandler::addToUuid_PathMap(string uuid, string filename)
{
    UUID_PATH.insert({uuid, filename});
}

void IndexHandler::addToUuid_totalWords(string uuid, int totalWords)
{
    uuid_totalWords.insert({uuid, totalWords});
}

DSTree<string, unordered_map<string, int>> *IndexHandler::getWordsTree()
{
    return &words;
}

DSTree<string, unordered_set<string>> *IndexHandler::getPersonsTree()
{
    return &persons;
}

DSTree<string, unordered_set<string>> *IndexHandler::getOrganizationsTree()
{
    return &organizations;
}

unordered_map<string, string> *IndexHandler::getUuidPathMap()
{
    return &UUID_PATH;
}

unordered_map<string, int> *IndexHandler::getUuid_totalWords()
{
    return &uuid_totalWords;
}

void IndexHandler::checkMemory()
{
    checkWordsMemory();
    checkPersonsMemory();
    checkOrgsMemory();
    checkUUID_PathsMemory();
    checkUUID_WordCountMemory();
}
void IndexHandler::checkWordsMemory()
{
    ifstream wordsMem;
    wordsMem.open("Memory/WordTree.txt");

    // Check if opened
    if (!wordsMem.is_open())
    {
        cerr << "Failed to open file: wordsMem\n";
    }

    // IGNORE FIRST LINE
    string junk;
    getline(wordsMem, junk);

    string line;

    string currentWord;
    string uuid;
    string frequency;
    char separator;

    // Word|UUID,Frequency|
    while (getline(wordsMem, line))
    {
        stringstream currLine(line);
        // AT FIRST LINE
        getline(currLine, currentWord, '|');

        string uuid_freqstr;
        getline(currLine, uuid_freqstr, '|');
        // CONTINUE THROUGH FIRST LINE UNTIL \n

        stringstream restOfLine(uuid_freqstr);

        while (getline(restOfLine, uuid, ','))
        {
            restOfLine >> frequency;
            addToWordTree(currentWord, uuid, stoi(frequency));
            restOfLine >> separator;
        }
    }
    wordsMem.close();
}
void IndexHandler::checkPersonsMemory()
{
    ifstream personsMem;
    personsMem.open("Memory/PersonTree.txt");

    // Check if opened
    if (!personsMem.is_open())
    {
        cerr << "Failed to open file: personsMem\n";
    }

    // IGNORE FIRST LINE
    string junk;
    getline(personsMem, junk);

    string line;

    string person;
    string uuid;


    // Person|UUID|
    while (getline(personsMem, line))
    {
        stringstream currLine(line);
        // AT FIRST LINE
        getline(currLine, person, '|');

        string uuidSTR;
        getline(currLine, uuidSTR);
        stringstream restOfLine(uuidSTR);
        // CONTINUE THROUGH FIRST LINE UNTIL \n

        while (getline(restOfLine, uuid, '|'))
        {
            addToPersonTree(person, uuid);
        }
    }
    personsMem.close();
}
void IndexHandler::checkOrgsMemory()
{
    ifstream orgMem;
    orgMem.open("Memory/OrganizationTree.txt");

    // Check if opened
    if (!orgMem.is_open())
    {
        cerr << "Failed to open file: orgMem\n";
    }

    // IGNORE FIRST LINE
    string junk;
    getline(orgMem, junk);

    string line;

    string org;
    string uuid;


    // PerOrganization son|UUID|
    while (getline(orgMem, line))
    {
        stringstream currLine(line);
        // AT FIRST LINE
        getline(currLine, org, '|');

        string uuidSTR;
        getline(currLine, uuidSTR);
        stringstream restOfLine(uuidSTR);
        // CONTINUE THROUGH FIRST LINE UNTIL \n

        while (getline(restOfLine, uuid, '|'))
        {
            addToOrgTree(org, uuid);
        }
    }
    orgMem.close();
}
void IndexHandler::checkUUID_PathsMemory()
{
    ifstream uuidPath("Memory/UUID_PathMap.txt");
    if (!uuidPath.is_open())
    {
        cerr << "Failed to open file." << endl;
    }

    // Ignore first line
    string junk;
    getline(uuidPath, junk);

    // Read UUID and path values
    string line;
    string uuid, path;
    while (getline(uuidPath, line))
    {
        stringstream ss(line);
        getline(ss, uuid, '|');
        getline(ss, path, '|');
        // Remove trailing space from path
        path = path.substr(0, path.length() );
        // Pass uuid and path values to function
        addToUuid_PathMap(uuid, path);
    }

    uuidPath.close();
}
void IndexHandler::checkUUID_WordCountMemory()
{
    ifstream uuidCount("Memory/UUID_WordCountMap.txt");
    if (!uuidCount.is_open())
    {
        cerr << "Failed to open file." << endl;
    }

    // Ignore first line
    string junk;
    getline(uuidCount, junk);

    // Read UUID and path values
    string line;
    string uuid, count;
    while (getline(uuidCount, line))
    {
        stringstream ss(line);
        getline(ss, uuid, '|');
        getline(ss, count, '|');
        // Remove trailing space from path
        // count = count.substr(0, count.length());
        // cout << uuid << "|" << count << '\n';
        // Pass uuid and path values to function
        addToUuid_totalWords(uuid, stoi(count));
    }
    uuidCount.close();
}

void IndexHandler::storeData()
{
    saveWordsTree();
    savePersonsTree();
    saveOrgsTree();
    saveUuidPathMap();
    saveUuidWordCountMap();
}
void IndexHandler::saveWordsTree()
{
    ofstream wordsFile;
    wordsFile.open("Memory/WordTree.txt");

    if (!wordsFile.is_open())
    {
        cerr << "Failed to open file: wordsFile\n";
    }
    wordsFile << "Word|UUID,Frequency| \n";
    for (auto &x : words)
    {
        wordsFile << x.key << "|";
        for (auto &uuid_freq : x.element)
        {
            wordsFile << uuid_freq.first << "," << uuid_freq.second << "|";
        }
        wordsFile << '\n';
    }
    wordsFile.close();
}
void IndexHandler::savePersonsTree()
{
    ofstream personsFile;
    personsFile.open("Memory/PersonTree.txt");

     if (!personsFile.is_open())
    {
        cerr << "Failed to open file: personsFile\n";
    }
    personsFile << "Person|UUID| \n";
    for (auto &x : persons)
    {
        personsFile << x.key << "|";
        for (auto &uuid : x.element)
        {
            personsFile << uuid << "|";
        }
        personsFile << '\n';
    }
    personsFile.close();
}
void IndexHandler::saveOrgsTree()
{
    ofstream orgFile;
    orgFile.open("Memory/OrganizationTree.txt");

    if (!orgFile.is_open())
    {
        cerr << "Failed to open file: orgFile\n";
    }

    orgFile << "Organization|UUID| \n";
    for (auto &x : organizations)
    {
        orgFile << x.key << "|";
        for (auto &uuid : x.element)
        {
            orgFile << uuid << "|";
        }
        orgFile << '\n';
    }
    orgFile.close();
}
void IndexHandler::saveUuidPathMap()
{
    ofstream uuidPathFile;
    uuidPathFile.open("Memory/UUID_PathMap.txt");

    if (!uuidPathFile.is_open())
    {
        cerr << "Failed to open file: uuidPathFile\n";
    }
    uuidPathFile << "UUID|PATH.json| \n";
    for (auto &x : UUID_PATH)
    {
        uuidPathFile << x.first << "|" << x.second << "|";

        uuidPathFile << '\n';
    }

    uuidPathFile.close();
}
void IndexHandler::saveUuidWordCountMap()
{
    ofstream uuidWordCountFile;
    uuidWordCountFile.open("Memory/UUID_WordCountMap.txt");

    if (!uuidWordCountFile.is_open())
    {
        cerr << "Failed to open file: uuidWordCountFile\n";
    }


    uuidWordCountFile << "UUID|WordCount| \n";
    for (auto &x : uuid_totalWords)
    {
        uuidWordCountFile << x.first << "|" << x.second << "|";

        uuidWordCountFile << '\n';
    }

    uuidWordCountFile.close();
}
