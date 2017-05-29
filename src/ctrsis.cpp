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
  /*vector<string> vec;
  istringstream iss(childInfo);
  copy(istream_iterator<string>(iss),
  istream_iterator<string>(),
  back_inserter(vec));
  //Envieronment variables level 1
  setenv("FICHEROCFG", vec[4].c_str(), 1);
  setenv("DIRDETRABAJO", vec[3].c_str(), 1);*/

  //verificar - innecessary
  string directory = "./evaluator"; //verificar esta parte(No sé si los controles tambien estan en ese directorio)
  const char *path = directory.c_str();
  //fin de verificar
  
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
    execl(path, "ctreval", NULL);
    return wiFil;
  }
  close(wiFil.pipeOut[0]);
  close(wiFil.pipeIn[1]);

  //Ends tube creation
  return wiFil;
}

int
main(void) {
  //Aquí leo el archivo
     //Creo las tuberias para cada proceso con una funcion -  createTube()
        //Vector para almacenar cada proceso con sus tuberias
  vector<WriteIn> TubesReference; 
  WriteIn mainTube;
  TubesReference.push_back(mainTube);


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
  configFile.close();

  //Ya tengo los procesos guardados en un vector
  //Ahora asocio las entradas con las salidas de cada una de las tuberias
  //Según corresponda

  
  if ((TubesReference[0].in = open("ctrsis.cfg", O_RDONLY)) == -1) {
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
 
  

  //Con ese array creo los hilos que van a ejecutar a los ctrEval 
  //Aqué quedaría listo el primer nivel
       //Cuento el numero de procesos hijos a crear y creo un array de hilos
  pthread_t levelOneThreads[levelOneProcesses];
  /*for (int i = 0; i < TubesReference.size(); ++i){
    // pthread_t Proccessthread;
    levelOneThreads[i] = new  pthread_t();
    cout << "Thread " << levelOneThreads[i] <<  endl;
  }*/

  cout << levelOneProcesses << endl;
  for(int i = 0; i < levelOneProcesses; ++i){
    cout << "HERE " << i << endl;
    pthread_t Proccessthread;
    int ret =  pthread_create(&Proccessthread,NULL, readWriteThread, &TubesReference[i]);
    if(ret != 0) {
      printf("Error: pthread_create() failed\n");
      exit(EXIT_FAILURE);
    }else{
      cout << "return: " << pthread_join(Proccessthread, NULL) << endl;
    }
    //cout << "Thread " << levelOneThreads[i] << " Tube " << TubesReference[i].child <<  endl;
    //pthread_create(&levelOneThreads[i], NULL, readWriteThread, &TubesReference[i]);
  }


  //string myarray [] = { "Control One { ./level1/son1 son1.cfg }", "Control Two { ./level1/son2 son2.cfg }" , "Control Three { ./level1/son3 son3.cfg }", "Control Three { ./level1/son3 son3.cfg }" };
  //Envieronment variables level 1
  /*void *ret;
  for(int i=0; i< TubesReference.size(); ++i){    
    pthread_join(levelOneThreads[i], NULL);//wait for thread i
    cout << i << ":: Evaluation finished" <<   endl;
  }*/
  return 0;
}

void* readWriteThread(void *arg) {
  sleep(3);
  /*WriteIn *dataInOut = (struct WriteIn *) arg;

  char c;
  while (read(dataInOut->in, &c, 1) > 0) {
    write(dataInOut->out, &c, 1);
  }
  close(dataInOut->in);
  close(dataInOut->out);*/
  

  //return NULL;
}