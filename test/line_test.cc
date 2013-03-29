#include "../gtest/gtest.h"
#include "../board.cc"
#include "../line.cc"

// This should be deleted
TEST (SampleTest, MustBeOK) {
  EXPECT_EQ(0 + 0, 0);
}

TEST (Line, ContinuousLength) {
  Board::Line line(kStoneBlack);
  EXPECT_EQ(line.ContinuousLength(), 0);
}
