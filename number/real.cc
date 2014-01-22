#include "number/real.h"

#include <gmp.h>
#include <cassert>

namespace {
int64 g_bit_precision = mpf_get_default_prec();
}  // namespace

Real::Real() {
  mpf_init(r_);
}

Real::Real(const Integer& n) {
  mpf_init(r_);
  mpf_set_z(r_, n.Mpz());
}

Real::~Real() {
  mpf_clear(r_);
}

void Real::SetValue(double r) {
  mpf_set_d(r_, r);
}

void Real::SetPrecision(int64 precision) {
  mpf_set_default_prec(precision);
}

void Real::Add(const Real& a, const Real& b, Real* c) {
  mpf_add(c->r_, a.r_, b.r_);
}

void Real::Div(const Real& a, const Real& b, Real* c) {
  mpf_div(c->r_, a.r_, b.r_);
}
