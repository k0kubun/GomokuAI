#include "brain.h"

Brain::Brain(StoneType own_stone) {
  this->set_own_stone(own_stone);
}

void Brain::PutStone(Board board) {
  board.set_stone(GetPutPosition(board), this->own_stone());
}

Position Brain::GetPutPosition(Board board) {
  Board::Line line;
  Position put_point;

  for (int i = board.MaxLineLength(); i > 0; i--) {
    line = board.FindAliveDiscontinuousLine(i, own_stone());
    if (line.Exists()) {
      put_point = GetExtendPoint(board, line);
      if (board.IsBannedPoint(put_point, own_stone()) == false) {
        return put_point;
      }
    }

    line = board.FindAliveDiscontinuousLine(i, opponent_stone());
    if (line.Exists()) {
      put_point = GetExtendPoint(board, line);
      if (board.IsBannedPoint(put_point, own_stone()) == false) {
        return put_point;
      }
    }
  }
  
  return GetEmptyPoint(board);
}

StoneType Brain::own_stone() {
  return own_stone_;
}

void Brain::set_own_stone(StoneType own_stone) {
  own_stone_ = own_stone;
  opponent_stone_ = OppositeStone(own_stone);
}

StoneType Brain::opponent_stone() {
  return opponent_stone_;
}

Position Brain::GetEmptyPoint(Board board) {
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      if (board.stone(i, j) == kStoneBlank) {
        return Position(i, j);
      }
    }
  }
  return Position(0, 0);
}

Position Brain::GetExtendPoint(Board board, Board::Line line) {
  if (line.IsContinuous()) {
    if (board.AllowsToPut(line.DirectionalBlank())) {
      return line.DirectionalBlank();
    } else if (board.AllowsToPut(line.UndirectionalBlank())) {
      return line.UndirectionalBlank();
    } else {
      return Position::Null();
    }
  } else {
    return line.SplitPoint();
  }
}

StoneType Brain::OppositeStone(StoneType stone) {
  if (stone == kStoneBlack) {
    return kStoneWhite;
  } else {
    return kStoneBlack;
  }
}
