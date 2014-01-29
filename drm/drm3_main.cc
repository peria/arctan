#include "drm/drm2.h"

#include <gmp.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "base/base.h"
#include "number/integer.h"
#include "number/real.h"

struct Term {
  int64 coef;
  int64 quatient;
};

namespace {
const Term terms[] = {{16, 5}, {-4, 239}};
}  // namespace

void ComputePi(int64 digits, Real* pi) {
  pi->SetValue(0);
  for (const Term& term : terms) {
    Integer ip, iq;
    clock_t start = clock();
    Drm2 drm(term.quatient, digits);
    drm.Compute(&ip, &iq);
    Integer::Mul(ip, term.coef, &ip);

    Real fp(ip), fq(iq);
    Real::Div(fp, fq, &fp);  // fp = fp / fq
    Real::Add(*pi, fp, pi);  // pi = pi + fp
    clock_t end = clock();
    std::fprintf(stderr, "Computed Arctan(1/%lld) in %6.3fsec\n",
                 term.quatient,
                 static_cast<double>(end - start) / CLOCKS_PER_SEC);
  }
}

int main(int argc, const char* argv[]) {
  int64 digits = 40;
  if (argc > 1) {
    digits = std::strtoll(argv[1], NULL, 10);
  }

  Real::SetPrecision((digits + 10) * std::log2(10.0));
  Real pi;
  clock_t start = clock();
  ComputePi(digits + 10, &pi);
  clock_t end = clock();
  std::fprintf(stderr, "Computed Pi in %6.3fsec\n",
               static_cast<double>(end - start) / CLOCKS_PER_SEC);
  mpf_out_str(stdout, 10, digits + 10, pi.Mpf());
  std::puts("");

  return 0;
}
