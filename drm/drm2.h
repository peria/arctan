#ifndef DRM_DRM2_H_
#define DRM_DRM2_H_

#include <gmp.h>

#include "base/base.h"
#include "drm/number.h"

// DRM (Binary splitting method) algorithm to reduce
//   1 /      C_0/      C_1/   /          C_{n-2}/       \   \\\      P
//  ---|B_0 + ---|B_1 + ---|...|B_{n-2} + -------|B_{n-1}|...||| ==> ---
//  A_0\      A_1\      A_2\   \          A_{n-1}\       /   ///      Q
// where A_k, B_k, C_k are set by SetA(k), SetB(k), SetC(k).
//
// Drm class computes using Number class, which describes a partially
// factored number.
class Drm {
 public:
  Drm(int64 x, int64 n) : x_(x), n_(n) {}
  void Compute(mpz_t p, mpz_t q);
  void Core(int64 low, int64 up, Number a, Number b, Number c);

 private:
  void SetA(int64 k, Number a);
  void SetB(int64 k, Number b);
  void SetC(int64 k, Number c);

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

#endif  // DRM_DRM2_H_
