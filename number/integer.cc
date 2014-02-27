#include "number/integer.h"

#include <gmp.h>
#include <cassert>
#include "number/factored_integer.h"

Integer::Integer() {
  mpz_init(n_);
}

Integer::Integer(const Integer& n) {
  mpz_init_set(n_, n.n_);
}

Integer::Integer(const int64 n) {
  mpz_init_set_si(n_, n);
}

Integer::~Integer() {
  mpz_clear(n_);
}

void Integer::SetValue(int64 n) {
  mpz_set_si(n_, n);
}

void Integer::CopyFrom(const Integer& n) {
  mpz_set(n_, n.n_);
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

int64 Integer::Remain(const Integer& a, const int64 b) {
  return mpz_fdiv_ui(a.n_, b);
}

void Integer::Div(const Integer& a, const int64 b, Integer* c) {
  mpz_fdiv_q_ui(c->n_, a.n_, b);
}

void Integer::Div(const Integer& a, const Integer& b, Integer* c) {
  mpz_fdiv_q(c->n_, a.n_, b.n_);
}

void Integer::Power(const int64 a, const int64 b, Integer* c) {
  mpz_ui_pow_ui(c->n_, a, b);
}

void Integer::Print(const Integer& n) {
  mpz_out_str(stderr, 10, n.n_);
}
