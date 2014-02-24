#include "search/search.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  Search search(40, 200);
  search.Sieve();
  std::vector<Element> elements;
  search.Debug(&elements);

  for (const Element element : elements) {
    const int64 x = element.x;
    const int64 v = static_cast<int64>(element.value);
    // Skip smooth numbers
    if (v == x * x + 1)
      continue;
    if ((x * x + 1) % v == 0)
      continue;

    std::cout << ((x * x + 1) / v) << " " << x
              << " " << (x * x + 1) << " " << v
              << " " << ((x * x + 1) % v) << ":";
    for (const auto factor : element.factors)
      std::cout << " " << factor.first << "^" << factor.second;
    std::cout << "\n";
  }

  return 0;
}
