#ifndef SEARCH_SEARCH_H_
#define SEARCH_SEARCH_H_

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "base/base.h"

struct Element;
struct Formula;
typedef std::vector<int32> Row;
typedef std::vector<Row> Matrix;

// Searcher class searches pi-fomulae with arctangents in following steps.
// 1) Sieve x^2+1 for x < x_max_, using primes up to p_max_.
// 2) Find combinations of x's whose factors are common.
// 3) Generate pi-fomulae and check them.
class Search {
 public:
  Search(int64 p_max, int64 x_max);

  // Factorize x^2+1 for 0<x<=x_max, using primes up to p_max.
  void Sieve();

  void FindFormulae(int num_terms, std::vector<Formula>* formulae);

 private:
  // Add exponent of prime for x = (r + pk*i), because (x^2+1) % prime == 0
  // for such x.
  void SieveInternal(int64 root, int64 pk, int prime);

  // Returns false if any element of coeffs is 0.
  bool GetCoefficients(const Matrix& matrix, std::vector<int32>* coeffs);

  // Compute determinant of a square matrix.  |matrix| is broken.
  int32 Determ(const Matrix& matrix) const;

  std::vector<Element> elements_;

  const int64 p_max_;
  const int64 x_max_;
  std::vector<int32> primes_;

  static const int32 kNumDigits;
  static const int32 kSieveWidth;
};

struct Element {
  // Returns true if |elem| is usable in the condition where other parameters
  // figure.
  bool IsSmooth(const std::vector<int32>& primes) const;

  int32 x;
  int64 norm;
  std::unordered_map<int32, int32> factors;  // factors[base] = exponent
};

// Term describes a term of a formula, |coef|*atan(1/quot).
struct Term {
  int32 coef;
  int32 quot;
};

// k*\pi/5 = \sum_i terms[i].coef * atan(1/terms[i].quot)
struct Formula {
  int32 k;
  int32 n;
  std::vector<Term> terms;
};

#endif  // SEARCH_SEARCH_H_
