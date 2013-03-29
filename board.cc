#include "board.h"

Board::Board() {
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      stone_[i][j] = kStoneBlank;
    }
  }
}

bool Board::HasWinner(StoneType stone) {
  if (stone == kStoneBlack) {
    return FindContinuousLineByLength(5, stone).IsLine();
  } else {
    return FindContinuousLineWithLengthAtLeast(5, stone).IsLine();
  }
}

bool Board::IsBannedPoint(int x, int y, StoneType stone) {
  ;
}

BannedReason Board::GetBannedReason(int x, int y, StoneType stone) {
  Board virtual_board(*this);
  virtual_board.set_stone(x, y, stone);
  //TODO: ここからやる
}

int Board::StoneNum() {
  int stone_num = 0;
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      if (stone_[i][j] != kStoneBlank) {
        stone_num++;
      }
    }
  }  
  return stone_num;
}

Board::Line Board::GetContinuousLineWithDirection(int x, int y, StoneType stone,
                                                  Vector direction_vector) {
  bool appended_in_this_loop;  
  Line line(stone);
  LinePointIter current_point;
  LinePoint append_point;

  // TODO: 双方向に伸ばすループにする
  if (stone_[x][y] == stone) {
    line.Append(x, y);
    do {
      appended_in_this_loop = false;
      current_point = line.PointBegin();
      while (current_point != line.PointEnd()) {
        append_point.x = current_point->x + direction_vector.x;
        append_point.y = current_point->y + direction_vector.y;
        if (this->stone(append_point.x, append_point.y) == stone &&
            IsInTheBoard(append_point.x, append_point.y)) {
          if (line.HasPoint(append_point.x, append_point.y) == false) {
            line.Append(append_point.x, append_point.y);
            appended_in_this_loop = true;
          }
        }

        append_point.x = current_point->x - direction_vector.x;
        append_point.y = current_point->y - direction_vector.y;
        if (this->stone(append_point.x, append_point.y) == stone &&
            IsInTheBoard(append_point.x, append_point.y)) {
          if (line.HasPoint(append_point.x, append_point.y) == false) {
            line.Append(append_point.x, append_point.y);
            appended_in_this_loop = true;
          }
        }
        current_point++;
      }
    } while (appended_in_this_loop == true);
  }
  return line;
}

Board::Line Board::GetMaxLengthContinuousLine(int x, int y, StoneType stone) {
  Line current_line, longest_line;
  for (int i = 0; i < kDirectionVectorNum; i++) {
    current_line =
        GetContinuousLineWithDirection(x, y, stone, kDirectionVector[i]);
    if (current_line.ContinuousLength() > longest_line.ContinuousLength()) {
      longest_line = current_line;
    }
  }
  return longest_line;
}

Board::Line Board::FindContinuousLineByLength(int length, StoneType stone) {
  Line continuous_line(stone);
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      continuous_line = GetMaxLengthContinuousLine(i, j, stone);
      if (continuous_line.ContinuousLength() == length) {
        return continuous_line;
      }
    }
  }  
  return Line();
}

Board::Line Board::FindContinuousLineWithLengthAtLeast(int least_length,
                                                       StoneType stone) {
  Line continuous_line(stone);
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      continuous_line = GetMaxLengthContinuousLine(i, j, stone);
      if (continuous_line.ContinuousLength() >= least_length) {
        return continuous_line;
      }
    }
  }  
  return Line();
}

Board::Line Board::FindDiscontinuousLine(int length, StoneType stone) {
  Line discontinuous_line(stone);
  return discontinuous_line;
}

StoneType Board::stone(int x, int y) {
  return stone_[x][y];
}

void Board::set_stone(int x, int y, StoneType stone) {
  stone_[x][y] = stone;
}
