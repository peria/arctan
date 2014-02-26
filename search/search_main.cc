#include "search/search.h"

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <iostream>
#include <vector>

#include "base/combination.h"

DEFINE_int64(pmax, 50, "Upper limit of prime numbers to use in sieve.");
DEFINE_int64(xmax, 250, "Upper bound of x to sieve.");
DEFINE_int32(terms, 3, "The nubmer of terms.");

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);

  Search search(FLAGS_pmax, FLAGS_xmax);
  search.Sieve();

#if 0
  std::vector<Element> elements;
  search.Debug(&elements);
  for (const Element elem : elements) {
    std::cout << elem.x << ":" << elem.value << ":";
    for (const auto factor : elem.factors)
      std::cout << " " << factor.first << "^" << factor.second;
    std::cout << "\n";
  }
#endif

  std::vector<Formula> formulae;
  search.FindFormulae(FLAGS_terms, &formulae);
  for (const Formula formula : formulae) {
    std::cout << formula.k << " : ";
    for (const Term term : formula.terms)
      std::cout << term.coef << "atan(1/" << term.quot << ") ";
    std::cout << "\n";
  }

  return 0;
}
