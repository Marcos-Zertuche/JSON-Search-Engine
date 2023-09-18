#include <iostream>
#include "./QueryProcessor_And_UI/QueryProcessor.h"
using namespace std;

int main(int argc, char **argv)
{
    // READ FILES AND CREATE LIBRARY
   
    string data(argv[1]);

    cout << "Reading Documents and Creating Library... \n\n";
    QueryProcessor qp(data);

    
    // SEARCH
    qp.start();

    auto startSave = chrono::high_resolution_clock::now();
    // Store Data
    qp.storeData();
    auto endSave = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedSave = endSave - startSave;
    cout << "Elapsed time for saving library ::: " << elapsedSave.count() << " seconds\n";

    return 0;
}