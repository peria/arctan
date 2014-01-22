#include "drm.h"

#include <gmp.h>

#include <iostream>

#include "base/base.h"

void Drm::Compute(Integer* p, Integer* q) {
  Integer c;
  Core(0, n_, q, p, &c);
}

void Drm::Core(int64 low, int64 up, Integer* a0, Integer* b0, Integer* c0) {
  if (low == up - 1) {
    SetValues(low, a0, b0, c0);
    return;
  }

  Integer ta, tb, tc;
  Integer *a1 = &ta;
  Integer *b1 = &tb;
  Integer *c1 = &tc;

  int64 mid = (low + up) / 2;
  Core(low, mid, a0, b0, c0);
  Core(mid, up, a1, b1, c1);

  Integer::Mul(a1, b0, b0);  // b0 = a1 * b0
  Integer::Mul(b1, c0, b1);  // b1 = b1 * c0
  Integer::Add(b0, b1, b0);  // b0 = b0 + b1 (= b0 * a1 + b1 * c0)
  Integer::Mul(a0, a1, a0);  // a0 = a0 * a1
  Integer::Mul(c0, c1, c0);  // c0 = c0 * c1
}

void Drm::SetValues(int64 k, Integer* a, Integer* b, Integer* c) {
  // A_k = (2 * k + 1) * x^2
  if (k == 0) {
    mpz_set_ui(a->n, x_);
  } else {
    mpz_set_ui(a->n, 2 * k + 1);
    mpz_mul_ui(a->n, a->n, x_ * x_);
  }

  // B_k = 1
  mpz_set_ui(b->n, 1);

  // C_k = -(2 * k + 1)
  mpz_set_si(c->n, - (2 * k + 1));
}

namespace {
const Arctan::Term terms[] = {{16, 5}, {-4, 239}};
}  // namespace

void ComputePi(int64 n, Real* pi) {
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
