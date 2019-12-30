// g++ ../../../rgen.cpp -o rgen -std=c++11

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "helper.hpp"

using namespace std;

template<class Trand>
Trand ReadRandom() {
    Trand random_value;
    size_t size = sizeof(random_value);
    ifstream urandom("/dev/urandom", ios::in|ios::binary);
    if(urandom)
    {
        urandom.read(reinterpret_cast<char*>(&random_value), size);
        if(!urandom)
        {
            cout << "Failed to read from /dev/urandom" << endl;
            exit(2);
        }
        urandom.close();
    }
    else
    {
        cout << "Failed to open /dev/urandom" << endl;
        exit(2);
    }
    return random_value;
}

int randint(int min, int max) {
    int range = max-min;
    int i = ReadRandom<int>();
    int result = (i % (range+1) + (range+1) ) % (range+1) + min;

    return result;
}

char genRandLetter() {
    char rand_char;
    int i;
    i = (ReadRandom<int>() % 26 + 26) % 26;
    if (randint(0,1)) {
        rand_char = 'A' + i;
    } else {
        rand_char = 'a' + i;
    }

    return rand_char;
}

string genRandWrd(int length=10) {
    string result;
    for(int i=0; i < length; i++) {
        result += genRandLetter();
    }
    return result;
}

bool isIntersectOccured(pair<int, int> p1, pair<int, int> p2, pair<int, int> p3, pair<int, int> p4) {
    int x1 = p1.first;
    int y1 = p1.second;
    int x2 = p2.first;
    int y2 = p2.second;
    int x3 = p3.first;
    int y3 = p3.second;
    int x4 = p4.first;
    int y4 = p4.second;
    float m1;
    float b1;
    float m2;
    float b2;


    int xdiffmax = max(min(x1, x2), min(x3, x4));
    int xdiffmin = min(max(x1, x2), max(x3, x4));
    int ydiffmax = max(min(y1, y2), min(y3, y4));
    int ydiffmin = min(max(y1, y2), max(y3, y4));

    // check for vertical overlapping lines
    if ( x1 == x2 && x2 == x3 && x3 == x4 ) {
        if ( (ydiffmin <= y1 && y1 <= ydiffmax) ||
             (ydiffmin <= y2 && y2 <= ydiffmax) ||
             (ydiffmin <= y3 && y3 <= ydiffmax) ||
             (ydiffmin <= y4 && y4 <= ydiffmax) )  {
            return true;
        }
    } else if ( x1 != x2 && x3 != x4) {
        m1 = (y2-y1)/(x2-x1);
        b1 = y1-m1*x1;
        m2 = (y4-y3)/(x4-x3);
        b2 = y3-m2*x3;
        // check if line equations are equal
        if (m1 == m2 && b1 == b2) {
            if (
             ( (xdiffmin <= x1 && x1 <= xdiffmax) ||
             (xdiffmin <= x2 && x2 <= xdiffmax) ||
             (xdiffmin <= x3 && x3 <= xdiffmax) ||
             (xdiffmin <= x4 && x4 <= xdiffmax) )
             &&
             ( (ydiffmin <= y1 && y1 <= ydiffmax) ||
             (ydiffmin <= y2 && y2 <= ydiffmax) ||
             (ydiffmin <= y3 && y3 <= ydiffmax) ||
             (ydiffmin <= y4 && y4 <= ydiffmax) )
                )
            {
            return true;
            }
        }
    }

    float xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4));
    float xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));

    float ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4);
    float yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    if (xden == 0 || yden == 0) {
        return false;
    }

    float xcrd = xnum / xden;
    float ycrd = ynum / yden;
    if ( xdiffmin <= xcrd && xcrd <= xdiffmax &&
        ydiffmin <= ycrd && ycrd <= ydiffmax ) {
            return true;
        }
    return false;
}

// Constructor
RG::RG(int svalue_int, int nvalue_int, int cvalue_int):
    svalue_int(svalue_int), nvalue_int(nvalue_int), cvalue_int(cvalue_int) {
        genRandGraph();
    }

//Private Functions
vector< pair<int,int> > RG::generateStrtSegments() {
    vector< pair<int,int> > street;
    int num_segments = randint(1, nvalue_int);
    int coord_range = cvalue_int;

    int attempts = 0;
    do {
        street.clear();
        for(int i = 0; i < num_segments + 1; i++ ) {
            pair<int, int> point;
            point.first = randint(-coord_range, coord_range);
            point.second = randint(-coord_range, coord_range);
            street.push_back(point);
        }
        attempts++;
        if (attempts > 25) {
            std::cerr << "Error: failed to generate valid input for " << attempts << " simultaneous attempts"
                      << endl;
        }
    } while (not isInputValid(street));
    return street;
}

void RG::genRandGraph() {
    int num_streets = randint(2, svalue_int);

    for(int i=0; i <  num_streets; i++) {
        string word = genRandWrd();
        //check if random word already exists and regenerate until unique
        while( find(strtnames.begin(), strtnames.end(), word) != strtnames.end() ) {
            word = genRandWrd();
        }
        strtnames.push_back(word);
        strtpoints.push_back(generateStrtSegments());
    }
}

bool RG::isInputValid(vector< pair<int,int> > street) {
    for (int i=0; i < street.size(); i++) {
        for (int j=i+1; j < street.size(); j++) {
            // Check for duplicate points
            if( (street[i].first == street[j].first) &&
                (street[i].second == street[j].second) )
            {
                return false;
            }
            // check if any two segments overlap or intersect
            if (i+1 < street.size() && j+1 < street.size()) {
                if(isIntersectOccured(street[i], street[i+1], street[j], street[j+1])) {
                    return false;
                }
            }
        }
    }
    return true;
}


void RG::showAddGraph() {
    string temp;
    for (int i=0; i < strtnames.size(); i++) {

        temp = temp + "a \"" + strtnames[i] + "\" ";
        for (auto& p : strtpoints[i]) {
            temp = temp + "(" + to_string(p.first) + "," + to_string(p.second) + ")";
        }
        temp += "\n";
    }
    temp += "g";
    cout << temp << endl;
    // cout << temp << endl;
}

void RG::showRemoveGraph() {
    string temp;
    for (auto& s_name : strtnames) {
        temp = temp + "r \"" + s_name + "\" ";
        temp += "\n";
        cout << "r \"" << s_name << "\"" << endl;
    }
}
