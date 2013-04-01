#include "board.h"

Board::Board() {
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      stone_[i][j] = kStoneBlank;
    }
  }
}

bool Board::HasWinner() {
  return this->HasWinner(kStoneBlack) || this->HasWinner(kStoneWhite);
}

bool Board::HasWinner(StoneType stone) {
  if (kAllowedLong[stone]) {
    return FindContinuousLineWithLengthAtLeast(5, stone).IsLine();
  } else {
    return FindContinuousLineByLength(5, stone).IsLine();
  }
}

bool Board::IsBannedPoint(int x, int y, StoneType stone) {
  return GetBannedReason(x, y, stone) != kNoBan;
}

bool Board::AllowsToPut(Position point) {
  return this->stone(point) == kStoneBlank && point.IsInTheBoard();
}

BannedReason Board::GetBannedReason(int x, int y, StoneType stone) {
  Board virtual_board(*this);
  Board::Line line;
  int num_of_length[kBoardSize + 1];
  for (int i = 0; i < kBoardSize + 1; i++) {
    num_of_length[i] = 0;
  }
  
  virtual_board.set_stone(x, y, stone);  
  for (int i = 0; i < kDirectionVectorNum; i++) {
    line = GetDiscontinuousLineWithDirection(x, y, stone, kDirectionVector[i]);
    num_of_length[line.DiscontinuousLength()]++;
  }

  if (kAllowedLong[stone] == false) {
    for (int i = 6; i < kBoardSize + 1; i++) {
      if (num_of_length[i] != 0) {
        return kBanLong;
      }
    }
  }
  if (kAllowed3x3[stone] == false) {
    if (num_of_length[3] >= 2) {
      return kBan3x3;
    }
  }
  if (kAllowed4x4[stone] == false) {
    if (num_of_length[4] >= 2) {
      return kBan4x4;
    }
  }
  return kNoBan;
}

int Board::StoneNum() {
  return NumOf(kStoneBlack) + NumOf(kStoneWhite);
}

int Board::NumOf(StoneType stone) {
  int num = 0;
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      if (stone_[i][j] == stone) {
        num++;
      }
    }
  }
  return num;
}

Board::Line Board::FindAliveDiscontinuousLine(int length, StoneType stone) {
  Line line;
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      line = GetMaxLengthAliveDiscontinuousLine(i, j, stone);
      if (line.DiscontinuousLength() == length) {
        return line;
      }
    }
  }  
  return Line::Null();
}

Board::Line Board::FindContinuousLineByLength(int length, StoneType stone) {
  Line continuous_line;
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      continuous_line = GetMaxLengthContinuousLine(i, j, stone);
      if (continuous_line.ContinuousLength() == length) {
        return continuous_line;
      }
    }
  }
  return Line::Null();
}

Board::Line Board::FindContinuousLineWithLengthAtLeast(int least_length) {
  Line continuous_line;
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      continuous_line = GetMaxLengthContinuousLine(i, j, kStoneBlack);
      if (continuous_line.ContinuousLength() >= least_length) {
        return continuous_line;
      }
      continuous_line = GetMaxLengthContinuousLine(i, j, kStoneWhite);
      if (continuous_line.ContinuousLength() >= least_length) {
        return continuous_line;
      }
    }
  }
  return Line::Null();
}

Board::Line Board::FindContinuousLineWithLengthAtLeast(int least_length,
                                                       StoneType stone) {
  Line continuous_line;
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      continuous_line = GetMaxLengthContinuousLine(i, j, stone);
      if (continuous_line.ContinuousLength() >= least_length) {
        return continuous_line;
      }
    }
  }
  return Line::Null();
}

StoneType Board::stone(int x, int y) {
  return stone_[x][y];
}

StoneType Board::stone(Position point) {
  return stone(point.x, point.y);
}

void Board::set_stone(int x, int y, StoneType stone) {
  stone_[x][y] = stone;
}

void Board::set_stone(Position point, StoneType stone) {
  this->set_stone(point.x, point.y, stone);
}

Board::Line Board::GetContinuousLineWithDirection(Position point,
                                                  StoneType stone,
                                                  Vector direction) {
  Line line(stone);
  Position append_point;
  if (this->stone(point) == stone) {
    line.Append(point);
    append_point = point;
    while (append_point.MoveForDirection(direction).IsInTheBoard() &&
           this->stone(append_point) == stone) {
      line.Append(append_point);
    }
    append_point = point;
    while (append_point.MoveAgainstDirection(direction).IsInTheBoard() &&
           this->stone(append_point) == stone) {
      line.Append(append_point);
    }
  }
  return line;
}

Board::Line Board::GetContinuousLineWithDirection(int x, int y, StoneType stone,
                                                  Vector direction) {
  return GetContinuousLineWithDirection(Position(x, y), stone, direction);
}

Board::Line Board::GetDiscontinuousLineWithDirection(int x, int y,
                                                     StoneType stone,
                                                     Vector direction) {
  Line main_line, split_line_a, split_line_b;
  Position directional_blank, undirectional_blank;
  
  main_line = GetContinuousLineWithDirection(x, y, stone, direction);
  
  directional_blank = main_line.DirectionalBlank();
  if (this->stone(directional_blank) == kStoneBlank) {
    split_line_a = GetContinuousLineWithDirection(
        directional_blank.MoveForDirection(direction), stone, direction);
  }

  undirectional_blank = main_line.UndirectionalBlank();
  if (this->stone(undirectional_blank) == kStoneBlank) {
    split_line_b = GetContinuousLineWithDirection(
        undirectional_blank.MoveAgainstDirection(direction), stone, direction);
  }

  if (split_line_a.ContinuousLength() > split_line_b.ContinuousLength()) {
    main_line.Append(split_line_a);
  } else {
    main_line.Append(split_line_b);
  }
  return main_line;
}

Board::Line Board::GetMaxLengthContinuousLine(int x, int y, StoneType stone) {
  Line current_line, longest_line = Line::Null();
  for (int i = 0; i < kDirectionVectorNum; i++) {
    current_line =
        GetContinuousLineWithDirection(x, y, stone, kDirectionVector[i]);
    if (current_line.ContinuousLength() > longest_line.ContinuousLength()) {
      longest_line = current_line;
    }
  }
  return longest_line;
}

Board::Line Board::GetMaxLengthAliveDiscontinuousLine(int x, int y,
                                                 StoneType stone) {
  Line current_line, longest_line = Line::Null();
  for (int i = 0; i < kDirectionVectorNum; i++) {
    current_line =
        GetDiscontinuousLineWithDirection(x, y, stone, kDirectionVector[i]);
    
    if (current_line.IsAliveIn(*this) &&
        current_line.DiscontinuousLength() >
        longest_line.DiscontinuousLength()) {
      longest_line = current_line;
    }
  }
  return longest_line;
}
