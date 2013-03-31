#include "brain.h"

Brain::Brain(StoneType play_stone) {
  this->set_play_stone(play_stone);
}

void Brain::PutStone(Board board) {
  board.set_stone(GetPutPosition(board), this->play_stone());
}

Position Brain::GetPutPosition(Board board) {
  return GetEmptyPosition(board);
}

StoneType Brain::play_stone() {
  return play_stone_;
}

void Brain::set_play_stone(StoneType play_stone) {
  play_stone_ = play_stone;
}

Position Brain::GetEmptyPosition(Board board) {
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      if (board.stone(i, j) != kStoneBlank) {
        return Position(i, j);
      }
    }
  }
  return Position(0, 0);
}
