#include "search/search.h"

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <iostream>
#include <vector>

DEFINE_int64(pmax, 100, "Upper limit of prime numbers to use in sieve.");
DEFINE_int64(xmax, 200, "Upper bound of x to sieve.");

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);

  Search search(FLAGS_pmax, FLAGS_xmax);
  search.Sieve();
  std::vector<Element> elements;
  search.Debug(&elements);

  int64 num_smooth = 0;
  int64 sub_smooth = 0;  // Count x's for (x^2+1/primes) < pmax^2.
  for (const Element element : elements) {
    const int64 x = element.x;
    const int64 v = static_cast<int64>(element.value);
    // Skip smooth numbers
    if (v == x * x + 1) {
      ++num_smooth;
      continue;
    }
    if ((x * x + 1) % v == 0) {
      if ((x * x + 1) / v < FLAGS_pmax * FLAGS_pmax)
        ++sub_smooth;
      continue;
    }

    std::cerr << "Error:"
              << "\nx=" << x
              << "\n x^2+1=" << (x * x + 1)
              << "\n value=" << v << "=";
    for (const auto factor : element.factors)
      std::cerr << factor.first << "^" << factor.second << "*";
    std::cerr << "\n";
  }

  std::cout << "smooth rate: " << num_smooth << " / "
            << sub_smooth << " / " << FLAGS_xmax
            << " for p <= " << FLAGS_pmax << "\n";

  return 0;
}
