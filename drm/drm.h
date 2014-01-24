#ifndef DRM_DRM_H_
#define DRM_DRM_H_

#include "base/base.h"
#include "number/integer.h"

// DRM (Binary splitting method) algorithm to reduce
//   1 /      C_0/      C_1/   /          C_{n-2}/       \   \\\      P
//  ---|B_0 + ---|B_1 + ---|...|B_{n-2} + -------|B_{n-1}|...||| ==> ---
//  A_0\      A_1\      A_2\   \          A_{n-1}\       /   ///      Q
// where A_k, B_k, C_k are set by SetA(k), SetB(k), SetC(k).
class Drm {
 public:
  Drm(int64 x, int64 n);
  void Compute(Integer* p, Integer* q);
  void Core(int64 low, int64 up, Integer* a, Integer* b, Integer* c);

 private:
  void SetValues(int64 k, Integer* a, Integer* b, Integer* c);

  int64 x_;
  int64 n_;
};

#endif  // DRM_DRM_H_
