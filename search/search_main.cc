#include "search/search.h"

int main(int argc, char** argv) {
  Search search(100, 100000);
  search.Sieve();

  return 0;
}
