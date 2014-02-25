#include "search/search.h"

#include <glog/logging.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <map>
#include <vector>

#include "base/base.h"
#include "base/combination.h"
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
      int64 t = (prime - s * Modulo::Inverse(2 * root, prime) % prime) % prime;
      root += t * pk;
    }
  }

  for (int64 x = 1; x < x_max_; ++x) {
    int64 norm = x * x + 1;
    if (norm > elements_[x].value * p_max_) {
      // Set a sign to figure this element is not smooth.
      LOG_IF(INFO, x == 239) << norm << " " << elements_[x].value << " "
                             << p_max_ << " "
                             << (elements_[x].value * p_max_);
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

namespace {
const double kPi4 = std::atan(1.0);
const double kEps = 1.0e-5;
}

void Search::FindFormulae(int num_terms, std::vector<Formula>* formulae) {
  const int num_primes = num_terms - 1;

  std::sort(primes_.begin(), primes_.end());
  do {
    std::vector<int32> usable_primes;
    for (int i = 0; i < num_primes; ++i)
      usable_primes.push_back(primes_[i]);

    std::vector<Element*> elements;
    for (size_t x = 1; x < elements_.size(); ++x) {
      if (IsUsable(elements_[x], usable_primes, num_primes))
        elements.push_back(&elements_[x]);
    }

    if (elements.size() < static_cast<size_t>(num_terms))
      continue;

    do {
      Matrix matrix(num_terms, Row(num_primes, 0));
      for (int i = 0; i < num_terms; ++i) {
        for (int j = 0; j < num_primes; ++j) {
          matrix[i][j] = elements[i]->factors[usable_primes[j]];
        }
      }

      // Compute coefficients
      std::vector<int32> coefficients;
      if (!GetCoefficients(matrix, &coefficients))
        continue;
        
      double sum = 0;
      for (int i = 0; i < num_terms; ++i)
        sum += coefficients[i] * std::atan(1.0 / elements[i]->x);
      sum /= kPi4;
      int k = std::round(sum);

      if (k == 0 || std::abs(k - sum) > kEps)
        continue;

      Formula formula;
      formula.k = k;
      formula.terms.resize(num_terms);
      for (int i = 0; i < num_terms; ++i) {
        formula.terms[i].coef = coefficients[i];
        formula.terms[i].quot = elements[i]->x;
      }
      formulae->push_back(formula);
    } while (std::next_combination(elements.begin(),
                                   elements.begin() + num_terms,
                                   elements.end()));
  } while (std::next_combination(primes_.begin(),
                                 primes_.begin() + num_primes,
                                 primes_.end()));
}

bool Search::GetCoefficients(const Matrix& matrix,
                             std::vector<int32>* coeffs) {
  int n = matrix.size();
  for (int i = 0; i < n; ++i) {
    Matrix square(matrix);
    square.erase(square.begin() + i);
    int32 d = Determ(square);
    if (d == 0)
      return false;
    coeffs->push_back((i % 2 == 0) ? d : -d);
  }
  return true;
}

int32 Search::Determ(Matrix& matrix) {
  const int n = matrix.size();

  int64 ret = 1;
  int64 mul = 1;
  for (int p = 0; p < n; ++p) {
    int i = p;
    for (i = p; i < n && matrix[i][p] == 0; ++i) {}
    if (i == n)
      return 0;
    if (i != p) {
      std::swap(matrix[i], matrix[p]);
      ret = -ret;
    }

    int32 piv = matrix[p][p];
    for (int j = i + 1; j < n; ++j) {
      if (matrix[j][p] == 0)
        continue;
      int32 mat = matrix[j][p];
      for (int k = p; k < n; ++k)
        matrix[j][k] = matrix[j][k] * piv - mat * matrix[p][k];
      mul *= piv;
    }
  }

  for (int i = 0; i < n; ++i)
    ret *= matrix[i][i];

  return ret / mul;
}

void Search::Debug(std::vector<Element>* elements) {
  *elements = elements_;
}

bool Search::IsUsable(const Element& elem, const std::vector<int32>& primes,
                      int32 num_primes) {
  LOG_IF(INFO, primes[0] == 13 && elem.x == 239) << "here";
  if (elem.value < 1)
    return false;
  LOG_IF(INFO, primes[0] == 13 && elem.x == 239) << "here";
  if (elem.factors.size() > static_cast<size_t>(num_primes))
    return false;
  LOG_IF(INFO, primes[0] == 13 && elem.x == 239) << "here";
  for (auto factor : elem.factors) {
    if (std::find(primes.begin(), primes.end(), factor.first) == primes.end())
      return false;
  }
  return true;
}
