#include <iostream>
#include "./QueryProcessor_And_UI/QueryProcessor.h"
#include "./QueryProcessor_And_UI/QueryUI.h"

using namespace std;

int main()
{
    auto start = chrono::high_resolution_clock::now();
    
    QueryProcessor q1("sample_data");


    q1.tester();


    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time ::: " << elapsed.count() << " seconds\n";



    return 0;
}