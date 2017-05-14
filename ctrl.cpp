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
  //Passing string to vector
  //istringstream iss(childInfo);
  //copy(istream_iterator<string>(iss),
  //istream_iterator<string>(),
  //ostream_iterator<string>(cout, "\n"));
  
  /*vector<string> vec;
  istringstream iss(childInfo);
  copy(istream_iterator<string>(iss),
  istream_iterator<string>(),
  back_inserter(vec));*/

  //Start tube creation
  WriteIn wiFil;  //si esto cambia todo cambia

  pid_t child;
  wiFil.child = child;

  pipe(wiFil.pipeIn);
  pipe(wiFil.pipeOut);

  if ((wiFil.child = fork()) == 0) {
    close(wiFil.pipeOut[1]);
    close(wiFil.pipeIn[0]);
    dup2(wiFil.pipeOut[0], 0);
    dup2(wiFil.pipeIn[1], 1);
    execl("./mayusculas", "mayusculas", NULL);
    return wiFil;
  }
  close(wiFil.pipeOut[0]);
  close(wiFil.pipeIn[1]);

  //Ends tube creation
  //cout << childInfo << vec[4] << endl;
  return wiFil;
}

int
main(void) {
  //Aquí leo el archivo
     //Creo las tuberias para cada proceso con una funcion -  createTube()
        //Vector para almacenar cada proceso con sus tuberias
  vector<WriteIn> TubesReference; 
  const char* file = "ctrsis.cfg";
  int levelOneProcesses = 0;
  string line;
  ifstream configFile(file);
  if(configFile.is_open()){
    while ( getline (configFile,line) ){
      WriteIn structure = createTube(line, levelOneProcesses);
      TubesReference.push_back(structure);     
      levelOneProcesses++;
    }
  }else{
    cout << "Unable to open file" << endl; 
  } 

  //Ya tengo los procesos guardados en un vector
  //Ahora asocio las entradas con las salidas de cada una de las tuberias
  //Según corresponda
  WriteIn mainTube;
  if ((mainTube.in = open("mayusculas.cpp", O_RDONLY)) == -1) {
    std::cerr << "Error open file" << std::endl;
    return 1;
  }
  for (int i = 0; i < TubesReference.size(); ++i){
    TubesReference[i].in = TubesReference[i].pipeIn[0];
    TubesReference[i].out = TubesReference[i+1].pipeOut[1];
  }
  //TubesReference[TubesReference.size()-1].out = 1;

  

  //Con ese array creo los hilos que van a ejecutar a los ctrEval 
  //Aqué quedaría listo el primer nivel
       //Cuento el numero de procesos hijos a crear y creo un array de hilos
  pthread_t levelOneThreads[levelOneProcesses];
  for (int i = 0; i < TubesReference.size() +1; ++i){
    pthread_t Proccessthread;
    levelOneThreads[i] = Proccessthread;
  }

  pthread_create(&levelOneThreads[0], NULL, readWriteThread, &mainTube);
  for (int i=1; i < TubesReference.size()+1; ++i){
    cout << "creating threads" <<  endl;
    pthread_create(&levelOneThreads[i], NULL, readWriteThread, &TubesReference[i-1]);
  }

  void *ret;
  pthread_join(levelOneThreads[0], &ret);
  pthread_join(levelOneThreads[1], &ret);

  return 0;
}

void* readWriteThread(void *arg) {
  WriteIn *dataInOut = (struct WriteIn *) arg;

  char c;
  while (read(dataInOut->in, &c, 1) > 0) {
    write(dataInOut->out, &c, 1);
  }
  close(dataInOut->in);
  close(dataInOut->out);
  return NULL;
}