// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2

#include <iostream>
#include <limits>
#include <string>
#include "SP.hpp"

using namespace std;

vector<string>ExtractEdgeValues(string str, string del){
	vector<string> edges;

	auto start = 0U;
	auto end = str.find(del);
	while (end != string::npos)
	{
		edges.push_back(str.substr(start, end - start));
		start = end + del.length();
		end = str.find(del, start);
	}

	edges.push_back(str.substr(start, end));
	return edges;
}

vector< pair<int,int> > GetEdges(string s){

  pair<int, int> edge;
  vector< pair<int,int> > result;

	try {
		vector<string> edge_list = ExtractEdgeValues(s, "{<");
		edge_list = ExtractEdgeValues(edge_list[1], ">}");
		edge_list = ExtractEdgeValues(edge_list[0], ">,<");

		for (unsigned int i = 0; i < edge_list.size(); i++) {
			vector<string> edgeVector = ExtractEdgeValues(edge_list[i], ",");
			int v1 = stoi(edgeVector[0]);
			int v2 = stoi(edgeVector[1]);
			edge.first = v1;
			// cout << edge.first;
			edge.second = v2;
			// cout << edge.second;

			result.push_back(edge);
		}

	}catch (...){
		std::cerr << "Error: Invalid Input" << '\n';
		result.clear();
  }
  return result;
}
void TakeInput(){
  char cmd;
  int vertices;
  int start;
  int end;
  string edge_string;
  SP* g = new SP(0);

  while (cin >> cmd) {
    if(cmd == 'V' || cmd == 'v'){
      cin >> vertices;
      cout << "V " << vertices << endl;
      if (vertices >= 0) {
          delete g;
          g = new SP(vertices);
      }
      else {
        SP::PrintErrorMessage("Error: Invalid vertex value");
      }
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

    }

    else if(cmd == 'E' || cmd == 'e'){

      cin >> edge_string;
      cout << "E " << edge_string << endl;
      g->AddEdge(GetEdges(edge_string));
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

    }
    else if(cmd == 'S' || cmd == 's'){
      cin >> start >> end;
      g->PrintSP(start, end);
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

    }

    else{
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      SP::PrintErrorMessage("Error: Invalid Command");

    }
  }
  delete g;
  return;
}

int main() {

    TakeInput();
    return 0;
}
