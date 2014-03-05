#include "search/search.h"

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <iostream>
#include <vector>

#include "base/combination.h"
#include "base/time.h"

DEFINE_int64(pmax, 50, "Upper limit of prime numbers to use in sieve.");
DEFINE_int64(xmax, 250, "Upper bound of x to sieve.");
DEFINE_int32(terms, 3, "The nubmer of terms.");

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);

  double t_start = Time::GetCurrentTime();
  Search search(FLAGS_pmax, FLAGS_xmax);
  search.Sieve();
  double t_sieve = Time::GetCurrentTime();
  std::cerr << "Sieve: " << (t_sieve - t_start) << " sec.\n";

  std::vector<Formula> formulae;
  search.FindFormulae(FLAGS_terms, &formulae);

  double t_select = Time::GetCurrentTime();
  std::cerr << "Selection: " << (t_select - t_sieve) << " sec.\n";

  for (const Formula formula : formulae) {
    std::cout << formula.n << "\t"
              << formula.k << "\t";
    for (const Term term : formula.terms)
      std::cout << term.coef << "atan(1/" << term.quot << ") ";
    std::cout << "\n";
  }

  return 0;
}
