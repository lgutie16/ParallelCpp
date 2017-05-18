#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <stdlib.h>
#include <cstdlib>
#include <cctype>
#define EOF (-1)

struct WriteIn {
  int in;
  int out;
  int pipeIn[2];
  int pipeOut[2];
  pid_t child;
};

using namespace std;

//It creates tubes and return them and after that 
//The tube is storage in a vector of type WriteIn
WriteIn createTube(string& childInfo, int& position) {
  WriteIn wiFil;

  //Ends tube creation
  return wiFil;
}



int
main(void) {
  vector<WriteIn> TubesReference;
  
  char *configfile = getenv("FICHEROCFG");
  char *configpath = getenv("DIRDETRABAJO");
  const char* file = "./level1/son1/son1.cfg";
  
  int levelOneProcesses = 0;
  string line;
  ifstream configFile(file);

  if(configFile.is_open()){
    while ( getline (configFile,line) ){
      WriteIn structure = createTube(line, levelOneProcesses);
      TubesReference.push_back(structure);
      cout << TubesReference[levelOneProcesses].child << "Th";
      levelOneProcesses++;    
    }
  }else{
    cout << "Unable to open file" << endl; 
  } 

  /*int c;
  while ((c = std::cin.get()) != EOF) {
    c = ::tolower(c);
    std::cout << (char) c << (char) c;
  }*/

  return 0;
}
