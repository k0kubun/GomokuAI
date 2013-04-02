#include "brain.h"

Brain::Brain(StoneType own_stone) {
  this->set_own_stone(own_stone);
}

void Brain::PutStone(Board board) {
  board.set_stone(GetPutPoint(board), this->own_stone());
}

void Brain::PrintBoard(Board board) {
  char c;
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      if (board.stone(i, j) == kStoneBlack) {
        c = 'x';
      } else if (board.stone(i, j) == kStoneWhite) {
        c = 'o';
      } else {
        c = '.';
      }
      printf("%c", c);
    }
    printf("\n");
  }
}

Position Brain::GetPutPoint(Board board) {
  Board::Line line;
  Position put_point;

  for (int i = kBoardSize; i >= 5; i--) {
    line = board.FindAliveDiscontinuousLine(i, opponent_stone());
    if (line.Exists()) {
      put_point = board.GetExtendPoint(line);
      if (put_point.Exists() &&
          board.IsBannedPoint(put_point, own_stone()) == false) {
        return put_point;
      }
    }
  }

  line = board.FindAliveDiscontinuousLine(4, own_stone());
  if (line.Exists()) {
    put_point = board.GetExtendPoint(line);
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }    
  }
  line = board.FindAliveDiscontinuousLine(4, opponent_stone());
  if (line.Exists()) {
    put_point = board.GetExtendPoint(line);
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }
  }

  put_point = board.FindMultipleLineMakablePoint(4, 3, own_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  line = board.FindAliveDiscontinuousLine(3, own_stone());
  if (line.Exists()) {
    put_point = board.GetExtendPoint(line);
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }
  }

  put_point = board.FindMultipleLine2MakablePoint(4, own_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  line = board.FindAliveDiscontinuousLine(3, opponent_stone());
  if (line.Exists()) {
    put_point = board.GetExtendPoint(line);
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }
  }

  put_point = board.FindMultipleLineMakablePoint(4, 3, opponent_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  put_point = board.FindMultipleLine2MakablePoint(4, opponent_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  put_point = board.FindMultipleLineMakablePoint(3, 3, own_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  put_point = board.FindMultipleLine2MakablePoint(3, own_stone());
  if (put_point.Exists()) {
    return put_point;
  }
  
  put_point = board.FindMultipleLineMakablePoint(3, 3, opponent_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  line = board.FindAliveDiscontinuousLine(2, own_stone());
  if (line.Exists()) {
    put_point = board.GetExtendPoint(line);
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }
  }
  
  put_point = board.FindMultipleLineMakablePoint(2, 2, own_stone());
  if (put_point.Exists()) {
    return put_point;
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
  int center = kBoardSize;
  if (center % 2 != 0) {
    center--;
  }
  center /= 2;
      
  for (int limit = 0; limit < center; limit++) {
    for (int i = center - limit; i <= center + limit; i++) {
      for (int j = center - limit; j <= center + limit; j++) {
        if (board.stone(i, j) == kStoneBlank) {
          return Position(i, j);
        }
      }
    }
  }
  return Position::Null();
}

StoneType Brain::OppositeStone(StoneType stone) {
  if (stone == kStoneBlack) {
    return kStoneWhite;
  } else {
    return kStoneBlack;
  }
}
