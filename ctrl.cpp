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
  
  vector<string> vec;
  istringstream iss(childInfo);
  copy(istream_iterator<string>(iss),
  istream_iterator<string>(),
  back_inserter(vec));

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
    close(wiFil.pipeOut[0]);
    close(wiFil.pipeIn[1]);
    execl("/usr/bin/sort", "sort", NULL);
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
  const char* file = "ctrsis.cfg";
  int levelOneProcesses = 0;
  string line;
  ifstream configFile(file);
  if(configFile.is_open()){
    while ( getline (configFile,line) ){
      levelOneProcesses++;
    }
  }else{
    cout << "Unable to open file" << endl; 
  } 
  //Vector which defines name of struc
  vector<WriteIn> TubesReference[levelOneProcesses]; 

  //Cuento el numero de procesos hijos a crear y creo un array de hilos
  pthread_t levelOneThreads[levelOneProcesses];

  const char* fileTwo = "ctrsis.cfg";
  ifstream configFileTwo(fileTwo);
  if(configFileTwo.is_open()){
    while ( getline (configFileTwo,line) ){
      //TubesReference.push_back(createTube(line, levelOneProcesses));
      WriteIn structure = createTube(line, levelOneProcesses);
      cout << structure.child << endl;
      //Guardar a structute en un vector para asi poder manipular las tuberias de los diferentes procesos
    }
  }else{
    cout << "Unable to open file" << endl; 
  } 
  //Con ese array creo los hilos que van a ejecutar a los ctrEval 
  //Aqué quedaría listo el primer nivel
  WriteIn wiFil;

  wiFil.pipeIn[2];
  wiFil.pipeOut[2];

  int pipeInMay[2];
  int pipeOutMay[2];

  pipe(wiFil.pipeIn);
  pipe(wiFil.pipeOut);
  pipe(pipeInMay);
  pipe(pipeOutMay);

  pid_t sortChild;
  pid_t mayChild;

  if ((sortChild = fork()) == 0) {
    // Sort
    close(pipeInMay[0]);
    close(pipeInMay[1]);
    close(pipeOutMay[0]);
    close(pipeOutMay[1]);
    close(wiFil.pipeOut[1]);
    close(wiFil.pipeIn[0]);
    dup2(wiFil.pipeOut[0], 0);
    dup2(wiFil.pipeIn[1], 1);
    close(wiFil.pipeOut[0]);
    close(wiFil.pipeIn[1]);
    execl("/usr/bin/sort", "sort", NULL);
    return 1;
  }

  if ((mayChild = fork()) == 0) {
    // mayusculas
    close(wiFil.pipeIn[0]);
    close(wiFil.pipeIn[1]);
    close(wiFil.pipeOut[0]);
    close(wiFil.pipeOut[1]);
    close(pipeInMay[0]);
    close(pipeOutMay[1]);
    dup2(pipeOutMay[0], 0);
    dup2(pipeInMay[1], 1);
    close(pipeOutMay[0]);
    close(pipeInMay[1]);
    execl("./mayusculas", "mayusculas", NULL);
    return 1;
  }

  close(wiFil.pipeOut[0]);
  close(wiFil.pipeIn[1]);
  close(pipeOutMay[0]);
  close(pipeInMay[1]);

  WriteIn  wiSort, wiMay;

  //No me interesa es solo un control
  if ((wiFil.in = open("ctrl.cpp", O_RDONLY)) == -1) {
    cerr << "Error open file" << endl;
    return 1;
  }


  //Se asigna la salida de un proceso como la entrada de otro 
  //para continuar con el flujo de información en los diferentes
  //hijos ctrlEval y Evaluators
  wiFil.out = wiFil.pipeOut[1];
  wiSort.in = wiFil.pipeIn[0];
  wiSort.out = pipeOutMay[1];
  wiMay.in = pipeInMay[0];
  wiMay.out = 1;

  pthread_t threadFile, threadSort, threadMay;
  
  pthread_create(&threadFile, NULL, readWriteThread, &wiFil);
  pthread_create(&threadSort, NULL, readWriteThread, &wiSort);
  pthread_create(&threadMay,  NULL, readWriteThread, &wiMay);

  void *ret;
  
  pthread_join(threadFile, &ret);
  pthread_join(threadSort, &ret);
  pthread_join(threadMay, &ret);

  return 0;
}

void* readWriteThread(void *arg) {
  WriteIn *dataInOut = (struct WriteIn *) arg;

  char c;
  while (read(dataInOut->in, &c, 1) > 0) {
    // cout << "In: " << c << endl;
    write(dataInOut->out, &c, 1);
  }
  close(dataInOut->in);
  close(dataInOut->out);
  return NULL;
}