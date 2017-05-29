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
#define BUF_SIZE 10
struct WriteIn {
  int in;
  int out;
  int pipeIn[2];
  int pipeOut[2];
  pid_t child;
};

using namespace std;
void* readWriteThread(void *);

//It creates tubes and return them and after that 
//The tube is storage in a vector of type WriteIn
WriteIn createTube(string& childInfo, int& position) {
  vector<string> vec;
  istringstream iss(childInfo);
  copy(istream_iterator<string>(iss),
  istream_iterator<string>(),
  back_inserter(vec));
  //verificar - innecessary
  string directory = "./evaluator";
  const char *path = directory.c_str();

  
  //Start tube creation
  WriteIn wiFil;
  pipe(wiFil.pipeIn);
  pipe(wiFil.pipeOut);

  pid_t child;
  wiFil.child = child;


  if ((wiFil.child = fork()) == 0) {
    close(wiFil.pipeOut[1]);
    close(wiFil.pipeIn[0]);
    dup2(wiFil.pipeOut[0], 0);
    dup2(wiFil.pipeIn[1], 1);
    close(wiFil.pipeOut[0]);    
    close(wiFil.pipeIn[1]);
    execl(path, "evaluator", NULL);
    return wiFil;
  }
  close(wiFil.pipeOut[0]);
  close(wiFil.pipeIn[1]);

  //Ends tube creation
  return wiFil;
}

int
main(void) {
  //Capturo Variable de ambiente para buscar el config file que corresponda
  char *configfile = getenv("FICHEROCFG");
  char *configpath = getenv("DIRDETRABAJO");  

  int size = strlen(configfile) + strlen(configpath) + 1;
  char result[size];   // array to hold the result.
  strcpy(result,configpath); // copy string one into the result.
  strcat(result,"/"); // append string two to the result.
  strcat(result,configfile);

  vector<WriteIn> TubesReference; 
  const char* file = result;// Simulacion de captura de  variables de ambiente
  int levelOneProcesses = 0;
  string line;
  ifstream configFile(file);

  ////////////
  char ch[20], evaluator[20] = "Evaluador";
  if(configFile.is_open()){ 
    while(configFile){
      configFile>>ch;
      if(strcmp(ch,evaluator)==0){ 
        WriteIn structure = createTube(line, levelOneProcesses);
        TubesReference.push_back(structure);
        cout << TubesReference[levelOneProcesses].child << endl;
        levelOneProcesses++;    
      };
    }    
  }else{
    cout << "Unable to open file" << endl; 
  }

  /////////// Enlazan los tubos
  WriteIn mainTube;
  TubesReference.push_back(mainTube);
  if ((TubesReference[0].in = open("./level1/son1/son1.cfg", O_RDONLY)) == -1) {
    std::cerr << "Error open file" << std::endl;
    return 1;
  } //simulando la entrada que esta dada por la salida del proceso ctrsis*/


  
 
  return 0;
}

void* readWriteThread(void *arg) {
  WriteIn *dataInOut = (struct WriteIn *) arg;
  cout << "here" << endl;
  char c;
  while (read(dataInOut->in, &c, 1) > 0) {
    write(dataInOut->out, &c, 1);
  }
  close(dataInOut->in);
  close(dataInOut->out);
  return NULL;
}
