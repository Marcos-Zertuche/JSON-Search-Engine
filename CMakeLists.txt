cmake_minimum_required(VERSION 3.15)
project(FinalProjExamples)


# filesystem.h require C++17
set(CMAKE_CXX_STANDARD 17)
set (input01 "sample_data")
set (input02 "Memory")
set (input03 "Stopwords")

file(COPY sample_data/ DESTINATION sample_data/)
file(COPY Memory/ DESTINATION Memory/)
file(COPY Stopwords/ DESTINATION Stopwords/)
# show compiler output and enable warnings
set(CMAKE_VERBOSE_MAKEFILE ON)
add_compile_options(-Wall -Wextra -pedantic)

#Test for document parser
add_executable(docParser_test test_DocParser.cpp DocumentParser/DocumentParser.cpp IndexHandler/IndexHandler.cpp CleanerClass/stemmer/porter2_stemmer.cpp catch2/catch.hpp)

#Test for entire system
# add_executable(query_processor_test Testers/test_system.cpp DocumentParser/DocumentParser.cpp IndexHandler/IndexHandler.cpp CleanerClass/stemmer/porter2_stemmer.cpp QueryProcessor_And_UI/QueryProcessor.cpp)

add_executable(rapidJSONExample rapidJSONExample.cpp)
add_executable(supersearch main.cpp DocumentParser/DocumentParser.cpp IndexHandler/IndexHandler.cpp CleanerClass/stemmer/porter2_stemmer.cpp QueryProcessor_And_UI/QueryProcessor.cpp)

# this makes sure we also link rapidjson
target_include_directories(rapidJSONExample PRIVATE rapidjson/)

#DSTree Tests
include(CTest)
# add_executable(tests_tree Testers/test_DSTree.cpp DSTree.h)
# add_test(NAME TestList COMMAND tests_tree)

add_executable(test_QP test_QP.cpp QueryProcessor_And_UI/QueryProcessor.cpp DocumentParser/DocumentParser.cpp IndexHandler/IndexHandler.cpp CleanerClass/stemmer/porter2_stemmer.cpp)
add_test(NAME TestList2 COMMAND test_QP)

set(CMAKE_VERBOSE_MAKEFILE OFF)
