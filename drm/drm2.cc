#include "drm2.h"

#include <gmp.h>

#include <iostream>

#include "base/base.h"

Number_::Number_() {
  mpz_init(n);
}

Number_::~Number_() {
  mpz_clear(n);
}

void Number::ToMpz(mpz_t m) {
  //m = n;
  //for (factors) {
  //  // compute power in O(log(b)).
  //  m *= factor.a ^ factor.b;
  //}
}

void Drm::Compute(mpz_t p, mpz_t q) {
  Number a, b, c;
  Core(0, n_, a, b, c);
  // a.ToMpz(q);
  // b.ToMpz(p);
}

void Drm::Core(int64 low, int64 up, Number a0, Number b0, Number c0) {
  if (low == up - 1) {
    SetA(low, a0);
    SetB(low, b0);
    SetC(low, c0);
    return;
  }

  mpz_t a1, b1, c1;
  mpz_inits(a1, b1, c1, NULL);

  int64 mid = (low + up) / 2;
  Core(low, mid, a0, b0, c0);
  Core(mid, up, a1, b1, c1);

  mpz_mul(b0, b0, a1);  // b0 = b0 * a1
  mpz_mul(b1, b1, c0);  // b1 = b1 * c0
  mpz_add(b0, b0, b1);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  mpz_mul(a0, a0, a1);  // a0 = a0 * a1
  mpz_mul(c0, c0, c1);  // c0 = c0 * c1

  mpz_clears(a1, b1, c1, NULL);
}

void Drm::SetA(int64 k, mpz_t a) {
  // A_k = (2 * k + 1) * x^2
  if (k == 0) {
    mpz_set_ui(a, x_);
  } else {
    mpz_set_ui(a, 2 * k + 1);
    mpz_mul_ui(a, a, x_ * x_);
  }
}

void Drm::SetB(int64 k, mpz_t b) {
  // B_k = 1
  mpz_set_ui(b, 1);
}

void Drm::SetC(int64 k, mpz_t c) {
  // C_k = -(2 * k + 1)
  mpz_set_si(c, - (2 * k + 1));
}

namespace {
const Arctan::Term terms[] = {{16, 5}, {-4, 239}};
}  // namespace

void Arctan::ComputePi(mpf_t pi, int64 n) {
  mpf_set_si(pi, 0);
  for (const Term& term : terms) {
    mpz_t ip, iq;
    mpz_inits(ip, iq, NULL);

    Drm drm(term.quatient, n);
    drm.Compute(ip, iq);
    mpz_mul_si(ip, ip, term.coef);

    mpf_t fp, fq;
    mpf_inits(fp, fq, NULL);
    mpf_set_z(fp, ip);  // fp = (Float)ip
    mpf_set_z(fq, iq);  // fq = (Float)iq
    mpz_clears(ip, iq, NULL);

    mpf_div(fp, fp, fq);  // fp = fp / fq
    mpf_add(pi, pi, fp);  // pi = pi + fp
    mpf_clears(fp, fq, NULL);
  }
}
