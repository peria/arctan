#include "number/integer.h"

#include <gmp.h>
#include <cassert>
#include "number/factored_integer.h"

Integer::Integer() {
  mpz_init(n_);
}

Integer::~Integer() {
  mpz_clear(n_);
}

void Integer::SetValue(int64 n) {
  mpz_set_si(n_, n);
}

void Integer::Add(const Integer& a, const Integer& b, Integer* c) {
  mpz_add(c->n_, a.n_, b.n_);
}

void Integer::Mul(const Integer& a, const Integer& b, Integer* c) {
  mpz_mul(c->n_, a.n_, b.n_);
}

void Integer::Mul(const Integer& a, const int64 b, Integer* c) {
  mpz_mul_si(c->n_, a.n_, b);
}

void Integer::Power(const int64 a, const int64 b, Integer* c) {
  mpz_ui_pow_ui(c->n_, a, b);
}
