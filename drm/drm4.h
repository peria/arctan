#ifndef DRM_DRM4_H_
#define DRM_DRM4_H_

#include "base/base.h"

class Integer;

// DRM (Binary splitting method) algorithm to reduce
//   1 /      C_0/      C_1/   /          C_{n-2}/       \   \\\      P
//  ---|B_0 + ---|B_1 + ---|...|B_{n-2} + -------|B_{n-1}|...||| ==> ---
//  A_0\      A_1\      A_2\   \          A_{n-1}\       /   ///      Q
// where A_k, B_k, C_k are set by SetValues().
//
// Those values can be factored easily, if values for j*2^i <= k < (j+1)*2^i
// are merged by DRM.  They must be factored over p^m < 2^i.
class Drm4 {
 public:
  Drm4(int64 x, int64 n);
  void Compute(Integer* p, Integer* q);

 private:
  void Core(int64 low, int64 up, Integer* a, Integer* b, Integer* c);
  void DivisionCore(int64 low, int64 up, Integer* a, Integer* b, Integer* c);
  void SetValues(int64 k, Integer* a, Integer* b, Integer* c);

  int64 x_;
  int64 n_;
};

#endif  // DRM_DRM_H_
