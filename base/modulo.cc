#include "base/modulo.h"

#include <cassert>
#include <utility>

#include "base/base.h"

uint32 Modulo::SquareRoot(const uint32 a, uint32 n) {
  uint32 m = n - 1;
  uint32 d = m / (m & -m);
  for (uint64 r = 2; r <= n / 2; ++r) {
    uint64 rr = Power(r, d, n);
    // Increment i, just in case.
    for (int i = 0; i < 32 && rr != 1; ++i) {
      uint64 sq = (rr * rr) % n;
      if (sq == a)
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

std::pair<int32, int32> Modulo::ExtendedGcd(int32 a, int32 b, int32 c) {
  if (a == 0 || b == 0)
    return std::pair<int32, int32>(0, 0);

  int32 x0 = 1, y0 = 0, z0 = a;
  int32 x1 = 0, y1 = 1, z1 = b;
  while (z1) {
    int32 q = z0 / z1;
    int32 x2 = x0 - q * x1;
    int32 y2 = y0 - q * y1;
    int32 z2 = z0 - q * z1;
    x0 = x1; y0 = y1; z0 = z1;
    x1 = x2; y1 = y2; z1 = z2;
  }
  if (c % z0 != 0)
    return std::pair<int32, int32>(0, 0);

  c /= z0;
  return std::pair<int32, int32>(x0 * c, y0 * c);
}

int32 Modulo::Inverse(int32 a, int32 p) {
  std::pair<int32, int32> r = ExtendedGcd(p, a, 1);
  return r.second;
}
