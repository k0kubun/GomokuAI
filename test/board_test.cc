#include "../gtest/gtest.h"
#include "../line.cc"
#include "../board.cc"

TEST (Board, HasWinner) {
  Board board;
  EXPECT_EQ(board.HasWinner(kStoneBlack), false);
}
