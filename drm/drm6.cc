#include "drm6.h"

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <algorithm>
#include <cassert>
#include <map>

#include "base/base.h"
#include "base/prime.h"
#include "number/integer.h"

namespace {

inline int BitCount(uint64 b) {
  static const uint64 kBit0 = 0x5555555555555555ULL;
  static const uint64 kBit1 = 0x3333333333333333ULL;
  static const uint64 kBit2 = 0x0f0f0f0f0f0f0f0fULL;
  b = (b & kBit0) + ((b >> 1) & kBit0);
  b = (b & kBit1) + ((b >> 2) & kBit1);
  b = (b & kBit2) + ((b >> 4) & kBit2);
  return (b * 0x0101010101010101ULL) >> (64 - 8);
}

// The number of steps in a perfect tournament.
const int kSteps = 8;

}  // namespace

Drm6::Drm6(int64 x, int64 digits) : Drm(x, digits) {
  int64 width = n_ >> kSteps;

  if (width == 0) {
    for (m_ = 2; m_ < n_; m_ *= 2) {}
  } else {
    for (int k = 1; k < 64; k *= 2)
      width |= width >> k;
    m_ = width + 1;
  }
  level_ = BitCount(m_ - 1);
  DCHECK(level_);

  gcd_.resize(level_ + 1);
  for (int i = 0; i <= level_; ++i)
    gcd_[i].SetValue(1);
  Prime primes(m_);
  primes.GetNextPrime();  // Ignore 2.
  for (int prime; (prime = primes.GetNextPrime()) > 0;) {
    for (int64 ppow = prime; ppow < m_; ppow *= prime) {
      for (int64 i = 1, n = 2; i <= level_; ++i, n *= 2) {
        if ((n / ppow) % 2)
          Integer::Mul(gcd_[i], prime, &gcd_[i]);
      }
    }
  }

  xk_.resize(level_ + 1);
  for (int64 i = 1, k = 4; i <= level_; ++i, k *= 2)
    Integer::Power(x_, k, &xk_[i]);

  LOG(INFO) << "N: " << n_;
  LOG(INFO) << "M: " << m_;
  LOG(INFO) << "Level: " << level_;
}

void Drm6::Compute(Integer* p, Integer* q) {
  Integer c;
  Core(0, (n_ + m_ - 1) / m_, q, p, &c);

  Integer gcd;
  gcd.SetValue(1);
  for (int i = 0; i <= level_; ++i)
    Integer::Mul(gcd, gcd_[i], &gcd);
  Integer::Mul(*q, gcd, q);
  Integer::Mul(*p, x_, p);
}

void Drm6::Core(int64 low, int64 up, Integer* a0, Integer* b0, Integer* c0) {
  if (low + 1 == up) {
    Core2(low * m_, m_, level_, a0, b0);
    c0->CopyFrom(*a0);
    Integer::Mul(*a0, xk_[level_], a0);
    return;
  }

  Integer a1, b1, c1;
  int64 mid = (low + up) / 2;
  Core(low, mid, a0, b0, c0);
  Core(mid, up, &a1, &b1, &c1);

  Integer::Mul(a1, *b0, b0);  // b0 = a1 * b0
  Integer::Mul(b1, *c0, &b1);  // b1 = b1 * c0
  Integer::Add(*b0, b1, b0);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  Integer::Mul(*a0, a1, a0);  // a0 = a0 * a1
  Integer::Mul(*c0, c1, c0);  // c0 = c0 * c1
}

void Drm6::Core2(int64 k0, int64 width, int64 level, Integer* a0, Integer* b0) {
  if (width == 2) {
    SetValues(k0, a0, b0);
    return;
  }

  width /= 2;
  Integer a1, b1;
  Core2(k0, width, level - 1, a0, b0);
  Core2(k0 + width, width, level - 1, &a1, &b1);

  Integer::Mul(a1, *b0, b0);  // b0 = a1 * b0
  Integer::Mul(xk_[level - 1], *b0, b0);  // b0 = b0 * x^width
  Integer::Mul(b1, *a0, &b1);  // b1 = b1 * c0
  Integer::Add(*b0, b1, b0);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  Integer::Mul(*a0, a1, a0);  // a0 = a0 * a1

  Integer::Div(*a0, gcd_[level], a0);
}

void Drm6::SetValues(int64 k, Integer* a, Integer* b) {
  // A_k = (2k+1) * (2k+3)
  // B_k = (2k+3)*x^2 - (2k+1)
  a->SetValue(-(2 * k + 1));
  b->SetValue(2 * k + 3);
  Integer::Mul(*b, x_ * x_, b);
  Integer::Add(*b, *a, b);
  Integer::Mul(*a, -(2 * k + 3), a);
}
