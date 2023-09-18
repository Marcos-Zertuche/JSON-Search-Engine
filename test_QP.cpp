#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

// add includes
#include "./QueryProcessor_And_UI/QueryProcessor.h"

TEST_CASE("Check that Query Processor works", "[QP]")
{
    // Create a QueryProcessor object and run a search
    QueryProcessor qp("sample_data");
    vector<string> include = {"source"};
    vector<string> exclude = {};
    vector<string> persons = {};
    vector<string> orgs = {};
    qp.search(include, exclude, persons, orgs);
    REQUIRE(qp.getSize() == 4);

    //other test case. on exclude do -100000000
}

TEST_CASE("Check that Query exclude works", "[QP]")
{
    // Create a QueryProcessor object and run a search
    QueryProcessor qp("sample_data");
    vector<string> include = {"source"};
    vector<string> exclude = {"million"};
    vector<string> persons = {};
    vector<string> orgs = {};
    qp.search(include, exclude, persons, orgs);
    REQUIRE(qp.getSize() == 2);

}

TEST_CASE("Check that Query include two words works", "[QP]")
{
    // Create a QueryProcessor object and run a search
    QueryProcessor qp("sample_data");
    vector<string> include = {"source", "million"};
    vector<string> exclude = {};
    vector<string> persons = {};
    vector<string> orgs = {};
    qp.search(include, exclude, persons, orgs);
    REQUIRE(qp.getSize() == 2);

}

TEST_CASE("Check that Persons works", "[QP]")
{
    // Create a QueryProcessor object and run a search
    QueryProcessor qp("sample_data");
    vector<string> include = {};
    vector<string> exclude = {};
    vector<string> persons = {"francesca"};
    vector<string> orgs = {};
    qp.search(include, exclude, persons, orgs);
    REQUIRE(qp.getSize() == 1);

}

TEST_CASE("Check that orgs works", "[QP]")
{
    // Create a QueryProcessor object and run a search
    QueryProcessor qp("sample_data");
    vector<string> include = {};
    vector<string> exclude = {};
    vector<string> persons = {};
    vector<string> orgs = {"reuters"};
    qp.search(include, exclude, persons, orgs);
    REQUIRE(qp.getSize() == 4);

}


// you can run the test using Run CTest in the task bar or by running the tests executable.