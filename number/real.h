#ifndef PI_NUMBER_REAL_H_
#define PI_NUMBER_REAL_H_

#include <gmp.h>
#include "base/base.h"
#include "number/integer.h"

// Real class stores an real number in long precision.
class Real {
 public:
  Real();
  Real(const Integer& n);
  ~Real();

  void SetValue(double r);

  const mpf_t& Mpf() const { return r_; }

  // Static methods --------------------------------------------------

  // Sets precision of mantissa in bits.
  static void SetPrecision(int64 precision);

  // Computes (*c) = a + b.
  static void Add(const Real& a, const Real& b, Real* c);

  // Computes (*c) = a / b.
  static void Div(const Real& a, const Real& b, Real* c);

 private:
  mpf_t r_;
};

#endif  // PI_NUMBER_REAL_H_
