#ifndef PI_NUMBER_FACTORED_INTEGER_H_
#define PI_NUMBER_FACTORED_INTEGER_H_

#include <vector>

#include "base/base.h"
#include "number/integer.h"

typedef std::pair<int, int> Factor;

// FactoredInteger class stores a number in following format.
//   n_ * \prod_i factor[i].first ^ factor[i].second
// where |n_| is not smooth on factors.
class FactoredInteger {
 public:
  FactoredInteger();
  ~FactoredInteger();

  void SetValue(int64 n);

  Integer ToInteger() const;
  void ToInteger(Integer*);

  // Static methods --------------------------------------------------

  // Computes (*c) = a + b.
  static void Add(const FactoredInteger& a,
                  const FactoredInteger& b,
                  Integer* c);

  // Computes c = a * b.
  static void Mul(const FactoredInteger& a,
                  const FactoredInteger& b,
                  FactoredInteger* c);

 private:
  Integer n_;
  std::vector<Factor> factors_;
};

#endif  // PI_NUMBER_FACTORED_INTEGER_H_
