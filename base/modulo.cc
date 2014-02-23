#include "base/modulo.h"

#include <cassert>

#include "base/base.h"

uint32 Modulo::SquareRoot(uint32 a, uint32 n) {
  const uint32 m = n - 1;
  uint32 d = m / (m & -m);
  for (uint64 r = 2; r < n / 2; ++r) {
    uint64 rr = Power(r, d, n);
    // Increment i, just in case.
    for (int i = 0; i < 32 && rr != 1; ++i) {
      uint64 sq = (rr * rr) % n;
      if (sq == m)
        return rr;
      rr = sq;
    }
  }
  // Could not find roots.
  return 0;
}

uint32 Modulo::Power(uint32 b, uint32 e, uint32 n) {
  uint64 ret = 1;
  uint64 mul = b;
  for (uint64 i = 1; i <= e; i <<= 1) {
    if (e & i)
      ret = (ret * mul) % n;
    mul = (mul * mul) % n;
  }
  return ret;
}
