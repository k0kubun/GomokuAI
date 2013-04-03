#include "../gtest/gtest.h"
#include "../line.cc"
#include "../board.cc"

class BoardTest : public Board {
 public:
  static void TestGetDiscontinuousLineWithDirection() {
    BoardTest board;
    Board::Line line;
    for (int i = 0; i < 5; i++) {
      board.set_stone(0, i, kStoneBlack);
    }
    line = board.GetDiscontinuousLineWithDirection(
        0, 0, kStoneBlack, kDirectionVector[kDirectionVertical]);
    EXPECT_EQ(5, line.ContinuousLength());

    board.set_stone(0, 3, kStoneBlank);
    line = board.GetDiscontinuousLineWithDirection(
        0, 0, kStoneBlack, kDirectionVector[kDirectionVertical]);
    EXPECT_EQ(4, line.DiscontinuousLength());

    board = BoardTest();
    line = board.GetDiscontinuousLineWithDirection(
        0, 0, kStoneBlack, kDirectionVector[kDirectionVertical]);
    EXPECT_EQ(false, line.Exists());

    board = BoardTest();
    board.set_stone(6, 11, kStoneWhite);
    board.set_stone(7, 12, kStoneWhite);
    board.set_stone(9, 14, kStoneWhite);
    line = board.GetDiscontinuousLineWithDirection(
        7, 12, kStoneWhite, kDirectionVector[kDirection45Degree]);
    EXPECT_EQ(3, line.DiscontinuousLength());
    EXPECT_EQ(true, line.IsAliveIn(board));
  }

  static void TestGetMaxLengthAliveDiscontinuousLine() {
    BoardTest board;
    Board::Line line;
    board.set_stone(6, 11, kStoneWhite);
    board.set_stone(7, 12, kStoneWhite);
    board.set_stone(9, 14, kStoneWhite);
    line = board.GetMaxLengthAliveDiscontinuousLine(7, 12, kStoneWhite);
    EXPECT_EQ(3, line.DiscontinuousLength());
  }
};

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
 
TEST (Board, IsBannedPoint) {
  Board board;
  if (kAllowed3x3[kStoneBlack] == false) {
    board.set_stone(0, 1, kStoneBlack);
    board.set_stone(0, 2, kStoneBlack);
    board.set_stone(1, 0, kStoneBlack);
    board.set_stone(2, 0, kStoneBlack);
    EXPECT_EQ(true, board.IsBannedPoint(0, 0, kStoneBlack));
  }

  board = Board();
  if (kAllowed4x4[kStoneBlack] == false) {
    board.set_stone(0, 1, kStoneBlack);
    board.set_stone(0, 2, kStoneBlack);
    board.set_stone(0, 3, kStoneBlack);
    board.set_stone(2, 0, kStoneBlack);
    board.set_stone(3, 0, kStoneBlack);
    board.set_stone(4, 0, kStoneBlack);
    EXPECT_EQ(true, board.IsBannedPoint(0, 0, kStoneBlack));
  }
}

TEST (Board, IsInTheBoard) {
  EXPECT_EQ(true, Board::IsInTheBoard(0, 0));
  EXPECT_EQ(true, Board::IsInTheBoard(kBoardSize - 1, kBoardSize - 1));
  EXPECT_EQ(false, Board::IsInTheBoard(-1, -1));
  EXPECT_EQ(false, Board::IsInTheBoard(kBoardSize, kBoardSize));
}

// TEST (Board, GetDiscontinuousLineWithDirection) {
//   BoardTest::TestGetDiscontinuousLineWithDirection();
// }

// TEST (Board, GetMaxLengthAliveDiscontinuousLine) {
//   BoardTest::TestGetMaxLengthAliveDiscontinuousLine();
// }

TEST (Board, MaxLineLength) {
  Board board;
  board.set_stone(7, 12, kStoneWhite);
  board.set_stone(9, 14, kStoneWhite);
  board.set_stone(6, 11, kStoneWhite);
  
  EXPECT_EQ(3, board.MaxLineLength());
}
