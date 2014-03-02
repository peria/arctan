#include "drm4.h"

#include <glog/logging.h>

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cmath>

#include "base/base.h"
#include "base/prime.h"
#include "number/integer.h"

namespace {
const int32 kDivisionLadder = 10;
const int32 kDivision = 1 << kDivisionLadder;
Integer g_gcd[kDivisionLadder];  // g_gcd[i] is GCD of 2^i terms.
}

// static
void Drm4::Init() {
  for (int i = 0; i < kDivisionLadder; ++i)
    g_gcd[i].SetValue(1);

  Prime primes(kDivision);
  primes.GetNextPrime();  // Ignore 2.
  for (int prime; (prime = primes.GetNextPrime()) > 0;) {
    for (int64 ppow = prime; ppow < kDivision; ppow *= prime) {
      int64 n = 4;
      for (int i = 1; i < kDivisionLadder; ++i, n *= 2) {
        if ((n / ppow) % 2 == 1)
          Integer::Mul(g_gcd[i], prime, &g_gcd[i]);
      }
    }
  }
}

// static
void Drm4::CopyGcdForTest(std::vector<Integer>* gcd) {
  gcd->resize(kDivisionLadder);
  for (int i = 0; i < kDivisionLadder; ++i)
    (*gcd)[i].CopyFrom(g_gcd[i]);
}

Drm4::Drm4(int64 x, int64 digits)
  : Drm(x, digits),
    m_(n_ / kDivision + 1) {
  x2k_.clear();
  Integer xk;
  xk.SetValue(x_);
  for (int i = 0; i <= kDivisionLadder; ++i) {
    Integer::Mul(xk, xk, &xk);
    x2k_.push_back(xk);
  }
}

void Drm4::Compute(Integer* p, Integer* q) {
  Integer c;
  Core(0, m_, q, p, &c);

  Integer gcd;
  gcd.SetValue(1);
  for (int i = 0; i < kDivisionLadder; ++i)
    Integer::Mul(gcd, g_gcd[i], &gcd);
  Integer::Mul(*q, gcd, q);
  Integer::Mul(*p, x_, p);
}

void Drm4::Core(int64 low, int64 up, Integer* a0, Integer* b0, Integer* c0) {
  if (low == up - 1) {
    DivisionCore(low * kDivision, kDivision, kDivisionLadder - 1, a0, b0, c0);
    Integer::Mul(x2k_[kDivisionLadder], *a0, a0);
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

void Drm4::DivisionCore(int64 low, int64 width, int level,
                        Integer* a0, Integer* b0, Integer* c0) {
  if (width == 1) {
    assert(level == -1);
    SetValues(low, a0, b0, c0);
    return;
  }

  Integer a1, b1, c1;

  int64 mid = low + width / 2;
  DivisionCore(low, width / 2, level - 1, a0, b0, c0);
  DivisionCore(mid, width / 2, level - 1, &a1, &b1, &c1);

  Integer::Mul(a1, *b0, b0);  // b0 = a1 * b0
  Integer::Mul(x2k_[level], *b0, b0);  // b0 = b0 * x^(2^(level + 1))
  Integer::Mul(b1, *c0, &b1);  // b1 = b1 * c0
  Integer::Add(*b0, b1, b0);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  Integer::Mul(*a0, a1, a0);  // a0 = a0 * a1
  Integer::Mul(*c0, c1, c0);  // c0 = c0 * c1

  Integer::Div(*a0, g_gcd[level], a0);
  Integer::Div(*c0, g_gcd[level], c0);
}

void Drm4::SetValues(int64 k, Integer* a, Integer* b, Integer* c) {
  // A_k = 2k + 1
  a->SetValue(2 * k + 1);

  // B_k = 1
  b->SetValue(1);

  // C_k = -(2k+1)
  c->SetValue((k < n_) ? (-(2 * k + 1)) : 0);
}
