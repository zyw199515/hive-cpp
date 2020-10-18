
#include "backend/pos.h"

#include <unordered_map>

#include "glog/logging.h"
#include "gtest/gtest.h"

using namespace hive;

TEST(PosTest, Functionality) {
  Pos a(1, 2, 3);
  EXPECT_EQ(a, Pos(1, 2, 3));
  EXPECT_NE(a, Pos(4, 5, 6));

  // Can be used as key
  std::unordered_map<Pos, int> test;
  test.emplace(Pos(1, 2, 3), 4);
}