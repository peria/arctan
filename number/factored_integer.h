#ifndef PI_NUMBER_FACTORED_INTEGER_H_
#define PI_NUMBER_FACTORED_INTEGER_H_

#include <vector>

#include "base/base.h"
#include "number/integer.h"

struct Factor;

// FactoredInteger class stores a number in following format.
//   n_ * \prod_i factor[i].base ^ factor[i].exponent
// where |n_| is not smooth on factors.
class FactoredInteger {
 public:
  FactoredInteger();
  ~FactoredInteger();

  void SetValue(int64 n);

  Integer ToInteger() const;
  void ToInteger(Integer* n);
  void CopyFrom(const FactoredInteger& m);

  // Static methods --------------------------------------------------

  // Computes (*c) = a + b.
  static void Add(const FactoredInteger& a,
                  const FactoredInteger& b,
                  FactoredInteger* c);
  static void Add(const FactoredInteger& a,
                  const FactoredInteger& b,
                  Integer* c);

  // Computes c = a * b.
  static void Mul(const FactoredInteger& a,
                  const FactoredInteger& b,
                  FactoredInteger* c);
  static void Mul(const FactoredInteger& a,
                  const int64 b,
                  FactoredInteger* c);

 private:
  typedef std::vector<Factor>::iterator Iterator;
  typedef std::vector<Factor>::const_iterator ConstIterator;

  // Factorize |n_| and make it smooth.
  void Normalize();

  Integer n_;
  std::vector<Factor> factors_;
};

struct Factor {
  Factor(int b, int e) : base(b), exponent(e) {}

  int base;
  int exponent;
};

#endif  // PI_NUMBER_FACTORED_INTEGER_H_
