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

using namespace std;

int
main(void) {
  int c;

  int i =0;
  while (i < 10) {
    c = ::toupper(c);    
    cout << i <<"(char) c" << endl  ;
    i++;
  }
  return 0;
}
