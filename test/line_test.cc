#include "../gtest/gtest.h"
#include "../board.cc"
#include "../line.cc"

TEST (Line, ContinuousLength) {
  Board::Line line(kStoneBlack);
  EXPECT_EQ(0, line.ContinuousLength());
}

TEST (Line, DiscontinuousLength) {
  Board::Line line(kStoneBlack);
  EXPECT_EQ(0, line.DiscontinuousLength());
}

TEST (Line, IsLine) {
  Board::Line line(kStoneBlack);
  EXPECT_EQ(false, line.IsLine());
}
