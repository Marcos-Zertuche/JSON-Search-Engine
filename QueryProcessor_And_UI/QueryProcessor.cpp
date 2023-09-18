#include "QueryProcessor.h"

QueryProcessor::QueryProcessor(const string &folder)
{

    clean.makeStopWordsMap();

    dp = DocumentParser(folder, clean);

    // setUp DSTrees in this class so we have access to it via pointers
    wordsLibrary = dp.words();
    personsLibrary = dp.persons();
    orgsLibrary = dp.organizations();
    uuid_paths_map = dp.id_path();
    uuids_totalWords = dp.uuid_totalWords();

    resultSize = 0;

}

void QueryProcessor::search(vector<string> &include, vector<string> &exclude, vector<string> &persons, vector<string> &orgs)
{

    // Check to see the

    search_include(include);
    search_ignore(exclude);
    search_persons(persons);
    search_orgs(orgs);
    getSearchResult();
    for (auto &x : include)
        orderResultsWord(x);

    for (auto &y : persons)
        orderResultsPerson(y);

    for (auto &z : orgs)
        orderResultsOrg(z);

    beginPrint();
}
void QueryProcessor::search_include(vector<string> &include)
{
    resultSize += include.size();
    for (auto &str : include)
    {
        if (!clean.stopWords(str) && str.length() > 1)
        {
            str = clean.stemWord(str); // Clean non stopwords

            if (wordsLibrary->find(str) != nullptr) // WE FOUND THE WORD
            {
                auto uuids_for_word = (wordsLibrary->find(str));

                // GET THE VECTOR AT THIS WORD
                for (auto &x : *uuids_for_word)
                {
                    results[x.first] += 1;
                }
            }
        }
    }
}
void QueryProcessor::search_ignore(vector<string> &exclude)
{
    for (auto &str : exclude)
    {
        if (!clean.stopWords(str) && str.length() > 1)
        {
            str = clean.stemWord(str);              // Clean non stopwords
            if (wordsLibrary->find(str) != nullptr) // WE FOUND THE WORD
            {
                auto uuids_for_word = (wordsLibrary->find(str)); // GET THE VECTOR AT THIS WORD
                for (auto &x : *uuids_for_word)
                {
                    results[x.first] -= 1;
                }
            }
        }
    }
}
void QueryProcessor::search_persons(vector<string> &persons)
{
    // "PERSON:"
    //  0123456
    resultSize += persons.size();
    for (auto &str : persons)
    {
        if (personsLibrary->find(str) != nullptr) // WE FOUND THE WORD
        {
            auto uuids_for_word = (personsLibrary->find(str)); // GET THE VECTOR AT THIS WORD
            for (auto &x : *uuids_for_word)
            {
                results[x] += 1;
            }
        }
    }
}
void QueryProcessor::search_orgs(vector<string> &orgs)
{
    // ORGANIZATION:reuters
    // 012345678901
    resultSize += orgs.size();
    for (auto &str : orgs)
    {
        if (orgsLibrary->find(str) != nullptr) // WE FOUND THE WORD
        {
            auto uuids_for_word = (orgsLibrary->find(str)); // GET THE VECTOR AT THIS WORD
            for (auto &x : *uuids_for_word)
            {
                results[x] += 1;
            }
        }
    }
}

void QueryProcessor::getSearchResult()
{

    // Check if UUID count does not match resultSize
    for (auto it = results.begin(); it != results.end();)
    {
        if (it->second != resultSize)
        {
            it = results.erase(it); // erase the key-value pair and get the new iterator
        }
        else
        {
            ++it; // move to the next element
        }
    }
}

void QueryProcessor::orderResultsWord(string &word)
{
    for (auto it = results.begin(); it != results.end();)
    {
        auto uuid = it->first; // uuid of current doc

        auto totWordsInDoc = uuids_totalWords->find(uuid)->second; // int of total word in uuid
        /// NEED TO MAKE DIFFERENT FUNCTIONS FOR EACH ONE
        auto wordVectorPairs = (wordsLibrary->find(word)); // vector of all uuids at that word with their frequencies

        int frequencyOfWord = 0;
        for (auto &pair : *wordVectorPairs)
        {

            if (pair.first == uuid) // iterate until i find the uuid i am looking for in the pairs
            {
                frequencyOfWord = pair.second; // set frequency to pair.second
            }
        }

        double freqPercentage = (1.0000 * frequencyOfWord) / (1.0000 * totWordsInDoc); // Get the frequency by dividing frequency of word by total words in that doc

        pair<string, double> tempPair = {uuid, freqPercentage};

        // ASK ADAM or Hahsler
        //  https://www.geeksforgeeks.org/lambda-expression-in-c/ || https://en.cppreference.com/w/cpp/language/lambda
        // Find the pair with the given UUID (if it exists)
        auto existingPair = find_if(uuids_frequencies.begin(), uuids_frequencies.end(),
                                    [&uuid](const pair<string, double> &pair)
                                    { return pair.first == uuid; });

        // If the UUID does not yet exist, add a new pair to the vector
        if (existingPair == uuids_frequencies.end())
        {
            uuids_frequencies.push_back(tempPair);
        }
        // If the UUID does exist, update the frequency percentage
        else
        {

            existingPair->second += freqPercentage;
        }

        it++;
    }

    // ORGANIZE VECTOR
    //  https://www.geeksforgeeks.org/sort-vector-of-pairs-in-ascending-order-in-c/
    sort(uuids_frequencies.begin(), uuids_frequencies.end(), sortbysec);
}
void QueryProcessor::orderResultsPerson(string &person)
{
    for (auto it = results.begin(); it != results.end();)
    {
        auto uuid = it->first; // uuid of current doc

        auto totWordsInDoc = uuids_totalWords->find(uuid)->second; // int of total word in uuid
        /// NEED TO MAKE DIFFERENT FUNCTIONS FOR EACH ONE

        int frequencyOfWord = totWordsInDoc;

        double freqPercentage = (1.0000 * frequencyOfWord) / (1.0000 * totWordsInDoc); // Get the frequency by dividing frequency of word by total words in that doc

        pair<string, double> tempPair = {uuid, freqPercentage};

        // ASK ADAM or Hahsler
        //  https://www.geeksforgeeks.org/lambda-expression-in-c/ || https://en.cppreference.com/w/cpp/language/lambda
        // Find the pair with the given UUID (if it exists)
        auto existingPair = find_if(uuids_frequencies.begin(), uuids_frequencies.end(),
                                    [&uuid](const pair<string, double> &pair)
                                    { return pair.first == uuid; });

        // If the UUID does not yet exist, add a new pair to the vector
        if (existingPair == uuids_frequencies.end())
        {
            uuids_frequencies.push_back(tempPair);
        }
        // If the UUID does exist, update the frequency percentage
        else
        {

            existingPair->second += freqPercentage;
        }

        it++;
    }

    // ORGANIZE VECTOR
    //  https://www.geeksforgeeks.org/sort-vector-of-pairs-in-ascending-order-in-c/
    sort(uuids_frequencies.begin(), uuids_frequencies.end(), sortbysec);
}
void QueryProcessor::orderResultsOrg(string &org)
{
    for (auto it = results.begin(); it != results.end();)
    {
        auto uuid = it->first; // uuid of current doc

        auto totWordsInDoc = uuids_totalWords->find(uuid)->second; // int of total word in uuid
        /// NEED TO MAKE DIFFERENT FUNCTIONS FOR EACH ONE
        auto wordVectorPairs = (orgsLibrary->find(org)); // vector of all uuids at that word with their frequencies

        int frequencyOfWord = totWordsInDoc;

        double freqPercentage = (1.0000 * frequencyOfWord) / (1.0000 * totWordsInDoc); // Get the frequency by dividing frequency of word by total words in that doc

        pair<string, double> tempPair = {uuid, freqPercentage};

        // ASK ADAM or Hahsler
        //  https://www.geeksforgeeks.org/lambda-expression-in-c/ || https://en.cppreference.com/w/cpp/language/lambda
        // Find the pair with the given UUID (if it exists)
        auto existingPair = find_if(uuids_frequencies.begin(), uuids_frequencies.end(),
                                    [&uuid](const pair<string, double> &pair)
                                    { return pair.first == uuid; });

        // If the UUID does not yet exist, add a new pair to the vector
        if (existingPair == uuids_frequencies.end())
        {
            uuids_frequencies.push_back(tempPair);
        }
        // If the UUID does exist, update the frequency percentage
        else
        {

            existingPair->second += freqPercentage;
        }

        it++;
    }

    // ORGANIZE VECTOR
    //  https://www.geeksforgeeks.org/sort-vector-of-pairs-in-ascending-order-in-c/
    sort(uuids_frequencies.begin(), uuids_frequencies.end(), sortbysec);
}

bool QueryProcessor::sortbysec(pair<string, double> &a, pair<string, double> &b)
{
    return (a.second > b.second);
}

void QueryProcessor::beginPrint()
{
    for (auto &files : uuids_frequencies)
    {
        if (resultsCounter < 15)
        {
            auto freq = files.second;
            auto fileLocation = uuid_paths_map->find(files.first);
            if (fileLocation != uuid_paths_map->end())
            {
                auto file = fileLocation->second;
                printSearchResults(file, freq);
                resultsCounter++;
            }
        }
    }
}

void QueryProcessor::printSearchResults(const string &fileName, double &frequency)
{

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

    // PRINTING OUT CONTENT
    //  Results:
    //  -->UUID
    cout << "\nUUID:" << string(d["uuid"].GetString()) << '\n'
         // -->TITLE
         << string(d["title"].GetString()) << '\n'

         // -->DATE :: TEXT
         << (string(d["published"].GetString())).substr(0, 10) << " :: " << (string(d["text"].GetString())).substr(0, 150) << '\n';

    // -->PERSONS
    auto personsArray = d["entities"]["persons"].GetArray();

    cout << "Persons: ";
    for (auto &p : personsArray)
    {
        cout << p["name"].GetString() << " || ";
    }
    cout << '\n';

    // -->ORGS
    cout << "Organizations: ";
    //---------------- Get Organizations Array -----------------
    auto orgsArray = d["entities"]["organizations"].GetArray();

    // Set tree from Orgs Array
    for (auto &o : orgsArray)
    {
        cout << o["name"].GetString() << " || ";
    }
    cout << "\n";
    if (frequency != 0)
        cout << "Relativity (by frequency percentage):: " << frequency << "\n\n";

    // -->ORGS

    input.close();
}

int QueryProcessor::getSize() {
    return resultsCounter;
}
void QueryProcessor::tester()
{
    vector<string> inc{"source", "million"};

    /*"Source"                                                "Million"
    0c76d9defc2af9b9fc2659a9ca3d77b546850438           ***      0c76d9defc2af9b9fc2659a9ca3d77b546850438
    d2c20cd187389fdc2875ee2e51127aa6bfed50c6           ***      d2c20cd187389fdc2875ee2e51127aa6bfed50c6
    4ad75456ae0bc42beeee2ff65fa58fa1cc503be2
    5293252386bb424828d17b0ac60299cb52a8ecb9
    */

    vector<string> exc{};

    /*"Source"                                                "Million"
    0c76d9defc2af9b9fc2659a9ca3d77b546850438                 0c76d9defc2af9b9fc2659a9ca3d77b546850438
    d2c20cd187389fdc2875ee2e51127aa6bfed50c6                 d2c20cd187389fdc2875ee2e51127aa6bfed50c6
    4ad75456ae0bc42beeee2ff65fa58fa1cc503be2        ***
    5293252386bb424828d17b0ac60299cb52a8ecb9        ***
    */

    // "PERSON:francesca"
    // "ORGANIZATION:reuters"
    // 0c76d9defc2af9b9fc2659a9ca3d77b546850438
    vector<string> persons{};
    vector<string> orgs{};

    search(inc, exc, persons, orgs);
}

void QueryProcessor::start()
{
    cout << "Welcome to our Search Engine. \n";

    while (true)
    {
        resultsCounter = 0;
        resultSize = 0;
        string usrInput;
        cout << "\nPlease enter search query: \n"
             << "To exclude a word from search, enter \" -{word} \" \n"
             << "To look for a person, enter \" PERSON:{name} \" \n"
             << "To look for a organization, enter \" ORG:{name} \" \n"
             << "To exit, enter \" QUIT \" \n";
        getline(cin, usrInput);
        stringstream ss(usrInput);
        string tempWord;
        vector<string> inputVect;

        // Seperates by Space
        while (getline(ss, tempWord, ' '))
        {
            inputVect.push_back(tempWord);
        }

        for (auto &word : inputVect)
        {
            if (word.size() == 4 && word == "QUIT")
            {
                return;
            }
            // pushes into exclude vector if it has the minus sign
            if (word.front() == '-')
            {
                excludes.push_back(word.substr(1));
            }
            else if (word.size() > 7 && word.find("PERSON:") == 0)
            {
                string name = word.substr(7);
                persons.push_back(name);
            }
            else if (word.size() > 4 && word.find("ORG:") == 0)
            {
                string org = word.substr(4);
                orgs.push_back(org);
            }

            else
            {
                includes.push_back(word);
            }
        }
        auto startSearch = chrono::high_resolution_clock::now();
        search(includes, excludes, persons, orgs);
        auto endSearch = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsedDocParse = endSearch - startSearch;
        cout << results.size() << " results found in " << elapsedDocParse.count() << " seconds\n";
        results.clear();
        includes.clear();
        excludes.clear();
        persons.clear();
        orgs.clear();
        uuids_frequencies.clear();
    }
}

void QueryProcessor::storeData()
{
    dp.storeData();
}
