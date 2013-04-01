#include "../gtest/gtest.h"
#include "../line.cc"
#include "../board.cc"
#include "../brain.cc"

TEST (Line, DirectionVector) {
  Brain brain(kStoneBlack);
  Board board;
  board.set_stone(11, 11, kStoneWhite);
  board.set_stone(12, 12, kStoneBlack);
  board.set_stone(13, 13, kStoneBlack);
  board.set_stone(14, 14, kStoneBlack);
  board.set_stone(15, 15, kStoneBlack);
  Position put_point = brain.GetPutPosition(board);  

  EXPECT_EQ(16, put_point.x);
  EXPECT_EQ(16, put_point.y);
}

