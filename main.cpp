#include <iostream>
#include "include/allocator.h"

using namespace std;

int main() {
    char *data[1100];

    /*
    auto p = new char[16]; delete[] p;
    p = new char[32]; delete[] p;
    p = new char[64]; delete[] p;
    p = new char[128]; delete[] p;
     */

    auto p = new char[1025];

    delete [] p;

/*
    for (size_t i = 0; i < 1100; i++)
        data[i] = new char[1 + i];

    for (auto &i : data)
        delete[] i;
*/


    return 0;
}