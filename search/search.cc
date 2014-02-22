#include "search/search.h"

#include <map>
#include <vector>

#include "base/modulo.h"

Search::Search(int64 p_max, int64 x_max)
  : p_max_(p_max), x_max_(x_max), primes_(p_max_) {
  elements_.resize(x_max_ + 1);
  for (int i = 0; i <= x_max_; ++i)
    elements_[i].x = i;
}


void Search::Sieve() {
  int prime;
  while ((prime = primes_.GetNextPrime()) > 0) {
    if (prime % 4 != 1)
      continue;
    int64 root = Modulo::SquareRoot(prime - 1, prime);
    for (int64 pk = prime; pk < x_max_; pk *= prime) {
      for (int64 x = root; x < x_max_; x += pk)
        elements_[x].factors[prime]++;
      for (int64 x = pk - root; x < x_max_; x += pk)
        elements_[x].factors[prime]++;
      // TODO(peria): update |root|.
    }
  }
}
