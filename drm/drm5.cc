#include "drm5.h"

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <algorithm>
#include <cassert>
#include <map>

#include "base/base.h"
#include "base/prime.h"
#include "number/integer.h"

namespace {
int64 GetSmooth(const int64 n) {
  // There must be a 5-smooth number in [n, 2n).
  int64 ret = n * 2;

  int64 p2 = 0, p3 = 0, p5 = 0;
  for (p2 = 1; p2 < n; p2 *= 2) {
    for (p3 = p2; p3 < n; p3 *= 3) {
      for (p5 = p3; p5 < n; p5 *= 5) {}
      ret = std::min(ret, p5);
    }
    ret = std::min(ret, p3);
  }
  ret = std::min(ret, p2);

  return ret;
}
}  // namespace

Drm5::Drm5(int64 x, int64 digits) : Drm(x, digits), m_(GetSmooth(n_)) {
  std::vector<int> factors;
  static int kPrime[] = {5, 3, 2};
  for (int64 m = m_; m > 1;) {
    for (int p : kPrime) {
      if (m % p == 0) {
        factors.push_back(p);
        m /= p;
        break;
      }
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

  xk_.resize(factors.size());
  for (int64 i = factors.size() - 1, k = 2; i >= 0; --i) {
    Integer::Power(x_, k, &xk_[i]);
    k *= factors[i];
  }

  LOG(INFO) << "N: " << n_;
  LOG(INFO) << "M: " << m_;
}

void Drm5::Compute(Integer* p, Integer* q) {
  Integer c;
  Core(0, m_, 0, q, p, &c);

  Integer val;
  Integer::Power(x_, m_ * 2, &val);
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

  if (width % 5 == 0) {
    Core5(k0, width / 5, level, a0, b0, c0);
  } else if (width % 3 == 0) {
    Core3(k0, width / 3, level, a0, b0, c0);
  } else if (width % 2 == 0) {
    Core2(k0, width / 2, level, a0, b0, c0);
  } else {
    // Must not reach here.
    assert(false);
  }
}

void Drm5::Core2(int64 k0, int64 width, int64 level,
                 Integer* a0, Integer* b0, Integer* c0) {
  Integer a1, b1, c1;

  Core(k0, width, level + 1, a0, b0, c0);
  Core(k0 + width, width, level + 1, &a1, &b1, &c1);

  Integer::Mul(a1, *b0, b0);  // b0 = a1 * b0
  Integer::Mul(xk_[level], *b0, b0);  // b0 = b0 * x^width
  Integer::Mul(b1, *c0, &b1);  // b1 = b1 * c0
  Integer::Add(*b0, b1, b0);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  Integer::Mul(*a0, a1, a0);  // a0 = a0 * a1
  Integer::Mul(*c0, c1, c0);  // c0 = c0 * c1

  Integer::Div(*a0, gcd_[level], a0);
  Integer::Div(*c0, gcd_[level], c0);
}

void Drm5::Core3(int64 k0, int64 width, int64 level,
                 Integer* a0, Integer* b0, Integer* c0) {
  Integer a1, b1, c1;

  Core(k0, width, level + 1, a0, b0, c0);
  Core(k0 + width, width, level + 1, &a1, &b1, &c1);

  Integer::Mul(a1, *b0, b0);  // b0 = a1 * b0
  Integer::Mul(xk_[level], *b0, b0);  // b0 = b0 * x^width
  Integer::Mul(b1, *c0, &b1);  // b1 = b1 * c0
  Integer::Add(*b0, b1, b0);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  Integer::Mul(*a0, a1, a0);  // a0 = a0 * a1
  Integer::Mul(*c0, c1, c0);  // c0 = c0 * c1

  Core(k0 + width * 2, width, level + 1, &a1, &b1, &c1);
  Integer::Mul(a1, *b0, b0);  // b0 = a1 * b0
  Integer::Mul(xk_[level], *b0, b0);  // b0 = b0 * x^width
  Integer::Mul(b1, *c0, &b1);  // b1 = b1 * c0
  Integer::Add(*b0, b1, b0);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  Integer::Mul(*a0, a1, a0);  // a0 = a0 * a1
  Integer::Mul(*c0, c1, c0);  // c0 = c0 * c1

  Integer::Div(*a0, gcd_[level], a0);
  Integer::Div(*c0, gcd_[level], c0);
}

void Drm5::Core5(int64 k0, int64 width, int64 level,
                 Integer* a0, Integer* b0, Integer* c0) {
  Integer a1, b1, c1;
  Integer a2, b2, c2;
  Integer a3, b3, c3;
  Integer a4, b4, c4;

  Core(k0            , width, level + 1, a0, b0, c0);
  Core(k0 + width    , width, level + 1, &a1, &b1, &c1);
  Core(k0 + width * 2, width, level + 1, &a2, &b2, &c2);
  Core(k0 + width * 3, width, level + 1, &a3, &b3, &c3);
  Core(k0 + width * 4, width, level + 1, &a4, &b4, &c4);

  // Merge [0] and [1] to [0]
  Integer::Mul(a1, *b0, b0);  // b0 = a1 * b0
  Integer::Mul(xk_[level], *b0, b0);  // b0 = b0 * x^width
  Integer::Mul(b1, *c0, &b1);  // b1 = b1 * c0
  Integer::Add(*b0, b1, b0);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  Integer::Mul(*a0, a1, a0);  // a0 = a0 * a1
  Integer::Mul(*c0, c1, c0);  // c0 = c0 * c1

  // Merge [3] and [4] to [3]
  Integer::Mul(a4, b3, &b3);  // b3 = a4 * b3
  Integer::Mul(xk_[level], b3, &b3);  // b3 = b3 * x^width
  Integer::Mul(b4, c3, &b4);  // b4 = b4 * c3
  Integer::Add(b3, b4, &b3);  // b3 = b3 + b4 (= b3 * a4 + b4 * c3)
  Integer::Mul(a3, a4, &a3);  // a3 = a3 * a4
  Integer::Mul(c3, c4, &c3);  // c3 = c3 * c4

  // Merge [0]([0-1] in original) and [2] to [0]
  Integer::Mul(a2, *b0, b0);  // b0 = a2 * b0
  Integer::Mul(xk_[level], *b0, b0);  // b0 = b0 * x^width
  Integer::Mul(b2, *c0, &b2);  // b2 = b2 * c0
  Integer::Add(*b0, b2, b0);  // b0 = b0 + b2 (= b0 * a2 + b2 * c0)
  Integer::Mul(*a0, a2, a0);  // a0 = a0 * a2
  Integer::Mul(*c0, c2, c0);  // c0 = c0 * c2

  // Merge [0]([0-2] in original) and [3]([3-4] in original) to [0]
  Integer::Mul(a3, *b0, b0);  // b0 = a2 * b0
  {
    Integer x2k;
    Integer::Mul(xk_[level], xk_[level], &x2k);
    Integer::Mul(x2k, *b0, b0);  // b0 = b0 * x^(2*width)
  }
  Integer::Mul(b3, *c0, &b3);  // b2 = b1 * c0
  Integer::Add(*b0, b3, b0);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  Integer::Mul(*a0, a3, a0);  // a0 = a0 * a1
  Integer::Mul(*c0, c3, c0);  // c0 = c0 * c1

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
