#ifndef SEARCH_SEARCH_H_
#define SEARCH_SEARCH_H_

#include <vector>

#include "base/base.h"
#include "base/prime.h"

class Factor;

// Searcher class searches pi-fomulae with arctangents in following steps.
// 1) Sieve x^2+1 for x < x_max_, using primes up to p_max_.
// 2) Find combinations of x's whose factors are common.
// 3) Generate pi-fomulae and check them.
class Search {
 public:
  Search(int64 p_max, int64 x_max);

 private:
  std::vector<std::vector<Factor> > factors_;

  const int64 p_max_;
  const int64 x_max_;
  Prime primes_;
};

struct Factor {
  int base;
  int exponent;
};

#endif  // SEARCH_SEARCH_H_
