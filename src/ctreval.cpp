#include <stdio.h>
#include <iostream>
#include <cctype>

#define EOF (-1)
int
main(void) {
  int c;

  while ((c = std::cin.get()) != EOF) {
    c = ::tolower(c);
    std::cout << (char) c << (char) c;
  }

  return 0;
}
