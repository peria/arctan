#ifndef DRM_NUMBER_INTEGER_H_
#define DRM_NUMBER_INTEGER_H_

#include <gmp.h>
#include "base/base.h"

// Integer class stores an integer in long precision.
class Integer {
 public:
  Integer();
  ~Integer();

  void SetValue(int64 n);

  const mpz_t& Mpz() const { return n_; }

  // Static methods --------------------------------------------------

  // Computes (*c) = a + b.
  static void Add(const Integer& a, const Integer& b, Integer* c);

  // Computes (*c) = a * b.
  static void Mul(const Integer& a, const Integer& b, Integer* c);
  static void Mul(const Integer& a, const int64 b, Integer* c);

 private:
  mpz_t n_;
};

#endif  // DRM_NUMBER_INTEGER_H_