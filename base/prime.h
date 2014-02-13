#ifndef BASE_PRIME_H_
#define BASE_PRIME_H_

#include <vector>

#include "base/base.h"

class Prime {
 public:
  Prime(const int n);
  int GetPrimes(int n, std::vector<int>* primes);
  int GetNextPrime();
  int CountPrimes(int n);
  bool IsPrime(int64 n);
  void ResetIndex();

  static bool IsPrimeSt(int64 n);

 private:
  void Sieve();

  std::vector<uint8> bits_;
  const int n_;
  int index_;
  int bit_;
};

#endif  // BASE_PRIME_H_
