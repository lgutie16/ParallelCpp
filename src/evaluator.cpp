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
main() {
  int c;

  int i =0;
  char *configfile = getenv("FICHEROCFG");
  char *configpath = getenv("DIRDETRABAJO");

  string line;
  //cout << getline(cin, line) << endl;
  while (i < 2) {
    cout << i << "ALGO" << configpath << configfile  << endl  ;
    i++;
  }

  /*for (std::string line; std::getline(std::cin, line);) {
    std::cout << line << std::endl;
  }*/

  /*string input_line;
  while(cin) {
    getline(cin, input_line);
    cout << input_line << endl;
  };*/

  /*while ((c = std::cin.get()) != EOF) {
    c = ::toupper(c);
    std::cout << (char) c;
  }*/
  return 0;
}
