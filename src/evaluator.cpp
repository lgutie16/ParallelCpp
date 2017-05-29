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
#include <iostream>
#define EOF (-1)

using namespace std;

int
main(char* env[]) {
  int c;

  int i =0;
  char *configfile = getenv("PATH");
  char *configpath = getenv("DIRDETRABAJO"); 
  while (i < 10) {
    c = ::toupper(c);
    cout << i << "ALGO" << configfile << configpath << endl  ;
    i++;
  }
  return 0;
}
