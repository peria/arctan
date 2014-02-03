#ifndef DRM_DRM3_H_
#define DRM_DRM3_H_

#include <gmp.h>

#include "base/base.h"
#include "drm/drm.h"

class Integer;
class FactoredInteger;

// DRM (Binary splitting method) algorithm to reduce
//   1 /      C_0/      C_1/   /          C_{n-2}/       \   \\\      P
//  ---|B_0 + ---|B_1 + ---|...|B_{n-2} + -------|B_{n-1}|...||| ==> ---
//  A_0\      A_1\      A_2\   \          A_{n-1}\       /   ///      Q
// where A_k, B_k, C_k are set by SetValues(k).
//
// Drm3 class computes using FactoredInteger class, which describes a partially
// factored number.
class Drm3 : public Drm {
 public:
  Drm3(int64 x, int64 n);
  virtual void Compute(Integer* p, Integer* q);

 private:
  virtual void Core(int64 low, int64 up, FactoredInteger* a, Integer* b,
		    FactoredInteger* c);
  virtual void SetValues(int64 k, FactoredInteger* a, Integer* b,
			 FactoredInteger* c);
};

#endif  // DRM_DRM2_H_
