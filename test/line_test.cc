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
  line_a = Board::Line::LineMake(Position(0,1), Position(0,2));
  line_b = Board::Line::LineMake(Position(0,3), Position(0,4));
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
  Position point;
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

TEST (Line, SplitPoint) {
  Board::Line line(kStoneBlack);
  Position point;

  line.Append(0, 0);
  line.Append(1, 1);
  line.Append(2, 2);
  line.Append(4, 4);
  point = line.SplitPoint();
  EXPECT_EQ(3, point.x);
  EXPECT_EQ(3, point.y);
  EXPECT_EQ(true, point.Exists());

  line = Board::Line(kStoneBlack);
  line.Append(0, 0);
  line.Append(1, 1);
  line.Append(2, 2);
  point = line.SplitPoint();
  EXPECT_EQ(false, point.Exists());
}

TEST (Line, DirectionVector) {
  Board::Line line(kStoneBlack);
  Vector vector;
  line = Board::Line::LineMake(Position(0,1), Position(0,2));
  EXPECT_EQ(0, line.PointBack().x);
  EXPECT_EQ(0, line.PointFront().x);
  
  vector = line.DirectionVector();
  EXPECT_EQ(0, vector.x);
  EXPECT_EQ(1, vector.y);
}

TEST (Line, IsAliveIn) {
  Board board;
  Board::Line line(kStoneBlack);
  board.set_stone(0, 1, kStoneBlack);
  board.set_stone(0, 2, kStoneBlack);
  line.Append(0, 1);
  line.Append(0, 2);
  EXPECT_EQ(true, line.IsAliveIn(board));

  board = Board();
  board.set_stone(7, 12, kStoneBlack);
  board.set_stone(9, 14, kStoneBlack);
  board.set_stone(10, 15, kStoneBlack);
  line = Board::Line(kStoneBlack);
  line.Append(7, 12);
  line.Append(9, 14);
  line.Append(10, 15);
  EXPECT_EQ(true, line.IsAliveIn(board));
}
