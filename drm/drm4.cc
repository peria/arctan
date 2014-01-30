#include "drm4.h"

#include <cassert>
#include <cstdio>
#include <cmath>

#include "base/base.h"
#include "number/integer.h"

namespace {
const int kDivisionLadder = 5;
const int kDivision = 1 << kDivisionLadder;
}

Drm4::Drm4(int64 x, int64 digits)
  : x_(x),
    n_(static_cast<int64>(digits / (2 * std::log10(x)))) {
}

void Drm4::Compute(Integer* p, Integer* q) {
  Integer c;
  Core(0, n_, q, p, &c);
}

void Drm4::Core(int64 low, int64 up, Integer* a0, Integer* b0, Integer* c0) {
  if (low == up - 1) {
    SetValues(low, a0, b0, c0);
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

void Drm4::DivisionCore(int64 low, int64 up, Integer* a0, Integer* b0,
                        Integer* c0) {
  if (low == up - 1) {
    SetValues(low, a0, b0, c0);
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

  // factorization
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
  c->SetValue(-(2 * k + 1));
}