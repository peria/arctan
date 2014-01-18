#include "number.h"

#include <gmp.h>
#include <vector>

Number_::Number_() {
  mpz_init(n);
}

Number_::~Number_() {
  mpz_clear(n);
}

void NumberToMpz(Number n, mpz_t m) {
  //m = n->n;
  //for (factors) {
  //  // compute power in O(log(b)).
  //  m *= factor.a ^ factor.b;
  //}
}

void NumberMul(Number a, Number b, Number c) {
}

void NumberAdd(Number a, Number b, Number c) {
}
