#ifndef SEARCH_SEARCH_H_
#define SEARCH_SEARCH_H_

#include <vector>

#include "base/base.h"

class Factor;

// Searcher class searches pi-fomulae with arctangents in following steps.
// 1) Sieve x^2+1 for x < x_max_, using primes up to p_max_.
// 2) Find combinations of x's whose factors are common.
// 3) Generate pi-fomulae and check them.
class Search {
 public:
  Search(p_max, x_max);

 private:
  int64 p_max_;
  int64 x_max_;
};

struct Factor {
  int base;
  int exponent;
};

#endif  // SEARCH_SEARCH_H_
