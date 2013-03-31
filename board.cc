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
  return false;
}

// BannedReason Board::GetBannedReason(int x, int y, StoneType stone) {
//   Board virtual_board(*this);
//   virtual_board.set_stone(x, y, stone);
//   //TODO: ここからやる
// }

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

Board::Line Board::FindAliveDiscontinuousLine(int length, StoneType stone) {
  Line line;
  for (int i = 0; i < kBoardSize; i++) {
    for (int j = 0; j < kBoardSize; j++) {
      line = GetMaxLengthDiscontinuousLine(i, j, stone);
      if (line.DiscontinuousLength() == length) {
        return line;
      }
    }
  }  
  return line;
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
  return Line();
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
  return Line();
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
  Position directional_edge, undirectional_edge;
  
  main_line = GetContinuousLineWithDirection(x, y, stone, direction);
  
  directional_edge = main_line.DirectionalEdge();
  directional_edge.MoveForDirection(direction);
  if (this->stone(directional_edge) == kStoneBlank) {
    directional_edge.MoveForDirection(direction);
    split_line_a = GetContinuousLineWithDirection(directional_edge,
                                                  stone, direction);
  }

  undirectional_edge = main_line.UndirectionalEdge();
  undirectional_edge.MoveAgainstDirection(direction);
  if (this->stone(undirectional_edge) == kStoneBlank) {
    undirectional_edge.MoveAgainstDirection(direction);
    split_line_b = GetContinuousLineWithDirection(undirectional_edge,
                                                  stone, direction);
  }

  if (split_line_a.ContinuousLength() > split_line_b.ContinuousLength()) {
    main_line.Append(split_line_a);
  } else {
    main_line.Append(split_line_b);
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

Board::Line Board::GetMaxLengthDiscontinuousLine(int x, int y,
                                                 StoneType stone) {
  Line current_line, longest_line;
  for (int i = 0; i < kDirectionVectorNum; i++) {
    current_line =
        GetDiscontinuousLineWithDirection(x, y, stone, kDirectionVector[i]);
    if (current_line.DiscontinuousLength() > longest_line.DiscontinuousLength()) {
      longest_line = current_line;
    }
  }
  return longest_line;
}
