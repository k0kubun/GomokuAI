#include "../gtest/gtest.h"
#include "../line.cc"
#include "../board.cc"
#include "../brain.cc"

TEST (Line, DirectionVector) {
  if (kBoardSize == 20) {
    Brain brain(kStoneBlack);
    Board board;
    board.set_stone(11, 11, kStoneWhite);
    board.set_stone(12, 12, kStoneBlack);
    board.set_stone(13, 13, kStoneBlack);
    board.set_stone(14, 14, kStoneBlack);
    board.set_stone(15, 15, kStoneBlack);
    Position put_point = brain.GetPutPoint(board);  
    EXPECT_EQ(16, put_point.x);
    EXPECT_EQ(16, put_point.y);

    board = Board();
    board.set_stone(10, 10, kStoneWhite);
    board.set_stone(9, 9, kStoneBlack);
    board.set_stone(10, 9, kStoneWhite);
    board.set_stone(10, 11, kStoneBlack);
    board.set_stone(9, 10, kStoneWhite);
    board.set_stone(11, 10, kStoneBlack);
    board.set_stone(12, 9, kStoneWhite);
    board.set_stone(9, 12, kStoneBlack);
    board.set_stone(8, 11, kStoneWhite);
    board.set_stone(8, 13, kStoneBlack);
    board.set_stone(7, 14, kStoneWhite);
    board.set_stone(7, 12, kStoneBlack);
    board.set_stone(8, 12, kStoneWhite);
    board.set_stone(9, 14, kStoneBlack);
    board.set_stone(6, 11, kStoneWhite);
    board.set_stone(10, 15, kStoneBlack);
    board.set_stone(11, 16, kStoneWhite);
    board.set_stone(9, 11, kStoneBlack);
    board.set_stone(10, 13, kStoneWhite);
    board.set_stone(11, 11, kStoneBlack);
    board.set_stone(11, 8, kStoneWhite);
    put_point = brain.GetPutPoint(board);  
    EXPECT_EQ(12, put_point.x);
    EXPECT_EQ(7, put_point.y);
    EXPECT_EQ(4, board.MaxLineLength());
  }
}

