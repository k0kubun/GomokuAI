#include "../gtest/gtest.h"
#include "../line.cc"
#include "../board.cc"

TEST (Board, HasWinner) {
  Board board;
  for (int i = 0; i < 5; i++) {
    board.set_stone(0, i, kStoneBlack);
  }
  EXPECT_EQ(true, board.HasWinner(kStoneBlack));
}

TEST (Board, StoneNum) {
  Board board;
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      board.set_stone(i, j, kStoneBlack);
      EXPECT_EQ(i * kBoardSize + j + 1, board.StoneNum());
    }
  }
}

TEST (Board, FindContinuousLineByLength) {
  Board board;
  Board::Line line;
  for (int i = 0; i < 5; i++) {
    board.set_stone(0, i, kStoneBlack);
    line = board.FindContinuousLineByLength(i + 1, kStoneBlack);
    EXPECT_EQ(i + 1, line.ContinuousLength());
  }
}

TEST (Board, GetMaxLengthContinuousLine) {
  Board board;
  Board::Line line;
  for (int i = 0; i < 5; i++) {
    board.set_stone(0, i, kStoneBlack);
    line = board.GetMaxLengthContinuousLine(0, 0, kStoneBlack);
    EXPECT_EQ(i + 1, line.ContinuousLength());
  }
}

TEST (Board, IsInTheBoard) {
  EXPECT_EQ(true, Board::IsInTheBoard(0, 0));
  EXPECT_EQ(true, Board::IsInTheBoard(kBoardSize - 1, kBoardSize - 1));
  EXPECT_EQ(false, Board::IsInTheBoard(-1, -1));
  EXPECT_EQ(false, Board::IsInTheBoard(kBoardSize, kBoardSize));
}

TEST (Board, GetContinuousLineWithDirection) {
  Board board;
  Board::Line line;
  for (int i = 0; i < 5; i++) {
    board.set_stone(0, i, kStoneBlack);
  }
  line = board.GetContinuousLineWithDirection(
      0, 0, kStoneBlack, kDirectionVector[kDirectionVertical]);
  EXPECT_EQ(5, line.ContinuousLength());
}

TEST (Board, GetDiscontinuousLineWithDirection) {
  Board board;
  Board::Line line;
  for (int i = 0; i < 5; i++) {
    board.set_stone(0, i, kStoneBlack);
  }
  line = board.GetDiscontinuousLineWithDirection(
      0, 0, kStoneBlack, kDirectionVector[kDirectionVertical]);
  EXPECT_EQ(5, line.ContinuousLength());

  board.set_stone(0, 3, kStoneBlank);
  EXPECT_EQ(4, line.ContinuousLength());
}
