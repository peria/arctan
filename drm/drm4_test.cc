#include "drm/drm4.h"

#include <gtest/gtest.h>

#include <vector>

#include "base/base.h"
#include "number/integer.h"

class TestableDrm4 : public Drm4 {
 public:
  using Drm4::Core;
  using Drm4::DivisionCore;
  using Drm4::SetValues;
  using Drm4::CopyGcdForTest;
};

class Drm4Test : public testing::Test {
};

TEST_F(Drm4Test, Init) {
  std::vector<Integer> gcd;
  Drm4::Init();
  Drm4::CopyGcdForTest(&gcd);

  //             2  4    8           16                    32
  int64 ans[] = {1, 3, 5*7, 3*3*5*11*13, 3*3*5*17*19*23*29*31};
  for (size_t i = 0; i < gcd.size(); ++i)
    EXPECT_EQ(ans[i], gcd[i].Int64()) << "For i = " << i;
}

TEST_F(Drm4Test, DivisionCore) {
}
