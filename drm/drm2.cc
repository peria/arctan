#include "drm/drm2.h"

#include <cmath>
#include <iostream>

#include "base/base.h"
#include "number/factored_integer.h"
#include "number/integer.h"

Drm2::Drm2(int64 x, int64 digits)
    : x_(x),
      n_(static_cast<int64>(digits / (2 * std::log10(x)))) {}

void Drm2::Compute(Integer* p, Integer* q) {
  FactoredInteger a, b, c;
  Core(0, n_, &a, &b, &c);
  a.ToInteger(q);
  b.ToInteger(p);
}

void Drm2::Core(int64 low, int64 up, FactoredInteger* a0, FactoredInteger* b0,
                FactoredInteger* c0) {
  if (low == up - 1) {
    SetValues(low, a0, b0, c0);
    return;
  }

  FactoredInteger a1, b1, c1;

  int64 mid = (low + up) / 2;
  Core(low, mid, a0, b0, c0);
  Core(mid, up, &a1, &b1, &c1);

  FactoredInteger::Mul(a1, *b0, b0);  // b0 = a1 * b0
  FactoredInteger::Mul(b1, *c0, &b1); // b1 = b1 * c0
  FactoredInteger::Add(*b0, b1, b0);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  FactoredInteger::Mul(*a0, a1, a0);  // a0 = a0 * a1
  FactoredInteger::Mul(*c0, c1, c0);  // c0 = c0 * c1
}

void Drm2::SetValues(int64 k, FactoredInteger* a, FactoredInteger* b,
                     FactoredInteger* c) {
  // A_k = (2 * k + 1) * x^2
  if (k == 0) {
    a->SetValue(x_);
  } else {
    a->SetValue(2 * k + 1);
    FactoredInteger::Mul(*a, x_ * x_, a);
  }

  // B_k = 1
  b->SetValue(1);

  // C_k = -(2 * k + 1)
  c->SetValue(-(2 * k + 1));
}
