#include "search/search.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  Search search(10, 200);
  search.Sieve();
  std::vector<Element> elements;
  search.Debug(&elements);

  for (const Element element : elements) {
    std::cout << element.x << " " << element.value;
    for (const auto factor : element.factors)
      std::cout << " " << factor.first << "^" << factor.second;
    std::cout << "\n";
  }

  return 0;
}
