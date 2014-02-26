#ifndef BASE_MODULO_H_
#define BASE_MODULO_H_

#include <utility>
#include "base/base.h"

// Mathmatical functions in modulus.
class Modulo {
 public:
  // Get square root of a in (mod n).  n is expected prime.
  static uint32 SquareRoot(uint32 a, uint32 n);

  // Returns b^e mod n.
  static uint32 Power(uint32 b, uint32 e, uint32 n);

  // Returns (x,y) where a*x + b*y = c, if there is a solution.
  // otherwise, returns (0, 0).
  static std::pair<int32, int32> ExtendedGcd(int32 a, int32 b, int32 c);

  // Returns GCD.
  static int32 Gcd(int32 a, int32 b);

  // Returns a^(-1) mod p.
  static int32 Inverse(int32 a, int32 p);
};

#endif  // BASE_MODULO_H_
