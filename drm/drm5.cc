#include "drm5.h"

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <algorithm>
#include <cassert>
#include <iostream>

#include "base/base.h"
#include "base/prime.h"
#include "number/integer.h"

namespace {
int64 GetSmooth(const int64 n) {
  std::vector<int64> smooths;
  const int64 lim = n * 2;
  for (int64 p2 = 1; p2 < lim; p2 *= 2)
    smooths.push_back(p2);

  std::sort(smooths.begin(), smooths.end());
  return *std::lower_bound(smooths.begin(), smooths.end(), n);
}
}  // namespace

Drm5::Drm5(int64 x, int64 digits) : Drm(x, digits), m_(GetSmooth(n_)) {
  std::vector<int> factors;
  for (int64 m = m_; m > 1;) {
    if (m % 2 == 0) {
      factors.push_back(2);
      m /= 2;
      continue;
    }
  }
  
  gcd_.resize(factors.size());
  for (size_t i = 0; i < factors.size(); ++i)
    gcd_[i].SetValue(1);

  Prime primes(m_);
  primes.GetNextPrime();  // Ignore 2.
  for (int prime; (prime = primes.GetNextPrime()) > 0;) {
    for (int64 ppow = prime; ppow < m_; ppow *= prime) {
      for (int64 i = factors.size() - 1, n = 1; i >= 0; --i) {
        int64 rem = (n / ppow) % factors[i];
        if (rem) {
          Integer pk;
          Integer::Power(prime, rem, &pk);
          Integer::Mul(gcd_[i], pk, &gcd_[i]);
        }
        n *= factors[i];
      }
    }
  }
}

void Drm5::Compute(Integer* p, Integer* q) {
  Integer c;
  Core(0, m_, 0, q, p, &c);

  Integer val;
  val.SetValue(1);
  for (const Integer& gcd : gcd_)
    Integer::Mul(val, gcd, &val);
  Integer::Mul(*q, val, q);
  Integer::Mul(*p, x_, p);
}

void Drm5::Core(int64 k0, int64 width, int64 level,
                Integer* a0, Integer* b0, Integer* c0) {
  if (width == 1) {
    SetValues(k0, a0, b0, c0);
    return;
  }

  if (width % 2 == 0) {
    Core2(k0, width / 2, level, a0, b0, c0);
    return;
  }

  // Must not reach here.
  assert(false);
}

void Drm5::Core2(int64 k0, int64 width, int64 level,
                 Integer* a0, Integer* b0, Integer* c0) {
  Integer a1, b1, c1;

  int64 k1 = k0 + width;
  Core(k0, width, level + 1, a0, b0, c0);
  Core(k1, width, level + 1, &a1, &b1, &c1);

  Integer::Mul(a1, *b0, b0);  // b0 = a1 * b0
  // TODO(peria): Avoid repeating computing |xk|.
  Integer xk;
  Integer::Power(x_, width, &xk);
  Integer::Mul(xk, *b0, b0);  // b0 = b0 * x^width
  Integer::Mul(b1, *c0, &b1);  // b1 = b1 * c0
  Integer::Add(*b0, b1, b0);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  Integer::Mul(*a0, a1, a0);  // a0 = a0 * a1
  Integer::Mul(*c0, c1, c0);  // c0 = c0 * c1

  Integer::Div(*a0, gcd_[level], a0);
  Integer::Div(*c0, gcd_[level], c0);
}

void Drm5::SetValues(int64 k, Integer* a, Integer* b, Integer* c) {
  // A_k = 2k + 1
  a->SetValue(2 * k + 1);

  // B_k = 1
  b->SetValue(1);

  // C_k = -(2k+1)
  c->SetValue((k < n_) ? (-(2 * k + 1)) : 0);
}
