#pragma once

#include <iostream>
#include <vector>

using namespace std;

template<class Trand>
Trand ReadRandom();
int randint(int min, int max);
char genRandLetter();
string genRandWrd(int length);
bool isIntersectOccured(pair<int, int> p1, pair<int, int> p2, pair<int, int> p3, pair<int, int> p4);

class RG {
private:
    vector< vector< pair<int,int> > > strtpoints;
    vector< string > strtnames;
    int svalue_int, nvalue_int, cvalue_int;

    vector< pair<int,int> > generateStrtSegments();
    bool isInputValid(vector< pair<int,int> > street);
    void genRandGraph();

public:
    RG(int svalue_int, int nvalue_int, int cvalue_int);
    void showAddGraph();
    void showRemoveGraph();

};
