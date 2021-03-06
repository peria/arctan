#ifndef PI_NUMBER_INTEGER_H_
#define PI_NUMBER_INTEGER_H_

#include <gmp.h>
#include <string>

#include "base/base.h"

// Integer class stores an integer in long precision.
class Integer {
 public:
  Integer();
  Integer(const Integer& n);
  Integer(const int64 n);
  ~Integer();

  void SetValue(int64 n);
  void CopyFrom(const Integer& n);

  std::string GetString();

  const mpz_t& Mpz() const { return n_; }
  int64 Int64() const { return mpz_get_si(n_); };
  
  // Static methods --------------------------------------------------

  // Computes (*c) = a + b.
  static void Add(const Integer& a, const Integer& b, Integer* c);

  // Computes (*c) = a * b.
  static void Mul(const Integer& a, const Integer& b, Integer* c);
  static void Mul(const Integer& a, const int64 b, Integer* c);

  // Returns a remainder of a / b.
  static int64 Remain(const Integer& a, const int64 b);

  // Computes (*c) = a / b, assuming a is a multiple of b.
  static void Div(const Integer& a, const int64 b, Integer* c);
  static void Div(const Integer& a, const Integer& b, Integer* c);

  // Computes (*c) = a ^ b.
  static void Power(const int64 a, const int64 b, Integer* c);

  static void Print(const Integer& n);

 private:
  mpz_t n_;
};

#endif  // PI_NUMBER_INTEGER_H_
