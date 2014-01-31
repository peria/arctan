#ifndef DRM_DRM_H_
#define DRM_DRM_H_

#include "base/base.h"

class Integer;

// DRM (Binary splitting method) algorithm to reduce
//   1 /      C_0/      C_1/   /          C_{n-2}/       \   \\\      P
//  ---|B_0 + ---|B_1 + ---|...|B_{n-2} + -------|B_{n-1}|...||| ==> ---
//  A_0\      A_1\      A_2\   \          A_{n-1}\       /   ///      Q
// where A_k, B_k, C_k are set by SetA(k), SetB(k), SetC(k).
class Drm {
 public:
  Drm(int64 x, int64 n);

  virtual ~Drm();

  // Computes above equation and stores P and Q.
  // p/q is computed values of arctan(1/x_) with expanding in n_ terms.
  virtual void Compute(Integer* p, Integer* q);

 protected:
  // Recursively calls itself to merge terms in [low, mid) and [mid, up).
  // Then Core() merges them into one term (A, B, C).
  // If the range [low, up) figures one integer, then set values for a term.
  virtual void Core(int64 low, int64 up, Integer* a, Integer* b, Integer* c);

  // Set values for k-th term.
  virtual void SetValues(int64 k, Integer* a, Integer* b, Integer* c);

  // The quotient of the argument of arctan.
  int64 x_;
  int64 n_;
};

#endif  // DRM_DRM_H_
