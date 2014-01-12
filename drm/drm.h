#ifndef DRM_DRM_H_
#define DRM_DRM_H_

#include <gmp.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>

#include "base/base.h"

// DRM (Binary splitting method) algorithm to reduce
//   1 /      C_0/      C_1/   /          C_{n-2}/       \   \\\      P
//  ---|B_0 + ---|B_1 + ---|...|B_{n-2} + -------|B_{n-1}|...||| ==> ---
//  A_0\      A_1\      A_2\   \          A_{n-1}\       /   ///      Q
// where A_k, B_k, C_k are set by SetA(k), SetB(k), SetC(k).
class Drm {
 public:
  Drm(int64 x, int64 n) : x_(x), n_(n) {}
  void Compute(mpz_t p, mpz_t q);
  void Core(int64 low, int64 up, mpz_t a, mpz_t b, mpz_t c);

 private:
  void SetA(int64 k, mpz_t a);
  void SetB(int64 k, mpz_t b);
  void SetC(int64 k, mpz_t c);

  int64 x_;
  int64 n_;
};

class Arctan {
 public:
  static void ComputePi(mpf_t pi, int64 n);
  struct Term {
    int64 coef;
    int64 quatient;
  };
};

#endif  // DRM_DRM_H_
