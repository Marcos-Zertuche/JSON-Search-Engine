#define CATCH_CONFIG_MAIN
#include "./catch2/catch.hpp"
#include "./DocumentParser/DocumentParser.h"

TEST_CASE("DocumentParser class tests", "[DocumentParser]")
{
    DocumentParser dp;
    DocumentParser d1;

    SECTION("Read JSON file")
    {
        // Test reading an existing JSON file
        dp.readJsonFile("sample_data/coll_1/news_0064567.json");
        REQUIRE(dp.words()->isEmpty() ==  true); //TRUE BECAUSE I ADD TO WORDS IN OPENFOLDER
        dp.testMaps();
        REQUIRE(dp.persons()->isEmpty() ==  false);
        REQUIRE(dp.organizations()->isEmpty() ==  false);
        REQUIRE(dp.id_path()->size() > 0);
        REQUIRE(dp.uuid_totalWords()->size() > 0);
    }
     SECTION("READ BAD JSON") 
     {
        // Test reading a non-existing JSON file
        d1.readJsonFile("nonexistingfile.json");
        REQUIRE(d1.words()->isEmpty() ==  true);
        REQUIRE(d1.persons()->isEmpty() ==  true);
        REQUIRE(d1.organizations()->isEmpty() ==  true);
        REQUIRE(d1.id_path()->size() == 0);
        REQUIRE(d1.uuid_totalWords()->size() == 0);
     }

    SECTION("Open folder")
    {
        // Test opening a folder containing JSON files
        dp.openFolder("sample_data");
        REQUIRE(dp.words()->isEmpty() ==  false);
        REQUIRE(dp.persons()->isEmpty() ==  false);
        REQUIRE(dp.organizations()->isEmpty() ==  false);
        REQUIRE(dp.id_path()->size() > 0);
        REQUIRE(dp.uuid_totalWords()->size() > 0);
    }

    SECTION("Get word frequencies")
    {
        // Test getting word frequencies for an existing word
        dp.readJsonFile("sample_data/coll_2/news_0064571.json");
        auto wordFreq = dp.wordFrequencies()->find("sourc");
        REQUIRE(wordFreq != dp.wordFrequencies()->end());
        REQUIRE(wordFreq->second.find("d2c20cd187389fdc2875ee2e51127aa6bfed50c6") != wordFreq->second.end());
        REQUIRE(wordFreq->second.find("d2c20cd187389fdc2875ee2e51127aa6bfed50c6")->second == 1);

        // Test getting word frequencies for a non-existing word
        wordFreq = dp.wordFrequencies()->find("fakewordthatwillneverbethere");
        REQUIRE(wordFreq == dp.wordFrequencies()->end());
    }
}