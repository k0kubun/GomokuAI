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

  line = board.FindAliveDiscontinuousLine(4, own_stone());
  if (line.Exists()) {
    put_point = GetExtendPoint(board, line);
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }    
  }
  line = board.FindAliveDiscontinuousLine(4, opponent_stone());
  if (line.Exists()) {
    put_point = GetExtendPoint(board, line);
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }
  }

  put_point = FindMultipleLineMakablePoint(board, 4, 4, own_stone());
  if (put_point.Exists()) {
    return put_point;
  }
  put_point = FindMultipleLineMakablePoint(board, 4, 3, own_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  line = board.FindAliveDiscontinuousLine(3, own_stone());
  if (line.Exists()) {
    put_point = GetExtendPoint(board, line);
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }
  }
  line = board.FindAliveDiscontinuousLine(3, opponent_stone());
  if (line.Exists()) {
    put_point = GetExtendPoint(board, line);
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }
  }

  put_point = FindMultipleLineMakablePoint(board, 4, 4, opponent_stone());
  if (put_point.Exists()) {
    return put_point;
  }
  put_point = FindMultipleLineMakablePoint(board, 4, 3, opponent_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  put_point = FindMultipleLineMakablePoint(board, 3, 3, own_stone());
  if (put_point.Exists()) {
    return put_point;
  }
  put_point = FindMultipleLineMakablePoint(board, 3, 3, opponent_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  line = board.FindAliveDiscontinuousLine(2, own_stone());
  if (line.Exists()) {
    put_point = GetExtendPoint(board, line);
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }
  }
  
  put_point = FindMultipleLineMakablePoint(board, 2, 2, own_stone());
  if (put_point.Exists()) {
    return put_point;
  }
  
  return GetEmptyPoint(board);
}

Position Brain::FindMultipleLineMakablePoint(Board board, int first_length,
                                             int second_length, StoneType stone) {
  Board virtual_board;
  Board::Line line;
  Position put_position = Position::Null();
  int max_length = -1;
  
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      if (board.stone(i, j) == kStoneBlank) {
        virtual_board = board;
        virtual_board.set_stone(i, j, stone);
        line = virtual_board.GetMaxLengthAliveDiscontinuousLine(i, j, stone);
        if (line.IsAliveIn(virtual_board) &&
            line.DiscontinuousLength() == first_length) {
          std::list<int> length_list =
              virtual_board.GetAliveDiscontinuousLineLengthList(i, j, stone);
          std::list<int>::iterator list_iter = length_list.begin();
          list_iter++;
          if (second_length == *list_iter) {
            max_length = *list_iter;
            put_position = Position(i, j);
          }
        }
      }
    }
  }
  return put_position;
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
  return GetEmptyPoint(board);
}

StoneType Brain::OppositeStone(StoneType stone) {
  if (stone == kStoneBlack) {
    return kStoneWhite;
  } else {
    return kStoneBlack;
  }
}
