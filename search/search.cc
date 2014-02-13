#include "search/search.h"

Search::Search(int64 p_max, int64 x_max)
  : p_max_(p_max), x_max_(x_max), primes_(p_max_) {
  elements_.resize(x_max_ + 1);
  for (int i = 0; i <= x_max_; ++i)
    elements_[i].x = i;
}

