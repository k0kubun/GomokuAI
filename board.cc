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

bool Board::IsBannedPoint(Position point, StoneType stone) {
  return this->IsBannedPoint(point.x, point.y, stone);
}

BannedReason Board::GetBannedReason(int x, int y, StoneType stone) {
  Board virtual_board(*this);
  Board::Line line;
  int num_of_discontinuous_length[kBoardSize + 1];
  int num_of_continuous_length[kBoardSize + 1];
  for (int i = 0; i < kBoardSize + 1; i++) {
    num_of_discontinuous_length[i] = 0;
    num_of_continuous_length[i] = 0;
  }

  virtual_board.set_stone(x, y, stone);
  for (int i = 0; i < kDirectionVectorNum; i++) {
    line = virtual_board.GetDiscontinuousLineWithDirection(
        x, y, stone, kDirectionVector[i]);
    if (line.IsAliveIn(*this)) {
      num_of_discontinuous_length[line.DiscontinuousLength()]++;
    }

    line = virtual_board.GetContinuousLineWithDirection(
        x, y, stone, kDirectionVector[i]);
    if (line.IsAliveIn(*this)) {
      num_of_continuous_length[line.ContinuousLength()]++;
    }
  }

  if (kAllowedLong[stone] == false) {
    for (int i = 6; i < kBoardSize + 1; i++) {
      if (num_of_continuous_length[i] != 0) {
        return kBanLong;
      }
    }
  }
  if (kAllowed3x3[stone] == false) {
    if (num_of_discontinuous_length[3] >= 2) {
      return kBan3x3;
    }
  }
  if (kAllowed4x4[stone] == false) {
    if (num_of_discontinuous_length[4] >= 2) {
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

int Board::MaxLineLength() {
  Line line;
  int max_length = 0, current_length;

  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      line = GetMaxLengthAliveDiscontinuousLine(i, j, kStoneBlack);
      current_length = line.DiscontinuousLength();
      if (max_length < current_length) {
        max_length = current_length;
      }

      line = GetMaxLengthAliveDiscontinuousLine(i, j, kStoneWhite);
      current_length = line.DiscontinuousLength();
      if (max_length < current_length) {
        max_length = current_length;
      }
    }
  }
  return max_length;
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

std::list<int> Board::GetAliveDiscontinuousLineLengthList(int x, int y,
                                                          StoneType stone) {
  Line current_line;
  std::list<int> length_list;
  for (int i = 0; i < kDirectionVectorNum; i++) {
    current_line =
        GetDiscontinuousLineWithDirection(x, y, stone, kDirectionVector[i]);
    if (current_line.IsAliveIn(*this)) {
      length_list.push_back(current_line.DiscontinuousLength());
    }
  }
  length_list.push_back(0); // Because brain access second member
  length_list.sort();
  length_list.reverse();
  return length_list;
}

Position Board::FindMultipleLineMakablePoint(int first_length,
                                             int second_length,
                                             StoneType stone) {
  Board virtual_board;
  Board::Line line;
  Position put_position = Position::Null();
  int max_length = -1;

  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      if (this->stone(i, j) == kStoneBlank &&
          this->IsBannedPoint(i, j, stone) == false) {
        virtual_board = *this;
        virtual_board.set_stone(i, j, stone);
        line = virtual_board.GetMaxLengthAliveDiscontinuousLine(i, j, stone);
        if (line.IsAliveIn(virtual_board) &&
            line.DiscontinuousLength() >= first_length) {
          std::list<int> length_list =
              virtual_board.GetAliveDiscontinuousLineLengthList(i, j, stone);
          std::list<int>::iterator list_iter = length_list.begin();
          list_iter++;
          if (second_length <= *list_iter) {
            max_length = *list_iter;
            put_position = Position(i, j);
          }
        }
      }
    }
  }
  return put_position;
}

Position Board::FindMultipleLinePreMakablePoint(int least_length,
                                                StoneType stone) {
  Board virtual_board;
  Board::Line line;

  Position point = virtual_board.
      FindMultipleLineMakablePoint(least_length, 3, stone);
  if (point.Exists()) {
    return Position::Null();
  }

  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      if (this->stone(i, j) == kStoneBlank) {
        virtual_board = *this;
        virtual_board.set_stone(i, j, stone);
        line = virtual_board.GetMaxLengthAliveDiscontinuousLine(i, j, stone);
        if (line.DiscontinuousLength() >= least_length) {
          virtual_board.set_stone(
              virtual_board.GetExtendPoint(line, OppositeStone(stone)),
              OppositeStone(stone));
          Position winning_point = virtual_board.
              FindMultipleLineMakablePoint(least_length, 3, stone);
          if (winning_point.Exists()) {
            return Position(i, j);
          }
        }
      }
    }
  }
  return Position::Null();
}

Position Board::GetExtendPoint(Board::Line line, StoneType put_stone) {
  if (line.IsContinuous() ||
      (line.IsContinuous() == false &&
       this->AllowsToPut(line.SplitPoint(), put_stone) == false)) {
    if (this->AllowsToPut(line.DirectionalBlank(), put_stone) &&
        (line.DiscontinuousLength() != 3 || // 長さ3の場合は2つ先をチェック
         this->IsInTheBoardAndBlank(line.DirectionalBlank(2)) == true)) {
      return line.DirectionalBlank();
    } else if (this->AllowsToPut(line.UndirectionalBlank(), put_stone) &&
               (line.DiscontinuousLength() != 3 || // 長さ3の場合は2つ先をチェック
                this->IsInTheBoardAndBlank(line.UndirectionalBlank(2)) == true)) {
      return line.UndirectionalBlank();
    } else {
      return Position::Null();
    }
  } else {
    return line.SplitPoint();
  }
  return Position::Null();
}

StoneType Board::StoneToPut() {
  if (NumOf(kStoneBlack) == NumOf(kStoneWhite)) {
    return kStoneBlack;
  } else {
    return kStoneWhite;
  }
}

Board Board::CopyWithPoint(int x, int y) {
  Board copy_board = *this;
  copy_board.set_stone(x, y, copy_board.StoneToPut());
  return copy_board;
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

StoneType Board::OppositeStone(StoneType stone) {
  if (stone == kStoneBlack) {
    return kStoneWhite;
  } else {
    return kStoneBlack;
  }
}

bool Board::AllowsToPut(Position point, StoneType stone) {
  return this->stone(point) == kStoneBlank &&
      point.IsInTheBoard() &&
      this->IsBannedPoint(point, stone) == false;
}

bool Board::IsInTheBoardAndBlank(Position point) {
  return this->stone(point) == kStoneBlank && point.IsInTheBoard();
}

Board::Line Board::GetContinuousLineWithDirection(Position point,
                                                  StoneType stone,
                                                  Vector direction) {
  Line line(stone);
  Position append_point;

  if (this->stone(point) == stone) {
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
  } else {
    return Line::Null();
  }
}

Board::Line Board::GetContinuousLineWithDirection(int x, int y, StoneType stone,
                                                  Vector direction) {
  return GetContinuousLineWithDirection(Position(x, y), stone, direction);
}

Board::Line Board::GetDiscontinuousLineWithDirection(int x, int y,
                                                     StoneType stone,
                                                     Vector direction) {
  Line main_line, split_line_a, split_line_b;
  Position directional_blank, undirectional_blank, split_point_a, split_point_b;

  if (this->stone(x, y) == stone) {
    main_line = GetContinuousLineWithDirection(x, y, stone, direction);

    directional_blank = main_line.DirectionalBlank(direction);
    if (directional_blank.IsInTheBoard() &&
        this->stone(directional_blank) == kStoneBlank) {
      split_point_a = directional_blank.MoveForDirection(direction);
      if (split_point_a.IsInTheBoard()) {
        split_line_a =
            GetContinuousLineWithDirection(split_point_a, stone, direction);
      }
    }

    undirectional_blank = main_line.UndirectionalBlank(direction);
    if (undirectional_blank.IsInTheBoard() &&
        this->stone(undirectional_blank) == kStoneBlank) {
      split_point_b = undirectional_blank.MoveAgainstDirection(direction);
      if (split_point_b.IsInTheBoard()) {
        split_line_b =
            GetContinuousLineWithDirection(split_point_b, stone, direction);
      }
    }

    if (split_line_a.ContinuousLength() > split_line_b.ContinuousLength()) {
      main_line.Append(split_line_a);
    } else if (split_line_a.ContinuousLength() < split_line_b.ContinuousLength()) {
      main_line.Append(split_line_b);
    }
    return main_line;
  } else {
    return Line::Null();
  }
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
