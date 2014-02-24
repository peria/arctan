#include "search/search.h"

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);

  const int64 pmax = 100;
  const int64 xmax = 200;
  Search search(pmax, xmax);
  search.Sieve();
  std::vector<Element> elements;
  search.Debug(&elements);

  int64 num_smooth = 0;
  for (const Element element : elements) {
    const int64 x = element.x;
    const int64 v = static_cast<int64>(element.value);
    // Skip smooth numbers
    if (v == x * x + 1) {
      ++num_smooth;
      continue;
    }
    if ((x * x + 1) % v == 0)
      continue;

    std::cerr << "Error:"
              << "\nx=" << x
              << "\n x^2+1=" << (x * x + 1)
              << "\n value=" << v << "=";
    for (const auto factor : element.factors)
      std::cerr << factor.first << "^" << factor.second << "*";
    std::cerr << "\n";
  }

  std::cout << "smooth rate: " << num_smooth << " / " << xmax
            << " for p <= " << pmax << "\n";

  return 0;
}
