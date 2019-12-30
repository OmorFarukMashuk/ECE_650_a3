// Acknowledgement: https://www.geeksforgeeks.org/shortest-path-unweighted-graph/

#include <iostream>
#include <vector>
#include <list>
#include <climits>
#include "SP.hpp"

using namespace std;

SP::SP(int v):vertices(v), graph(v), parent(v), cost(v){
this->vertices = v;
}

void SP::PrintErrorMessage(string em){
  std::cerr << em << endl;
  return;
}

bool SP::IsInputValid(vector< pair<int,int> > edges) {
    for ( auto& e : edges) {
        // cout << this->vertices << endl;
        if (this->vertices <= e.first || this->vertices <= e.second || e.first < 0 || e.second < 0) {

          PrintErrorMessage("Error: vertex does not exist");
          return false;
        }
    }
    return true;
}

int SP::GetVertices() const {
    return this->vertices;
}

bool SP::RunBFS(int start, int end) {
    list<int> q;
    bool visited[this->vertices];

    int i = 0;
    while(i < this->vertices){
      visited[i] = false;
      this->cost[i] = INT_MAX;
      this->parent[i] = -1;
      i++;
    }

    visited[start] = true;
    this->cost[start] = 0;
    q.push_back(start);

    while (!q.empty()) {
        int n = q.front();
        q.pop_front();
        for (unsigned int i =0; i < this->graph[n].size(); i++) {
            if (visited[this->graph[n][i]] == false)
            {
                visited[this->graph[n][i]] = true;
                this->cost[this->graph[n][i]] = cost[n] + 1;
                this->parent[this->graph[n][i]] = n;
                q.push_back(this->graph[n][i]);

                if (this->graph[n][i] == end) {
                    return true;
                }
            }
        }
    }
    return false;
}

void SP::AddEdge(vector< pair<int,int> > edges) {
    if ( IsInputValid(edges) ) {
        for ( auto& e : edges) {
                this->graph[e.first].push_back(e.second);
                this->graph[e.second].push_back(e.first);
            }
        }
    return;
}

bool SP::isPathVertexValid(int start, int end){

  return start >= this->vertices || end >= this->vertices ? false : true;
}

void SP::PrintSP(int start, int end) {

  // if(!isPathVertexValid(start, end)){
  //   PrintErrorMessage("Error: invalid vertex specified");
  //   return;
  // }
  if (RunBFS(start, end) == false) {

    PrintErrorMessage("Error: there is no path between given vertices");
    return;
  }

  vector<int> sp;
  int current = end;
  sp.push_back(current);

  while (this->parent[current] != -1) {
      sp.push_back(this->parent[current]);
      current = this->parent[current];
  }

  int i = sp.size() -1;

  while(i > -1){
    cout << sp[i];
    if (i > 0){
      cout << "-";
    }
    i--;
  }
  cout << endl;
  return;
}
