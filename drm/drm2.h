#ifndef PI_DRM_DRM2_H_
#define PI_DRM_DRM2_H_

#include <gmp.h>

#include "base/base.h"

class Integer;
class FactoredInteger;

// DRM (Binary splitting method) algorithm to reduce
//   1 /      C_0/      C_1/   /          C_{n-2}/       \   \\\      P
//  ---|B_0 + ---|B_1 + ---|...|B_{n-2} + -------|B_{n-1}|...||| ==> ---
//  A_0\      A_1\      A_2\   \          A_{n-1}\       /   ///      Q
// where A_k, B_k, C_k are set by SetValues(k).
//
// Drm2 class computes using FactoredInteger class, which describes a partially
// factored number.
class Drm2 {
 public:
  Drm2(int64 x, int64 n);
  void Compute(Integer* p, Integer* q);
  void Core(int64 low, int64 up, FactoredInteger* a, FactoredInteger* b,
            FactoredInteger* c);

 private:
  void SetValues(int64 k, FactoredInteger* a, FactoredInteger* b,
                 FactoredInteger* c);

  int64 x_;
  int64 n_;
};

#endif  // PI_DRM_DRM2_H_
