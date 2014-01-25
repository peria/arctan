#include "number/factored_integer.h"

#include <algorithm>
#include <cassert>
#include <iostream>
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

void FactoredInteger::Normalize() {
  Iterator it = factors_.begin();

  // Prime factorization using primes up to |kMaxPrime|.
  for (int p = 2; p < kMaxPrime; ++p) {
    int ex = 0;
    for (; Integer::Remain(n_, p) == 0; Integer::Div(n_, p, &n_))
      ++ex;
    if (ex == 0)
      continue;

    // Add/append exponent of p.
    for (; it != factors_.end() && it->first < p; ++it) {}
    if (it == factors_.end()) {
      factors_.push_back(Factor(p, ex));
      it = factors_.end();
      continue;
    }

    if (it != factors_.end() && it->first == p)
      it->second += ex;
    else
      it = factors_.insert(it, Factor(p, ex));
    ++it;
  }
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
  n->CopyFrom(n_);
  for (const Factor factor : factors_) {
    Integer powers;
    Integer::Power(factor.first, factor.second, &powers);
    Integer::Mul(*n, powers, n);
  }
}

void FactoredInteger::Add(const FactoredInteger& a,
                          const FactoredInteger& b,
                          FactoredInteger* c) {
  FactoredInteger dst;

  // Compute GCD to store in factors_.
  ConstIterator ia = a.factors_.begin();
  ConstIterator ib = b.factors_.begin();
  Integer an(a.n_), bn(b.n_), powers;
  while (ia != a.factors_.end() && ib != b.factors_.end()) {
    if (ia->first < ib->first) {
      Integer::Power(ia->first, ia->second, &powers);
      Integer::Mul(an, powers, &an);
      ++ia;
      continue;
    }
    if (ib->first < ia->first) {
      Integer::Power(ib->first, ib->second, &powers);
      Integer::Mul(bn, powers, &bn);
      ++ib;
      continue;
    }

    // ia->first == ib->first
    int ex = std::min(ia->second, ib->second);
    dst.factors_.push_back(Factor(ia->first, ex));
    if (ia->second > ex) {
      Integer::Power(ia->first, ia->second - ex, &powers);
      Integer::Mul(an, powers, &an);
    }
    if (ib->second > ex) {
      Integer::Power(ib->first, ib->second - ex, &powers);
      Integer::Mul(bn, powers, &bn);
    }
    ++ia;
    ++ib;
  }
  // Compute an and bn with remained factors.
  for (; ia != a.factors_.end(); ++ia) {
    Integer::Power(ia->first, ia->second, &powers);
    Integer::Mul(an, powers, &an);
  }
  for (; ib != b.factors_.end(); ++ib) {
    Integer::Power(ib->first, ib->second, &powers);
    Integer::Mul(bn, powers, &bn);
  }

  Integer::Add(an, bn, &dst.n_);
  dst.Normalize();

  c->CopyFrom(dst);
}

void FactoredInteger::CopyFrom(const FactoredInteger& m) {
  n_.CopyFrom(m.n_);
  factors_ = m.factors_;
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

  ConstIterator ia = a.factors_.begin();
  ConstIterator ib = b.factors_.begin();
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
    ++ia;
    ++ib;
  }
  for (; ia != a.factors_.end(); ++ia)
    dst.factors_.push_back(*ia);
  for (; ib != b.factors_.end(); ++ib)
    dst.factors_.push_back(*ib);

  // TODO(peria): Remove this copy if |c| is independent from |a| and |b|.
  c->CopyFrom(dst);
}

void FactoredInteger::Mul(const FactoredInteger& a,
                          const int64 b,
                          FactoredInteger* c) {
  Integer::Mul(a.n_, b, &(c->n_));
  c->Normalize();
}
