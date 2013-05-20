#include "brain.h"

Brain::Brain(StoneType own_stone) {
  this->set_own_stone(own_stone);
}

void Brain::PrintBoard(Board board) {
  char c;

  printf("EDCBA9876543210\n");
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      StoneType stone = board.stone(kBoardSize - j - 1, i);
      if (stone == kStoneBlack) {
        c = 'x';
      } else if (stone == kStoneWhite) {
        c = 'o';
      } else {
        c = '.';
      }
      printf("%c", c);
    }
    printf("\n");
  }
}

Position Brain::MinMaxSearch(Board board) {
  return GetMinMaxPoint(board, false);
}

Position Brain::AlphaBetaPruning(Board board) {
  return GetMinMaxPoint(board, true);
}

// Original Algorithm
Position Brain::GetPutPoint(Board board) {
  Board::Line line;
  Position put_point;

  if (kAllowedLong[opponent_stone()] == true) {
    for (int i = kBoardSize; i >= 5; i--) {
      line = board.FindAliveDiscontinuousLine(i, opponent_stone());
      if (line.Exists()) {
        put_point = board.GetExtendPoint(line, own_stone());
        if (put_point.Exists() &&
            board.IsBannedPoint(put_point, own_stone()) == false) {
          return put_point;
        }
      }
    }
  }

  line = board.FindAliveDiscontinuousLine(4, own_stone());
  if (line.Exists()) {
    put_point = board.GetExtendPoint(line, own_stone());
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }
  }
  line = board.FindAliveDiscontinuousLine(4, opponent_stone());
  if (line.Exists()) {
    put_point = board.GetExtendPoint(line, own_stone());
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
    put_point = board.GetExtendPoint(line, own_stone());
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }
  }

  put_point = board.FindMultipleLinePreMakablePoint(4, own_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  line = board.FindAliveDiscontinuousLine(3, opponent_stone());
  if (line.Exists()) {
    put_point = board.GetExtendPoint(line, own_stone());
    if (put_point.Exists() &&
        board.IsBannedPoint(put_point, own_stone()) == false) {
      return put_point;
    }
  }

  put_point = board.FindMultipleLineMakablePoint(4, 3, opponent_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  put_point = board.FindMultipleLinePreMakablePoint(4, opponent_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  if (kAllowed3x3[own_stone()] == true) {
    put_point = board.FindMultipleLineMakablePoint(3, 3, own_stone());
    if (put_point.Exists()) {
      return put_point;
    }
  }

  put_point = board.FindMultipleLinePreMakablePoint(3, own_stone());
  if (put_point.Exists()) {
    return put_point;
  }

  if (kAllowed3x3[opponent_stone()] == true) {
    put_point = board.FindMultipleLineMakablePoint(3, 3, opponent_stone());
    if (put_point.Exists()) {
      return put_point;
    }
  }

  line = board.FindAliveDiscontinuousLine(2, own_stone());
  if (line.Exists()) {
    put_point = board.GetExtendPoint(line, own_stone());
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

void Brain::set_own_stone(StoneType own_stone) {
  own_stone_ = own_stone;
}

StoneType Brain::own_stone() {
  return own_stone_;
}

StoneType Brain::opponent_stone() {
  return OppositeStone(own_stone_);
}

bool Brain::IsMyTurn(int count) {
  return count % 2 == 0;
}

StoneType Brain::TurnStone(int count) {
  if (IsMyTurn(count) == true) {
    return own_stone();
  } else {
    return opponent_stone();
  }
}

int Brain::Heuristic(Board board, int count) {
  int num = 0;
  for (int i = 4; i < 10; i++) {
    for (int j = 4; j < 10; j++) {
      if (board.stone(i, j) == own_stone()) {
        num++;
      }
    }
  }
  return num;
}

// If count is even, return Brain's max heuristic
// If count is odd,  return enemy's min heuristic
int Brain::MinMax(Board board, int count, bool alpha_beta) {
  Board virtual_board;
  int best_min_max, current_min_max;

  if (IsMyTurn(count) == true) {
    best_min_max = 0;
  } else {
    best_min_max = 100;
  }

  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      if (board.stone(i, j) != kStoneBlank || board.IsBannedPoint(i, j, TurnStone(count))) {
        continue;
      }

      virtual_board = board;
      virtual_board.set_stone(i, j, TurnStone(count));
      if (count == 0) {
        current_min_max = Heuristic(virtual_board, count);
      } else {
        current_min_max = MinMax(virtual_board, count - 1, alpha_beta);
      }

      if (IsMyTurn(count) == true) {
        if (current_min_max > best_min_max) {
          best_min_max = current_min_max;
        }
      } else {
        if (current_min_max < best_min_max) {
          best_min_max = current_min_max;
        }
      }
    }
  }
  return best_min_max;
}

Position Brain::GetMinMaxPoint(Board board, bool alpha_beta) {
  Position put_point;
  Board virtual_board;
  int max_min_max = 0, current_min_max;

  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      virtual_board = board;
      virtual_board.set_stone(i, j, own_stone());
      current_min_max = MinMax(virtual_board, 0, alpha_beta);
      if (current_min_max > max_min_max) {
        max_min_max = current_min_max;
        put_point = Position(i, j);
      }
    }
  }
  return put_point;
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
