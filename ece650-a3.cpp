#include<vector>

#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

using namespace std;

int getInput(void) {
    while (!cin.eof()) {
        string line;
        getline(cin, line);
        if (line.size () > 0)
            cout << line << endl;
    }
    return 0;
}

int Runa3(vector<pid_t> child, pid_t child_pid, int pipea1toa2[2], char **argv){
  child_pid = fork();
  if (child_pid == 0) {
      dup2(pipea1toa2[1], STDOUT_FILENO);
      close(pipea1toa2[0]);
      close(pipea1toa2[1]);

      return getInput();
  }
  else if (child_pid < 0) {
      std::cerr << "Error: a3 fork failed" << endl;
      return 1;
  }
  child.push_back(child_pid);

  int monitor;
  wait(&monitor);

  // send kill signal to all children
  for (pid_t k : child) {
      int status;
      kill (k, SIGTERM);
      waitpid(k, &status, 0);
      // std::cerr << "process status: " << status << endl;
  }

  return 0;
}

int Runa2(vector<pid_t> child, pid_t child_pid, int pipea1toa2[2],char **argv){
  child_pid = fork();
  if (child_pid == 0) {
      // redirect stdin from the pipe
      dup2(pipea1toa2[0], STDIN_FILENO);
      close(pipea1toa2[1]);
      close(pipea1toa2[0]);
      execv("ece650-a2", argv);
      perror ("Error: ece650-a2 execution failed");
      return 1;
  }
  else if (child_pid < 0) {
      std::cerr << "Error: a2 fork failed" << endl;
      return 1;
  }
  child.push_back(child_pid);

  Runa3(child, child_pid, pipea1toa2, argv);
  return 0;
}
int Runa1(vector<pid_t> child, pid_t child_pid, int pipe_rgentoa1[2], int pipea1toa2[2], char **argv){

  child_pid = fork();
  if (child_pid == 0) {
      // redirect stdin from the pipe
      dup2(pipe_rgentoa1[0], STDIN_FILENO);
      close(pipe_rgentoa1[1]);
      close(pipe_rgentoa1[0]);
      // redirect stdout from the pipe
      dup2(pipea1toa2[1], STDOUT_FILENO);
      close(pipea1toa2[0]);
      close(pipea1toa2[1]);
      execv("ece650-a1.py", argv);
      perror ("Error: Could not execute ece650-a1.py");
      return 1;
  }
  else if (child_pid < 0) {
      std::cerr << "Error: ece650-a1.py fork failed" << endl;
      return 1;
  }
  child.push_back(child_pid);

  Runa2(child, child_pid, pipea1toa2, argv);


  return 0;
}
int Runrgen(char **argv){

  vector<pid_t> child;
  pid_t child_pid;
  // create pipes
  int pipe_rgentoa1[2];
  pipe(pipe_rgentoa1);

  int pipea1toa2[2];
  pipe(pipea1toa2);

  child_pid = fork();
  if (child_pid == 0) {
      // redirect stdout to the pipe
      dup2(pipe_rgentoa1[1], STDOUT_FILENO);
      close(pipe_rgentoa1[0]);
      close(pipe_rgentoa1[1]);
      execv("rgen", argv);
      perror ("Error: rgen execution failed");
      return 1;
  }
  else if (child_pid < 0) {
      std::cerr << "Error: rgen fork failed" << endl;
      return 1;
  }
  child.push_back(child_pid);

  Runa1(child, child_pid, pipe_rgentoa1, pipea1toa2, argv);

  return 0;
}
int main (int argc, char **argv) {
    string svalue_st;
    int svalue_int = 10; // num of streets randint [2,k] k >=2
    string nvalue_st;
    int nvalue_int = 5; //num of line segs per street rand int [1,k] k >= 1
    string lvalue_st;
    int lvalue_int = 5; //wait rand time rand int [5,k] k>=5
    string cvalue_st;
    int cvalue_int = 20; //xy coord range rand int [-k,k] k>=1
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

    Runrgen(argv);

    return 0;
}
