#include "../gtest/gtest.h"
#include "../board.cc"
#include "../line.cc"

TEST (Line, ContinuousLength) {
  Board::Line line(kStoneBlack);
  EXPECT_EQ(0, line.ContinuousLength());
  for (int i = 0; i < 10; i++) {
    line.Append(0, i);
    EXPECT_EQ(i + 1, line.ContinuousLength());
  }
  line.Append(0, 11);
  EXPECT_EQ(10, line.ContinuousLength());
}

TEST (Line, DiscontinuousLength) {
  Board::Line line(kStoneBlack);
  EXPECT_EQ(0, line.DiscontinuousLength());
}

TEST (Line, IsLine) {
  Board::Line line(kStoneBlack);
  EXPECT_EQ(false, line.IsLine());
}
