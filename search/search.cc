#include "search/search.h"

#include <glog/logging.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <map>
#include <unordered_set>
#include <vector>

#include "base/base.h"
#include "base/combination.h"
#include "base/modulo.h"
#include "base/prime.h"

namespace {
const int32 kNumDigits = 10000;

int32 GcdOfAll(const std::vector<int32>& vs) {
  if (vs.empty())
    return 0;
  int32 gcd = std::abs(vs[0]);
  for (size_t i = 1; i < vs.size(); ++i) {
    int32 v = vs[i];
    if (v == 0)
      return 0;
    gcd = Modulo::Gcd(gcd, std::abs(v));
    if (gcd == 1)
      return 1;
  }
  return gcd;
}

}  // namespace

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
  std::vector<int32> usable_primes;
  // Sieve using |primes_|.
  for (int prime : primes_) {
    int64 root = Modulo::SquareRoot(prime - 1, prime);
    std::unordered_set<int64> xs;
    for (int64 pk = prime; root <= x_max_ || (pk - root <= x_max_);
         pk *= prime) {
      for (int64 x = root; x < x_max_; x += pk) {
        elements_[x].factors[prime] = elements_[x].factors[prime] + 1;
        elements_[x].value *= prime;
        xs.insert(x);
      }
      for (int64 x = pk - root; x < x_max_; x += pk) {
        elements_[x].factors[prime] = elements_[x].factors[prime] + 1;
        elements_[x].value *= prime;
        xs.insert(x);
      }

      int64 s = (root * root + 1) / pk;
      int64 t = (prime - s * Modulo::Inverse(2 * root, prime) % prime) % prime;
      root += t * pk;
      root %= (pk * prime);
    }
    if (xs.size() >= 2) {
      usable_primes.push_back(prime);
    } else if (xs.size() == 1) {
      elements_[*xs.begin()].value = 0;  // unusable x
    }
  }
  LOG(INFO) << "Reduce primes from " << primes_.size() << " to " << usable_primes.size() << ".";
  primes_ = usable_primes;

  for (int64 x = 1; x < x_max_; ++x) {
    if (elements_[x].value == 0)
      continue;

    if (x * x + 1 != elements_[x].value) {
      // Set a sign to figure this element is not smooth.
      elements_[x].value = 0;
      continue;
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

  std::vector<Element*> usable_elements;
  for (auto& elem : elements_) {
    if (elem.value > 1 && elem.factors.size() <= static_cast<size_t>(num_primes))
      usable_elements.push_back(&elem);
  }

  std::sort(primes_.begin(), primes_.end());
  do {
    std::vector<int32> usable_primes;
    for (int i = 0; i < num_primes; ++i)
      usable_primes.push_back(primes_[i]);

    std::vector<Element*> elements;
    for (auto* elem : usable_elements) {
      if (IsUsable(*elem, usable_primes))
        elements.push_back(elem);
    }

    if (elements.size() < static_cast<size_t>(num_terms))
      continue;

    do {
      Matrix matrix(num_terms, Row(num_primes, 0));
      for (int i = 0; i < num_terms; ++i) {
        const std::unordered_map<int, int>& factors = elements[i]->factors;
        for (int j = 0; j < num_primes; ++j) {
          const int32 p = usable_primes[j];
          auto itr = factors.find(p);
          matrix[i][j] = (itr == factors.end()) ? 0 : itr->second;
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

      if (k < 0) {
        k = -k;
        for (size_t i = 0; i < coefficients.size(); ++i)
          coefficients[i] = -coefficients[i];
      }

      int32 gcd = Modulo::Gcd(k, GcdOfAll(coefficients));
      if (gcd > 1) {
        for (int i = 0; i < num_terms; ++i)
          coefficients[i] /= gcd;
        k /= gcd;
      }

      Formula formula;
      formula.k = k;
      formula.n = 1;
      formula.terms.resize(num_terms);
      for (int i = 0; i < num_terms; ++i) {
        formula.terms[i].coef = coefficients[i];
        formula.terms[i].quot = elements[i]->x;
        formula.n += kNumDigits / 2 / std::log10(elements[i]->x);
      }
      formulae->push_back(formula);
    } while (std::next_combination(elements.begin(),
                                   elements.begin() + num_terms,
                                   elements.end()));
  } while (std::next_combination(primes_.begin(),
                                 primes_.begin() + num_primes,
                                 primes_.end()));

  std::sort(formulae->begin(), formulae->end(),
            [](const Formula& a, const Formula& b) -> bool {
              return a.n < b.n;
            });
  LOG(INFO) << "Found " << formulae->size() << " formulae";
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

namespace {

const int kUndefined = 0x7fffffff;

inline int32 BitToIndex(int32 b) {
  switch (b) {
    case 1 << 0: return 0;
    case 1 << 1: return 1;
    case 1 << 2: return 2;
    case 1 << 3: return 3;
    case 1 << 4: return 4;
    case 1 << 5: return 5;
    case 1 << 6: return 6;
    case 1 << 7: return 7;
  }
  return -1;
}

int32 DetRecursive(const Matrix& matrix, int32 row, int32 bit, std::vector<int32>& dp) {
  if (dp[bit] != kUndefined)
    return dp[bit];

  int32 det = 0;
  int32 sign = 1;
  for (int32 id = bit; id; id &= id - 1) {
    int32 b = id & (-id);
    det += sign * matrix[row][BitToIndex(b)] * DetRecursive(matrix, row + 1, bit ^ b, dp);
    sign = -sign;
  }
  dp[bit] = det;
  return det;
}

}  // namespace

int32 Search::Determ(const Matrix& matrix) const {
  const int n = matrix.size();

  std::vector<int32> dp(1 << n, kUndefined);
  for (int i = 0; i < n; ++i)
    dp[1 << i] = matrix[n - 1][i];
  return DetRecursive(matrix, 0, (1 << n) - 1, dp);
}

void Search::Debug(std::vector<Element>* elements) {
  *elements = elements_;
}

bool Search::IsUsable(const Element& elem,
                      const std::vector<int32>& primes) {
  for (auto factor : elem.factors) {
    auto itr = std::find(primes.begin(), primes.end(), factor.first);
    if (itr == primes.end())
      return false;
  }
  return true;
}
