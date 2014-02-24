#include "search/search.h"

#include <algorithm>
#include <cassert>
#include <map>
#include <vector>

#include "base/modulo.h"
#include "base/prime.h"

Search::Search(int64 p_max, int64 x_max) : p_max_(p_max), x_max_(x_max) {
  elements_.resize(x_max_ + 1);
  for (int i = 0; i <= x_max_; ++i) {
    elements_[i].x = i;
    elements_[i].value = 1 + (i & 1);
  }

  Prime all_primes(p_max_);
  primes_.clear();
  for (int32 p; (p = all_primes.GetNextPrime()) > 0;) {
    if (p % 4 == 1)
      primes_.push_back(p);
  }
}

void Search::Sieve() {
  // Sieve using |primes_|.
  for (int prime : primes_) {
    int64 root = Modulo::SquareRoot(prime - 1, prime);
    for (int64 pk = prime; pk < x_max_; pk *= prime) {
      root %= pk;
      for (int64 x = root; x < x_max_; x += pk) {
        elements_[x].factors[prime] = elements_[x].factors[prime] + 1;
        elements_[x].value *= prime;
      }
      for (int64 x = pk - root; x < x_max_; x += pk) {
        elements_[x].factors[prime] = elements_[x].factors[prime] + 1;
        elements_[x].value *= prime;
      }

      int64 s = (root * root + 1) / pk;
      int64 t = prime - s * Modulo::Inverse(2 * root, prime) % prime;
      root += t * pk;
    }
  }

  for (int64 x = 1; x < x_max_; ++x) {
    int64 norm = x * x + 1;
    if (norm > elements_[x].value * p_max_) {
      // Set a sign to figure this element is not smooth.
      elements_[x].value = 0;
      continue;
    }

    int64 value = elements_[x].value;
    // If (x^2+1)/value is less than p_max, it should be smooth.
    if (norm > value) {
      elements_[x].factors[norm / value] = 1;
      elements_[x].value = norm;
      value = 1;
    }
    // Set signs of coefficients.
    for (auto factor : elements_[x].factors) {
      const int32 p = factor.first;
      if (x % p > p / 2)
        elements_[x].factors[p] = -elements_[x].factors[p];
    }
  }
}

void Search::FindFormulae(int num_terms, std::vector<Formula>* formulae) {
  assert(num_terms == 3);
  std::vector<int32> used_primes(2);
  // TODO(peria): Choose |num_terms| - 1 primes.
  //              Now |num_terms| is ignored, assuming it is 3.
  for (size_t i0 = 0; i0 < primes_.size(); ++i0) {
    used_primes[0] = primes_[i0];
    for (size_t i1 = i0 + 1; i1 < primes_.size(); ++i1) {
      used_primes[1] = primes_[i1];
      // Filter elements
      std::vector<Element*> usable_elements;
      for (size_t x = 1; x < elements_.size(); ++x) {
        if (IsUsable(elements_[x], used_primes, num_terms))
          usable_elements.push_back(&elements_[x]);
      }

      FindFormulaeCore(num_terms, usable_elements, used_primes, formulae);
    }
  }
}

void FindFormulaeCore(int num_terms,
                      const std::vector<Element*>& elements,
                      const std::vector<int32>& primes,
                      std::vector<Formula>* formulae) {
  Matrix matrix(num_terms, Row(num_terms - 1, 0));
  // TODO(peria): Choose |num_terms| primes.
  //              Now |num_terms| is ignored, assuming it is 3.
  for (size_t i0 = 0; i0 < elements.size(); ++i0) {
    matrix[0][0] = elements[i0]->factors[primes[0]];
    matrix[0][1] = elements[i0]->factors[primes[1]];
    for (size_t i1 = i0 + 1; i1 < elements.size(); ++i1) {
      matrix[1][0] = elements[i1]->factors[primes[0]];
      matrix[1][1] = elements[i1]->factors[primes[1]];
      for (size_t i2 = i1 + 1; i2 < elements.size(); ++i2) {
        matrix[2][0] = elements[i2]->factors[primes[0]];
        matrix[2][1] = elements[i2]->factors[primes[1]];
        // Compute coefficients
        std::vector<int32> coefficients;
        GetCoefficients(matrix, &coefficients);
        // Check k, coefficient of pi, is not 0.
      }
    }
  }
}

void Search::GetCoefficients(const Matrix& matrix,
                             std::vector<int32>* coeffs) {
  
}

void Search::Debug(std::vector<Element>* elements) {
  *elements = elements_;
}

bool Search::IsUsable(const Element& elem, const std::vector<int32>& primes,
                      int32 num_terms) {
  if (elem.value < 1)
    return false;
  if (elem.factors.size() >= static_cast<size_t>(num_terms))
    return false;
  for (auto factor : elem.factors) {
    if (std::find(primes.begin(), primes.end(), factor.first) == primes.end())
      return false;
  }
  return true;
}
