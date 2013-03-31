#include "brain.h"

Brain::Brain(StoneType own_stone) {
  this->set_own_stone(own_stone);
}

void Brain::PutStone(Board board) {
  board.set_stone(GetPutPosition(board), this->own_stone());
}

Position Brain::GetPutPosition(Board board) {
  Position put_position;
  put_position = 
  return GetEmptyPosition(board);
}

StoneType Brain::own_stone() {
  return own_stone_;
}

void Brain::set_own_stone(StoneType own_stone) {
  own_stone_ = own_stone;
  opponent_stone_ = OppositeStone(own_stone);
}

Position Brain::GetEmptyPosition(Board board) {
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      if (board.stone(i, j) == kStoneBlank) {
        return Position(i, j);
      }
    }
  }
  return Position(0, 0);
}

StoneType Brain::OppositeStone(StoneType stone) {
  if (stone == kStoneBlack) {
    return kStoneWhite;
  } else {
    return kStoneBlack;
  }
}
