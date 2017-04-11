#ifndef DRM_DRM5_H_
#define DRM_DRM5_H_

#include <vector>

#include "base/base.h"
#include "drm/drm.h"

class Integer;

// DRM (Binary splitting method) algorithm to reduce
//   1 /      C_0/      C_1/   /          C_{n-2}/       \   \\\      P
//  ---|B_0 + ---|B_1 + ---|...|B_{n-2} + -------|B_{n-1}|...||| ==> ---
//  A_0\      A_1\      A_2\   \          A_{n-1}\       /   ///      Q
// where A_k, B_k, C_k are set by SetValues().
//
// 2 or 3 terms are merged in one step, and the merging is done
// recursively.  Before starting computation, well balanced call-tree is
// defined.  In a such case, n_ <= 2^i*3^j*5^k.
class Drm5 : public Drm {
 public:
  Drm5(int64 x, int64 n);
  void Compute(Integer* p, Integer* q);

 protected:
  // Entrance of one step.
  void Core(int64 k0, int64 width, int64 level,
            Integer* a, Integer* b, Integer* c);

  void Core2(int64 k0, int64 width, int64 level,
             Integer* a, Integer* b, Integer* c);

  void Core3(int64 k0, int64 width, int64 level,
             Integer* a, Integer* b, Integer* c);

  void Core5(int64 k0, int64 width, int64 level,
             Integer* a, Integer* b, Integer* c);

  void SetValues(int64 k, Integer* a, Integer* b, Integer* c);

  int64 m_;
  std::vector<Integer> gcd_;
  std::vector<Integer> xk_;  // List of x^k, with constant k's.
};

#endif  // DRM_DRM5_H_
