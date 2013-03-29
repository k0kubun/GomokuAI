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

TEST (Line, Sort) {
  Board::Line line(kStoneBlack);
  line.Append(1, 1);
  line.Append(0, 0);
  line.Append(2, 2);
  line.Sort();
  EXPECT_EQ(0, line.PointBegin()->x);
  EXPECT_EQ(2, line.PointBack()->x);
  
  // line = Board::Line(kStoneBlack);
  // line.Append(0, 1);
  // line.Append(0, 2);
  // line.Append(0, 0);  
  // line.Sort();
  // EXPECT_EQ(0, line.PointBegin()->y);
  // EXPECT_EQ(2, line.PointBack()->y);

  line = Board::Line(kStoneBlack);
  line.Append(2, 0);
  line.Append(0, 0);
  line.Append(1, 0);  
  line.Sort();
  EXPECT_EQ(0, line.PointBegin()->x);
  EXPECT_EQ(2, line.PointBack()->x);
  
  line = Board::Line(kStoneBlack);
  line.Append(2, 0);
  line.Append(1, 1);
  line.Append(0, 2);
  line.Sort();
  EXPECT_EQ(0, line.PointBegin()->x);
  EXPECT_EQ(2, line.PointBack()->x);
}

TEST (Line, DirectionVector) {
  Board::Line line(kStoneBlack);
  line.Append(0, 2);
  line.Append(0, 0);
  Vector direction = line.DirectionVector();
  EXPECT_EQ(0, direction.x);
  EXPECT_EQ(1, direction.y);
}
