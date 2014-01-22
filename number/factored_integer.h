#ifndef DRM_NUMBER_H_
#define DRM_NUMBER_H_

#include <gmp.h>
#include <vector>
#include "base/base.h"

typedef std::pair<int, int> Factor;

// Number (Number_) class stores a number in following format.
//   n * \prod_i factor[i].first ^ factor[i].second
// where |n| is not smooth on factors.
struct Number_ {
 public:
  Number_();
  ~Number_();
  mpz_t n;
  std::vector<Factor> factors;
};

typedef Number_ Number[1];

// Converts a Number class to mpz_t.
void NumberToMpz(Number n, mpz_t m);

// Computes c = a + b.
void NumberAdd(Number a, Number b, Number c);

// Computes c = a * b.
void NumberMul(Number a, Number b, Number c);

#endif  // DRM_NUMBER_H_
