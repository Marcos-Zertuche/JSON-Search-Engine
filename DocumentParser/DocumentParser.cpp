#include "DocumentParser.h"
//  Code from RapidJSONExample
void DocumentParser::readJsonFile(const string &fileName)
{
    total_documents++;

    // open an ifstream on the file of interest and check that it could be opened.
    ifstream input(fileName);
    if (!input.is_open())
    {
        cerr << "cannot open file: " << fileName << endl;
        return;
    }

    // Create a RapidJSON IStreamWrapper using the file input stream above.
    IStreamWrapper isw(input);

    // Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
    Document d;
    d.ParseStream(isw);

    // GET UUID
    auto uuid = string(d["uuid"].GetString());

    // ------------------ Get Persons Array ------------------
    auto personsArray = d["entities"]["persons"].GetArray();

    // Set tree from Persons Array
    for (auto &p : personsArray)
    {
        ih.addToPersonTree(p["name"].GetString(), uuid);
    }

    //---------------- Get Organizations Array -----------------
    auto orgsArray = d["entities"]["organizations"].GetArray();

    // Set tree from Orgs Array
    for (auto &o : orgsArray)
    {
        ih.addToOrgTree(o["name"].GetString(), uuid);
    }

    //---------------- Get File Text ----------------------
    auto txt = string(d["text"].GetString());

    stringstream currSS(txt);

    string tempWord;
    int totalWords{0};

    unordered_map<string, int> frequency_word;

    while (getline(currSS, tempWord, ' '))
    {
        // clean.cleanWord(tempWord);

        // Check if a stop word
        if (!clean.stopWords(tempWord) && tempWord.length() > 1)
        {
            tempWord = clean.stemWord(tempWord); // Clean non stopwords
            totalWords++;
            frequency_word[tempWord] += 1;

            // ih.addToWordTree(tempWord, uuid, 0);
        }

        input.close();
    }
    //
    for (auto &x : frequency_word)
    {

        pair<string, int> tempPair = {uuid, x.second};
        word_frequency_map[x.first].insert(tempPair);
    }

    //---------UUID_PATH MAP -----------
    ih.addToUuid_PathMap(uuid, fileName);

    //---------UUID_TOTALWORDS MAP --------
    ih.addToUuid_totalWords(uuid, totalWords);
}
//  Code from RapidJSONExample
/**
 * example code for how to traverse the filesystem using std::filesystem
 * which is new for C++17.
 *
 * @param path an absolute or relative path to a folder containing files
 * you want to parse.
 */
void DocumentParser::openFolder(const string &path)
{
    // recursive_director_iterator used to "access" folder at parameter -path-
    // we are using the recursive iterator so it will go into subfolders.
    // see: https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
    auto it = filesystem::recursive_directory_iterator(path);

    // loop over all the entries.
    for (const auto &entry : it)
    {

        // cout << "--- " << setw(60) << left << entry.path().c_str() << " ---" << endl;

        // We only want to attempt to parse files that end with .json...
        if (entry.is_regular_file() && entry.path().extension().string() == ".json")
        {
            readJsonFile(entry.path().string());
        }
    }
    // IN THIS FOR LOOP
    //  WORD FREQUENCY MAP HAS A KEY = WORD AND VALUE = VECTOR OF pair< uuid , words >
    // first loop goes through a word
    // then in that word, we loop through the vector and add each one of the words , uuids, and freqCount to words tree
    // using ih.addToWordTree
    for (auto &x : word_frequency_map)
    {
        for (auto &y : x.second)
        {
            ih.addToWordTree(x.first, y.first, y.second);
        }
    }
}

// Passing pointers to memory of DSTrees
DSTree<string, unordered_map<string, int>> *DocumentParser::words()
{
    return ih.getWordsTree();
}

DSTree<string, unordered_set<string>> *DocumentParser::persons()
{
    return ih.getPersonsTree();
}

DSTree<string, unordered_set<string>> *DocumentParser::organizations()
{
    return ih.getOrganizationsTree();
}

unordered_map<string, string> *DocumentParser::id_path()
{
    return ih.getUuidPathMap();
}

unordered_map<string, unordered_map<string, int>> *DocumentParser::wordFrequencies()
{
    return &word_frequency_map;
}

unordered_map<string, int> *DocumentParser::uuid_totalWords()
{
    return ih.getUuid_totalWords();
}
// USED TO SEE MAPS FOR DEBUGGING

void DocumentParser::testMaps()
{
    auto tempW = *ih.getWordsTree();
    auto tempP = *ih.getPersonsTree();
    auto tempO = *ih.getOrganizationsTree();
    auto tempUP = *ih.getUuidPathMap();

    for (auto it = tempW.begin(); it != tempW.end(); ++it)
    {
        cout << it->key << ":\n";
        for (auto s : it->element)
        {
            cout << s.first << " | " << s.second;
        }
        cout << "\n\n";
    }

    for (auto it = tempP.begin(); it != tempP.end(); ++it)
    {
        cout << it->key << ":\n";
        for (auto s : it->element)
        {
            cout << s << " | ";
        }
        cout << "\n\n";
    }

    for (auto it = tempO.begin(); it != tempO.end(); ++it)
    {
        cout << it->key << ":\n";
        for (auto s : it->element)
        {
            cout << s << " | ";
        }
        cout << "\n\n";
    }

    for (auto &it : tempUP)
    {
        cout << "UUID:" << it.first << " | Path:" << it.second << "\n";
    }
}

void DocumentParser::checkMemory()
{
    ih.checkMemory();
}

void DocumentParser::storeData()
{
    ih.storeData();
}
