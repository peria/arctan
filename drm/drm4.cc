#include "drm4.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cmath>

#include "base/base.h"
#include "number/integer.h"

namespace {
const int kDivisionLadder = 3;
const int kDivision = 1 << kDivisionLadder;
// Odd primes up to kDivision.
const int kPrimes[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31};

// g_gcd[i] is GCD of 2^i terms.
Integer g_gcd[kDivisionLadder];
}

void Drm4::Init() {
  for (int i = 0; i < kDivisionLadder; ++i)
    g_gcd[i].SetValue(1);
  for (int prime : kPrimes) {
    for (int ppow = prime; ppow < kDivision; ppow *= prime) {
      for (int i = 1, n = 4; n <= kDivision; ++i, n <<= 1) {
        if ((n / ppow) % 2 == 1)
          Integer::Mul(g_gcd[i], prime, &g_gcd[i]);
      }
    }
  }
}

Drm4::Drm4(int64 x, int64 digits)
  : Drm(x, digits),
    m_(n_  / kDivision + 1) {}

void Drm4::Compute(Integer* p, Integer* q) {
  Integer c;
  Core(0, m_, q, p, &c);
}

void Drm4::Core(int64 low, int64 up, Integer* a0, Integer* b0, Integer* c0) {
  if (low == up - 1) {
    DivisionCore(low * kDivision, kDivision, kDivisionLadder - 1, a0, b0, c0);
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
  Integer::Mul(b1, *c0, &b1);  // b1 = b1 * c0
  Integer::Add(*b0, b1, b0);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  Integer::Mul(*a0, a1, a0);  // a0 = a0 * a1
  Integer::Mul(*c0, c1, c0);  // c0 = c0 * c1

  // Divide GCD.
  Integer::Div(*a0, g_gcd[level], a0);
  Integer::Div(*c0, g_gcd[level], c0);

#if 0
  // Debug output
  std::fprintf(stderr, "[%4lld-%4lld] ", low, width);
  Integer::Print(*a0);
  std::fprintf(stderr, " ");
  Integer::Print(*b0);
  std::fprintf(stderr, " ");
  Integer::Print(*c0);
  std::fprintf(stderr, "\n");
#endif
}

void Drm4::SetValues(int64 k, Integer* a, Integer* b, Integer* c) {
  // A_k = (2 * k + 1) * x^2
  if (k == 0) {
    a->SetValue(x_);
  } else {
    a->SetValue(2 * k + 1);
    Integer::Mul(*a, x_ * x_, a);
  }

  // B_k = 1
  b->SetValue(1);

  // C_k = -(2 * k + 1)
  c->SetValue((k < n_) ? (-(2 * k + 1)) : 0);
}
