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


  WriteIn mainTube;
  TubesReference.push_back(mainTube);

  ////////////
  char ch[20], evaluator[20] = "Evaluador";
  if(configFile.is_open()){ 
    while(configFile){
      configFile>>ch;
      if(strcmp(ch,evaluator)==0){ 
        WriteIn structure = createTube(line, levelOneProcesses);
        TubesReference.push_back(structure);
        //cout << TubesReference[levelOneProcesses].child << endl;
        levelOneProcesses++;    
      };
    }    
  }else{
    cerr << "Unable to open file" << endl; 
  }

  //Passing the envieronment to children
  //In is emphy and after it is changer by the evaluator output

  /////////// Enlazan los tubos
  if ((TubesReference[0].in = open(file, O_RDONLY)) == -1) {
    std::cerr << "Error open file" << std::endl;
    return 1;
  } 
  TubesReference[0].out = TubesReference[1].pipeOut[1];
  cout << TubesReference[0].in << "::" << TubesReference[0].out << endl;
  for (int i = 1; i < levelOneProcesses; ++i){
    TubesReference[i].in = TubesReference[i].pipeIn[0];
    if(i != TubesReference.size()-1){
      TubesReference[i].out = TubesReference[i+1].pipeOut[1];
    }else{
      TubesReference[TubesReference.size()-1].out = 1;
    }
    cout << TubesReference[i].in << "::" << TubesReference[i].out << endl;
  }


  pthread_t levelOneThreads[levelOneProcesses];  
  for(int i = 0; i < levelOneProcesses; ++i){
    //putenv(configfile);
    //putenv(configpath);
    system( "./evaluator" );
    int ret =  pthread_create(&levelOneThreads[i],NULL, readWriteThread, &TubesReference[i]);
    if(ret != 0) {
      cerr << "Error: pthread_create() failed\n" << endl;
      exit(EXIT_FAILURE);
    }else{
      cout << "return: " << pthread_join(levelOneThreads[i], NULL) << endl;
    }
  }

  //cout << "Something" << file  << TubesReference[0].in << endl;
  return 0;
}

void* readWriteThread(void *arg) {
  sleep(3);
  WriteIn *dataInOut = (struct WriteIn *) arg;
  char c;
  while (read(dataInOut->in, &c, 1) > 0) {
    cout << c ;
  }
  close(dataInOut->in);
  close(dataInOut->out);
}
