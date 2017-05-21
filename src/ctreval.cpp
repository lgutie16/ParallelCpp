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
  WriteIn wiFil;  //si esto cambia todo cambia
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
        //cout << TubesReference[levelOneProcesses].child << endl;
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

  cout << TubesReference[0].in << endl;

  pthread_t levelOneThreads[levelOneProcesses];
  TubesReference[0].out = TubesReference[1].pipeOut[1];
  TubesReference[1].in = TubesReference[1].pipeIn[0];
  TubesReference[1].out = TubesReference[2].pipeOut[1];
  TubesReference[2].in = TubesReference[2].pipeIn[0];
  TubesReference[2].out = 1;
  cout << TubesReference[0].in << "::" << TubesReference[0].out << endl;
  cout << TubesReference[1].in << "::" << TubesReference[1].out << endl;
  cout << TubesReference[2].in << "::" << TubesReference[2].out << endl;

  pthread_create(&levelOneThreads[0], NULL, readWriteThread, &TubesReference[0]);
  pthread_create(&levelOneThreads[1], NULL, readWriteThread, &TubesReference[1]);
  pthread_create(&levelOneThreads[2], NULL, readWriteThread, &TubesReference[2]);

  void *ret;
  pthread_join(levelOneThreads[0], &ret);
  pthread_join(levelOneThreads[1], &ret);
  pthread_join(levelOneThreads[2], &ret);

  /*TubesReference[0].out = TubesReference[1].pipeOut[1];*/
 
 /* for (int i = 1; i < TubesReference.size()-1; ++i){
    TubesReference[i].in = TubesReference[i].pipeIn[0];
    if((i + 1) != TubesReference.size()-1){
      TubesReference[i].out = TubesReference[i+1].pipeOut[1];
    }else{
      TubesReference[TubesReference.size()-1].out = 1;
    }
  }*/

  //Con ese array creo los hilos que van a ejecutar a los ctrEval 
  //Aqué quedaría listo el primer nivel
       //Cuento el numero de procesos hijos a crear y creo un array de hilos
  /*pthread_t levelOneThreads[levelOneProcesses];
  for (int i = 0; i < TubesReference.size(); ++i){
    pthread_t Proccessthread;
    levelOneThreads[i] = Proccessthread;
  }

  for(int i = 0; i < TubesReference.size(); ++i){    
    cout << levelOneThreads[i] << endl;
    pthread_create(&levelOneThreads[i], NULL, readWriteThread, &TubesReference[i]);
  }*/


 /* void *ret;
  cout << TubesReference.size() << endl;
  for(int i=0; i< TubesReference.size(); ++i){
    cout << "Somerthin" << endl;
    pthread_join(levelOneThreads[i], &ret);
  }*/

   /* void *ret;
    pthread_join(levelOneThreads[0], &ret);
    pthread_join(levelOneThreads[1], &ret);*/

  /*int c;
  while ((c = std::cin.get()) != EOF) {
    c = ::tolower(c);
    std::cout << (char) c << (char) c;
  }*/

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
