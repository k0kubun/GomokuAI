#include <stdlib.h>
#include "board.h"

Board::Line::Line(StoneType stone) {
  stone_ = stone;
}

PositionIter Board::Line::PointBegin() {
  return point_list_.begin();
}

PositionIter Board::Line::PointEnd() {
  return point_list_.end();
}

Position Board::Line::PointFront() {
  return point_list_.front();
}

Position Board::Line::PointBack() {
  return point_list_.back();
}

void Board::Line::Append(Position point) {
  point_list_.push_back(point);
}

void Board::Line::Append(int x, int y) {
  this->Append(Position(x, y));
}

void Board::Line::Append(Line line) {
  PositionIter current_point = line.PointBegin();
  while (current_point != line.PointEnd()) {
    if (this->HasPoint(*current_point) == false) {
      this->Append(*current_point);
    }
    current_point++;
  }
}

bool Board::Line::HasPoint(Position point) {
  return this->HasPoint(point.x, point.y);
}

bool Board::Line::HasPoint(int x, int y) {
  PositionIter current_point;
  current_point = this->PointBegin();
  while (current_point != this->PointEnd()) {
    if (current_point->x == x && current_point->y == y) {
      return true;
    }
    current_point++;
  }
  return false;
}

int Board::Line::ContinuousLength() {
  PositionIter current_point, prev_point;
  int current_length, max_length;
  if (this->IsLine()) {
    this->Sort();
    current_point = this->PointBegin();
    while (current_point != this->PointEnd()) {
      if (current_point == this->PointBegin()) {
        current_length = 1;
        max_length = 1;
      } else {
        if (AreContinuousPoints(current_point, prev_point)) {
          current_length++;
        } else {
          current_length = 1;
        }
        if (max_length < current_length) {
          max_length = current_length;
        }
      }
      prev_point = current_point;
      current_point++;
    }
    return max_length;
  } else {
    return point_list_.size();
  }
}

int Board::Line::DiscontinuousLength() {
  return point_list_.size();
}

bool Board::Line::Exists() {
  return this->stone() != kStoneBlank;
}

Board::Line Board::Line::Null() {
  return Line(kStoneBlank);
}

bool Board::Line::IsLine() {
  return point_list_.size() > 1;
}

bool Board::Line::IsContinuous() {
  PositionIter current_point, prev_point;
  this->Sort();

  current_point = this->PointBegin();
  while (current_point != this->PointEnd()) {
    if (current_point != this->PointBegin()) {
      if (AreContinuousPoints(current_point, prev_point) == false) {
        return false;
      }
    }
    prev_point = current_point;
    current_point++;
  }
  return true;
}

int Board::Line::BlankNumIn(Board board) {
  int blank_num = 0;
  if (this->IsLine()) {
    if (board.stone(this->DirectionalBlank()) == kStoneBlank) {
      blank_num++;
    }
    if (board.stone(this->UndirectionalBlank()) == kStoneBlank) {
      blank_num++;
    }
  }
  return blank_num;
}

bool Board::Line::IsAliveIn(Board board) {
  Position split_point = this->SplitPoint();
  if (split_point.Exists()) {
    if (board.stone(split_point) != kStoneBlank) {
      return false;
    }
  }
  if (this->DiscontinuousLength() >= 5) {
    return true;
  } else if (this->DiscontinuousLength() == 4) {
    if (split_point.Exists()) {
      return true;
    } else {
      if (this->BlankNumIn(board) > 0) {
        return true;
      }
    }
  } else if (this->DiscontinuousLength() <= 3) {
    if (this->BlankNumIn(board) == 2) {
      return true;
    }
  }
  return false;
}

Vector Board::Line::DirectionVector() {
  if (this->IsLine()) {
    Vector direction;
    direction.x = this->PointFront().x - this->PointBack().x;
    if (direction.x != 0) {
      direction.x /= abs(direction.x);
    }
    direction.y = this->PointFront().y - this->PointBack().y;
    if (direction.y != 0) {
      direction.y /= abs(direction.y);
    }

    if (direction.IsDirectionVector() == false) {
      direction = ReverseVector(direction);
    }
    return direction;
  } else {
    return VectorMake(0, 0);
  }
}

void Board::Line::Sort() {
  if (this->IsLine()) {
    point_list_.sort(Line::ComparePoint);
  }
}

bool Board::Line::ComparePoint(Position point_a, Position point_b) {
  Vector direction = LineMake(point_a, point_b).DirectionVector();

  if (direction.x == 1) {
    return point_a.x < point_b.x;
  } else {
    return point_a.y < point_b.y;
  }
}

Position Board::Line::EdgeWithDirection(Vector direction) {
  this->Sort();
  if (direction.IsDirectionVector()) {
    return this->PointBack();
  } else {
    return this->PointFront();
  }
}

Position Board::Line::DirectionalEdge(Vector direction) {
  return this->EdgeWithDirection(direction);
}

Position Board::Line::UndirectionalEdge(Vector direction) {
  return this->EdgeWithDirection(ReverseVector(direction));
}

Position Board::Line::DirectionalBlank() {
  Vector direction = this->DirectionVector();
  return this->DirectionalEdge(direction).MoveForDirection(direction);
}

Position Board::Line::DirectionalBlank(int blank_num) {
  Vector direction = this->DirectionVector();
  Position destination = this->DirectionalEdge(direction);
  for (int i = 0; i < blank_num; i++) {
    destination = destination.MoveForDirection(direction);
  }
  return destination;
}

Position Board::Line::DirectionalBlank(Vector direction) {
  return this->DirectionalEdge(direction).MoveForDirection(direction);
}

Position Board::Line::UndirectionalBlank() {
  Vector direction = this->DirectionVector();
  return this->UndirectionalEdge(direction).MoveAgainstDirection(direction);
}

Position Board::Line::UndirectionalBlank(int blank_num) {
  Vector direction = this->DirectionVector();
  Position destination = this->UndirectionalEdge(direction);
  for (int i = 0; i < blank_num; i++) {
    destination = destination.MoveAgainstDirection(direction);
  }
  return destination;
}

Position Board::Line::UndirectionalBlank(Vector direction) {
  return this->UndirectionalEdge(direction).MoveAgainstDirection(direction);
}

Position Board::Line::SplitPoint() {
  PositionIter current_point, prev_point;
  this->Sort();

  current_point = this->PointBegin();
  while (current_point != this->PointEnd()) {
    if (current_point != this->PointBegin()) {
      if (AreContinuousPoints(current_point, prev_point) == false) {
        return Position(prev_point).MoveForDirection(this->DirectionVector());
      }
    }
    prev_point = current_point;
    current_point++;
  }
  return Position::Null();
}

StoneType Board::Line::stone() {
  return stone_;
}

void Board::Line::set_stone(StoneType stone) {
  stone_ = stone;
}

Board::Line Board::Line::LineMake(Position point_a, Position point_b) {
  Line line;
  line.Append(point_a.x, point_a.y);
  line.Append(point_b.x, point_b.y);
  return line;
}

bool Board::Line::AreContinuousPoints(PositionIter point_a,
                                      PositionIter point_b) {
  return abs(point_a->x - point_b->x) < 2 && abs(point_a->y - point_b->y) < 2;
}
