#include "drm.h"

#include <gmp.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "base/base.h"
#include "number/real.h"

int main(int argc, const char* argv[]) {
  int64 digits = 40;
  if (argc > 1) {
    digits = std::strtoll(argv[1], NULL, 10);
  }

  Real::SetPrecision((digits + 10) * std::log2(10.0));
  Real pi;
  ComputePi(digits + 10, &pi);
  mpf_out_str(stdout, 10, digits + 10, pi.Mpf());
  std::puts("");

  return 0;
}
