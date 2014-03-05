#include "base/time.h"

#include <sys/time.h>

#include "base/base.h"

double Time::GetCurrentTime() {
  timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec + t.tv_usec * 1e-6;
}
