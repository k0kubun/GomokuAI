#include "board.h"

Board::Line::Line(StoneType stone) {
  stone_ = stone;  
}

LinePointIter Board::Line::PointBegin() {
  return point_list_.begin();
}

LinePointIter Board::Line::PointEnd() {
  return point_list_.end();
}

LinePoint Board::Line::PointFront() {
  return point_list_.front();
}

LinePoint Board::Line::PointBack() {
  return point_list_.back();
}

void Board::Line::Append(int x, int y) {
  this->Append(LinePoint(x, y));
}

void Board::Line::Append(LinePoint point) {  
  point_list_.push_back(point);
}

bool Board::Line::HasPoint(int x, int y) {
  LinePointIter current_point;
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
  LinePointIter current_point, prev_point;
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

bool Board::Line::IsLine() {
  return point_list_.size() > 1;
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
 
StoneType Board::Line::stone() {
  return stone_;
}
 
void Board::Line::set_stone(StoneType stone) {
  stone_ = stone;
}

bool Board::Line::ComparePoint(LinePoint point_a, LinePoint point_b) {
  Vector direction = LineMake(point_a, point_b).DirectionVector();

  if (direction.x == 1) {
    return point_a.x < point_b.x;
  } else {
    return point_a.y < point_b.y;
  }
}

LinePoint Board::Line::EdgeWithDirection(Vector direction) {
  this->Sort();
  if (direction.IsDirectionVector()) {
    return this->PointBack();
  } else {
    return this->PointFront();
  }
}

bool Board::Line::AreContinuousPoints(LinePointIter point_a,
                                      LinePointIter point_b) {
  return abs(point_a->x - point_b->x) < 2 && abs(point_a->y - point_b->y) < 2;
}

Board::Line Board::Line::LineMake(LinePoint point_a, LinePoint point_b) {
  Line line;
  line.Append(point_a.x, point_a.y);
  line.Append(point_b.x, point_b.y);
  return line;
}
