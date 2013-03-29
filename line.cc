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

void Board::Line::Append(int x, int y) {  
  LinePoint new_point;  
  new_point.x = x;
  new_point.y = y;
  point_list_.push_back(new_point);
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
    Vector direction_vector;
    direction_vector.x = (point_list_.begin())->x - (point_list_.end())->x;
    direction_vector.x /= abs(direction_vector.x);
    direction_vector.y = (point_list_.begin())->y - (point_list_.end())->y;
    direction_vector.y /= abs(direction_vector.y);

    // This method should return one of kDirectionVector[]
    if (direction_vector.y < 0 ||
        (direction_vector.x == -1 && direction_vector.y == 0)) {
      direction_vector = ReverseVector(direction_vector);
    }
    return direction_vector;
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
  Vector direction_vector = LineMake(point_a, point_b).DirectionVector();

  if (direction_vector.x == 1) {
    return point_a.x < point_b.x;
  } else {
    return point_a.y < point_b.y;
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
