#pragma once

#include <iostream>
#include <vector>

using namespace std;

class SP {
private:
    int vertices;
    vector< vector<int> > graph;
    vector<int> parent;
    vector<int> cost;

    bool RunBFS(int start, int end);
    bool IsInputValid(vector< pair<int,int> > edges);
    bool isPathVertexValid(int start, int end);

public:
    SP(int v = 0);
    int GetVertices() const;
    void PrintSP(int start, int end);
    void AddEdge(vector< pair<int,int> > edges);
    static void PrintErrorMessage(string em);

};
