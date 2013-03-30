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

TEST (Line, Append) {
  Board::Line line_a, line_b;
  line_a = Board::Line::LineMake(LinePoint(0,1), LinePoint(0,2));
  line_b = Board::Line::LineMake(LinePoint(0,3), LinePoint(0,4));
  line_a.Append(line_b);
  EXPECT_EQ(4, line_a.DiscontinuousLength());
}

TEST (Line, Sort) {
  Board::Line line(kStoneBlack);
  line.Append(1, 1);
  line.Append(0, 0);
  line.Append(2, 2);
  line.Sort();
  EXPECT_EQ(0, line.PointFront().x);
  EXPECT_EQ(2, line.PointBack().x);

  line = Board::Line(kStoneBlack);
  line.Append(0, 1);
  line.Append(0, 3);
  line.Append(0, 2);  
  line.Sort();
  EXPECT_EQ(1, line.PointFront().y);
  EXPECT_EQ(3, line.PointBack().y);

  line = Board::Line(kStoneBlack);
  line.Append(2, 0);
  line.Append(0, 0);
  line.Append(1, 0);  
  line.Sort();
  EXPECT_EQ(0, line.PointFront().x);
  EXPECT_EQ(2, line.PointBack().x);
  
  line = Board::Line(kStoneBlack);
  line.Append(2, 0);
  line.Append(1, 1);
  line.Append(0, 2);
  line.Sort();
  EXPECT_EQ(2, line.PointFront().x);
  EXPECT_EQ(0, line.PointBack().x);
}


TEST (Line, EdgeWithDirection) {
  Board::Line line(kStoneBlack);
  LinePoint point;
  line.Append(2, 2);
  line.Append(0, 0);
  line.Append(1, 1);
  point = line.EdgeWithDirection(kDirectionVector[kDirection45Degree]);
  EXPECT_EQ(2, point.x);

  line = Board::Line(kStoneBlack);
  line.Append(1, 2);
  line.Append(1, 4);
  line.Append(1, 3);
  point = line.EdgeWithDirection(kDirectionVector[kDirection45Degree]);
  EXPECT_EQ(4, point.y);
}

TEST (Line, DirectionVector) {
  Board::Line line(kStoneBlack);
  Vector vector;
  line = Board::Line::LineMake(LinePoint(0,1), LinePoint(0,2));
  EXPECT_EQ(0, line.PointBack().x);
  EXPECT_EQ(0, line.PointFront().x);
  
  vector = line.DirectionVector();
  EXPECT_EQ(0, vector.x);
  EXPECT_EQ(1, vector.y);
}
