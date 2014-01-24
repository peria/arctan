#include "number/factored_integer.h"

#include <cassert>
#include <vector>

#include "base/base.h"
#include "number/integer.h"

namespace {
const int kMaxPrime = 100;
}

FactoredInteger::FactoredInteger() {}

FactoredInteger::~FactoredInteger() {}

void FactoredInteger::SetValue(int64 n) {
  factors_.clear();

  // Prime factorization using primes up to |kMaxPrime|.
  for (int p = 2; p < kMaxPrime && p * p <= n; ++p) {
    int ex = 0;
    for (; n % p == 0; n /= p)
      ++ex;
    if (ex)
      factors_.push_back(Factor(p, ex));
  }

  // Now |n| is not |kMaxPrime|-smooth.
  n_.SetValue(n);
}

Integer FactoredInteger::ToInteger() const {
  Integer n(n_);
  for (const Factor factor : factors_) {
    Integer powers;
    Integer::Power(factor.first, factor.second, &powers);
    Integer::Mul(n, powers, &n);
  }
  return n;
}

void FactoredInteger::ToInteger(Integer* n) {
  *n = n_;
  for (const Factor factor : factors_) {
    Integer powers;
    Integer::Power(factor.first, factor.second, &powers);
    Integer::Mul(*n, powers, n);
  }
}

void FactoredInteger::Add(const FactoredInteger& a,
         const FactoredInteger& b,
         FactoredInteger* c) {
  // TODO(peria): Impelement.
  assert(false);
}

void FactoredInteger::Add(const FactoredInteger& a,
                          const FactoredInteger& b,
                          Integer* c) {
  Integer::Add(a.ToInteger(), b.ToInteger(), c);
}

void FactoredInteger::Mul(const FactoredInteger& a,
                          const FactoredInteger& b,
                          FactoredInteger* c) {
  FactoredInteger dst;
  Integer::Mul(a.n_, b.n_, &(dst.n_));

  std::vector<Factor>::const_iterator ia = a.factors_.begin();
  std::vector<Factor>::const_iterator ib = b.factors_.begin();
  while (ia != a.factors_.end() && ib != b.factors_.end()) {
    if (ia->first < ib->first) {
      dst.factors_.push_back(*ia);
      ++ia;
      continue;
    }
    if (ia->first > ib->first) {
      dst.factors_.push_back(*ib);
      ++ib;
      continue;
    }
    // ia->first == ib->first
    dst.factors_.push_back(Factor(ia->first, ia->second + ib->second));
  }
  for (; ia != a.factors_.end(); ++ia)
    dst.factors_.push_back(*ia);
  for (; ib != b.factors_.end(); ++ib)
    dst.factors_.push_back(*ib);

  // TODO(peria): Remove this copy if |c| is independent from |a| and |b|.
  *c = dst;
}

void FactoredInteger::Mul(const FactoredInteger& a,
                          const int64 b,
                          FactoredInteger* c) {
  // TODO(peria): Impelement.
  assert(false);
}
