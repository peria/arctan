#include "drm.h"

#include <gmp.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "base/base.h"

int main(int argc, const char* argv[]) {
  int64 digits = 40;
  if (argc > 1) {
    digits = std::strtoll(argv[1], NULL, 10);
  }
  mpf_set_default_prec((digits + 10) * std::log2(10.0));

  mpf_t pi;
  mpf_init(pi);
  Arctan::ComputePi(pi, digits);
  mpf_out_str(stdout, 10, digits, pi);
  std::puts("");
  mpf_clear(pi);

  return 0;
}
