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

  while ((c = cin.get()) != EOF) {
    c = ::tolower(c);    
    cout << "Pasa Algo XD" << endl;
    cout << (char) c << (char) c;
  }

  return 0;
}
