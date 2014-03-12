#include "drm/drm.h"
#include "drm/drm2.h"
#include "drm/drm3.h"
#include "drm/drm4.h"
#include "drm/drm5.h"

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <gmp.h>

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "base/base.h"
#include "number/integer.h"
#include "number/real.h"

DEFINE_int64(digits, 40, "Computed digits, counted after decimal point.");
DEFINE_int32(algorithm, 1, "Using algorithm. Choose one of 1-4.");

namespace {
struct Term {
  int64 coef;
  int64 quatient;
};

const Term terms[] = {{16, 5}, {-4, 239}};
}  // namespace

Drm* ChooseAlgorithm(int64 quat, int64 digits) {
  switch (FLAGS_algorithm) {
  case 1:
    return new Drm(quat, digits);
  case 2:
    return new Drm2(quat, digits);
  case 3:
    return new Drm3(quat, digits);
  case 4:
    Drm4::Init();
    return new Drm4(quat, digits);
  case 5:
    return new Drm5(quat, digits);
  }
  return NULL;
}

void ComputePi(Real* pi) {
  std::fprintf(stderr,
               "Computing Pi for %ld digits using algorithm No.%d.\n",
              FLAGS_digits, FLAGS_algorithm);

  pi->SetValue(0);
  for (const Term& term : terms) {
    Integer ip, iq;
    clock_t start = clock();
    Drm* drm = ChooseAlgorithm(term.quatient, FLAGS_digits);
    assert(drm != NULL);
    drm->Compute(&ip, &iq);
    Integer::Mul(ip, term.coef, &ip);

    Real fp(ip), fq(iq);
    Real::Div(fp, fq, &fp);  // fp = fp / fq
    Real::Add(*pi, fp, pi);  // pi = pi + fp
    clock_t end = clock();
    std::fprintf(stderr, "Computed Arctan(1/%lld) in %6.3fsec\n",
                 term.quatient,
                 static_cast<double>(end - start) / CLOCKS_PER_SEC);
    delete drm;
  }
}

bool CheckOptions() {
  if (FLAGS_digits < 0)
    return false;
  if (FLAGS_algorithm < 1 || FLAGS_algorithm > 5)
    return false;
  return true;
}

void Usage(const char* argv0) {
  std::cerr << "Usage: " << argv0 << " <option>\n"
            << " Please use " << argv0 << " --help for the details.\n";
}

int main(int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  if (!CheckOptions()) {
    Usage(argv[0]);
    return 0;
  }

  FLAGS_digits += 10;
  Real::SetPrecision((FLAGS_digits) * std::log2(10.0));
  Real pi;
  clock_t start = clock();
  ComputePi(&pi);
  clock_t end = clock();
  std::fprintf(stderr, "Computed Pi in %6.3fsec\n",
               static_cast<double>(end - start) / CLOCKS_PER_SEC);
  mpf_out_str(stdout, 10, FLAGS_digits, pi.Mpf());
  std::puts("");

  return 0;
}
