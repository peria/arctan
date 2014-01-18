#ifndef DRM_NUMBER_H_
#define DRM_NUMBER_H_

#include <gmp.h>
#include <vector>
#include "base/base.h"

typedef std::pair<int, int> Factor;

struct Number_ {
 public:
  Number_();
  ~Number_();
  mpz_t n;
};

typedef Number_ Number[1];

void NumberToMpz(Number n, mpz_t m);

void NumberAdd(Number a, Number b, Number c);
void NumberMul(Number a, Number b, Number c);

#endif  // DRM_NUMBER_H_
