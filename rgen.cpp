// g++ -o rgen ../../../rgen.cpp ../../../helper.cpp -std=c++11

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "helper.hpp"

using namespace std;

void GenerateGraph(int svalue_int, int nvalue_int, int cvalue_int,int lvalue_int){
  RG test = RG(svalue_int, nvalue_int, cvalue_int);
  test.showAddGraph();
  sleep(randint(5,lvalue_int));
  // usleep(400000);
  test.showRemoveGraph();
  return;
}

int main (int argc, char **argv) {
    string svalue_st;
    // num of streets randint [2,k] k >=2
    int svalue_int = 10;
    string nvalue_st;
    //num of line segs per street rand int [1,k] k >= 1
    int nvalue_int = 5;
    string lvalue_st;
    //wait rand time rand int [5,k] k>=5
    int lvalue_int = 5;
    string cvalue_st;
    //xy coord range rand int [-k,k] k>=1
    int cvalue_int = 20;
    int c;

    opterr = 0;

    // expected options are '-s value', '-n value', '-l value', and '-c value'
    while ((c = getopt (argc, argv, "s:n:l:c:?")) != -1)

        if(c == 's'){
          svalue_st = optarg;
          svalue_int = atoi(svalue_st.c_str());
          if(svalue_int < 2) {
              std::cerr << "Error: Option -s has invalid arguments" << endl;
              return 1;
          }

        }
        else if(c == 'n'){
          nvalue_st = optarg;
          nvalue_int = atoi(nvalue_st.c_str());
          if(nvalue_int < 1) {
              std::cerr << "Error: Option -n has invalid arguments" << endl;
              return 1;
          }

        }
        else if(c == 'l'){
          lvalue_st = optarg;
          lvalue_int = atoi(lvalue_st.c_str());
          if(lvalue_int < 5) {
              std::cerr << "Error: Option -l has invalid arguments" << endl;
              return 1;
          }

        }
        else if(c == 'c'){
          cvalue_st = optarg;
          cvalue_int = atoi(cvalue_st.c_str());
          if(cvalue_int < 1) {
              std::cerr << "Error: Option -c has invalid arguments" << endl;
              return 1;
          }

        }
        else if(c == '?'){
          std::cerr << "Error: Invalid Choice: " << optopt << endl;
          return 1;

        }
        else{
          return 0;
        }

    while(1) {
        GenerateGraph(svalue_int, nvalue_int, cvalue_int,lvalue_int);
    }
    return 0;
}
