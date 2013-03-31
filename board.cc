#include "board.h"

Board::Board() {
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      stone_[i][j] = kStoneBlank;
    }
  }
}

bool Board::HasWinner(StoneType stone) {
  if (kAllowedLong[stone]) {
    return FindContinuousLineWithLengthAtLeast(5, stone).IsLine();
  } else {
    return FindContinuousLineByLength(5, stone).IsLine();
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

Board::Line Board::GetContinuousLineWithDirection(Position point,
                                                  StoneType stone,
                                                  Vector direction) {
  Line line(stone);
  Position append_point;
  if (this->stone(point) == stone) {
    line.Append(point);
    append_point = point;
    while (append_point.MoveWithDirection(direction).IsInTheBoard() &&
           this->stone(append_point) == stone) {
      line.Append(append_point);
    }
    append_point = point;
    direction = ReverseVector(direction);
    while (append_point.MoveWithDirection(direction).IsInTheBoard() &&
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
  bool appended_in_this_loop;
  Line main_line, split_line_for_direction, split_line_against_direction;
  Position split_point_for_direction, split_point_against_direction;
  main_line = GetContinuousLineWithDirection(x, y, stone, direction);
  split_point_for_direction = main_line.EdgeWithDirection(direction);
  split_point_for_direction.MoveWithDirection(direction);
  split_point_for_direction.MoveWithDirection(direction);
  split_line_for_direction = GetContinuousLineWithDirection(
      split_point_for_direction, stone, direction);

  split_point_against_direction =
      main_line.EdgeWithDirection(ReverseVector(direction));
  split_point_against_direction.MoveWithDirection(ReverseVector(direction));
  split_point_against_direction.MoveWithDirection(ReverseVector(direction));
  split_line_against_direction = GetContinuousLineWithDirection(
      split_point_against_direction, stone, direction);

  if (split_line_for_direction.ContinuousLength() >
      split_line_against_direction.ContinuousLength()) {
    main_line.Append(split_line_for_direction);
  } else {
    main_line.Append(split_line_against_direction);
  }
  return main_line;
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

StoneType Board::stone(Position point) {
  return stone(point.x, point.y);
}

void Board::set_stone(int x, int y, StoneType stone) {
  stone_[x][y] = stone;
}
