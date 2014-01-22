#include "number.h"

#include <gmp.h>
#include <cassert>
#include <vector>

Number_::Number_() {
  mpz_init(n);
}

Number_::~Number_() {
  mpz_clear(n);
}

void NumberToMpz(Number n, mpz_t m) {
  mpz_set(m, n->n);
  mpz_t tmp;
  mpz_init(tmp);
  for (const Factor& factor : n->factors) {
    mpz_ui_pow_ui(tmp, factor.first, factor.second);
    mpz_mul(m, m, tmp);
  }
  mpz_clear(tmp);
}

void NumberMul(Number a, Number b, Number c) {
  Number dst;
  mpz_mul(dst->n, a->n, b->n);

  auto ia = a->factors.begin();
  auto ib = b->factors.begin();
  while (ia != a->factors.end() && ib != b->factors.end()) {
    if (ia->first < ib->first) {
      dst->factors.push_back(*ia);
      ++ia;
      continue;
    }
    if (ia->first > ib->first) {
      dst->factors.push_back(*ib);
      ++ib;
      continue;
    }
    // ia->first == ib->first
    dst->factors.push_back(Factor(ia->first, ia->second + ib->second));
    ++ia;
    ++ib;
  }
  for (; ia != a->factors.end(); ++ia)
    dst->factors.push_back(*ia);
  for (; ib != b->factors.end(); ++ib)
    dst->factors.push_back(*ib);

  // c <- dst
  mpz_set(c->n, dst->n);
  c->factors = dst->factors;
}

void NumberAdd(Number a, Number b, Number c) {
  mpz_t an;
  mpz_t bn;
  mpz_init_set(an, a->n);
  mpz_init_set(bn, b->n);

  mpz_t tmp;
  mpz_init(tmp);
  Number dst;
  auto ia = a->factors.begin();
  auto ib = b->factors.begin();
  while (ia != a->factors.end() && ib != b->factors.end()) {
    if (ia->first < ib->first) {
      mpz_ui_pow_ui(tmp, ia->first, ia->second);
      mpz_mul(an, an, tmp);
      ++ia;
      continue;
    }
    if (ia->first > ib->first) {
      mpz_ui_pow_ui(tmp, ib->first, ib->second);
      mpz_mul(bn, bn, tmp);
      ++ib;
      continue;
    }

    // ia->first == ib->first
    int e = std::min(ia->second, ib->second);
    dst->factors.push_back(Factor(ia->first, e));
    if (e == ia->second) {
      mpz_ui_pow_ui(tmp, ib->first, ib->second - e);
      mpz_mul(bn, bn, tmp);
    } else {
      mpz_ui_pow_ui(tmp, ia->first, ia->second - e);
      mpz_mul(an, an, tmp);
    }
    ++ia;
    ++ib;
  }
  for (; ia != a->factors.end(); ++ia) {
    mpz_ui_pow_ui(tmp, ia->first, ia->second);
    mpz_mul(an, an, tmp);
  }
  for (; ib != b->factors.end(); ++ib) {
    mpz_ui_pow_ui(tmp, ib->first, ib->second);
    mpz_mul(bn, bn, tmp);
  }

  mpz_add(dst->n, an, bn);

  // Factored |n| into factors
  
  mpz_clears(tmp, an, bn, NULL);
}
