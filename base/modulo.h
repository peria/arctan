#ifndef BASE_MODULO_H_
#define BASE_MODULO_H_

#include "base/base.h"

// Mathmatical functions in modulus.
class Modulo {
 public:
  // Get square root of a in (mod n).  n is expected prime.
  static uint32 SquareRoot(uint32 a, uint32 n);

  // Returns b^e mod n.
  static uint32 Power(uint32 b, uint32 e, uint32 n);
};

#endif  // BASE_MODULO_H_
