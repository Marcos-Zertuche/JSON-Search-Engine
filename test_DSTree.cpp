#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

// add includes
#include "DSTree.h"

TEST_CASE("Check that DStree works", "[DStree]")
{
    DSTree<int, int> tree;

    SECTION("Test insert()") {
        tree.insert(5);
        REQUIRE(tree.contains(5));
        REQUIRE_FALSE(tree.contains(10));
        tree.insert(10);
        REQUIRE(tree.contains(10));
    }

    SECTION("Test remove()") {
        tree.insert(5);
        tree.insert(10);
        tree.remove(5);
        REQUIRE_FALSE(tree.contains(5));
        REQUIRE(tree.contains(10));
        tree.remove(10);
        REQUIRE_FALSE(tree.contains(10));
    }

    SECTION("Test copy constructor and assignment operator") {
        tree.insert(5);
        tree.insert(10);
        DSTree<int, int> tree2 = tree;

        REQUIRE(tree2.contains(5));
        REQUIRE(DSTree(tree2).contains(10));
        tree.insert(15);
        tree2 = tree;
        REQUIRE(tree2.contains(15));
    }

    SECTION("Test DSTree") {
        DSTree<std::string, std::vector<std::string>> words;
        std::string str = "tacos";
        words.insert(str);
        std::string stl = "good";
        words.insert(stl);
        words.find("tacos");


    }

}


// you can run the test using Run CTest in the task bar or by running the tests executable.
