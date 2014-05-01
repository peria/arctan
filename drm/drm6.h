#ifndef DRM_DRM6_H_
#define DRM_DRM6_H_

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
// In one step, every 2 terms are merged into one, and the merging is done
// recursively.
class Drm6 : public Drm {
 public:
  Drm6(int64 x, int64 n);
  virtual void Compute(Integer* p, Integer* q);

 protected:
  void Core(int64 low, int64 up, Integer* a, Integer* b, Integer* c);
  void Core2(int64 k0, int64 width, int64 level, Integer* a, Integer* b);

  virtual void SetValues(int64 k, Integer* a, Integer* b);

  // Merge every |m_|=2^|levle_| terms in perfect tournament.
  int64 m_;
  int level_;

  std::vector<Integer> gcd_;
  std::vector<Integer> xk_;  // List of x^k, with constant k's.
};

#endif  // DRM_DRM5_H_
