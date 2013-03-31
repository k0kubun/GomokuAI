#ifndef POSITION_H_
#define POSITION_H_

#include "rule.h"

class Position {
 public:
  Position() {
    valid_ = true;
  }
  Position(int point_x, int point_y) {
    x = point_x;
    y = point_y;
    valid_ = true;
  }
  Position(std::list<Position>::iterator point_iter) {
    x = point_iter->x;
    y = point_iter->y;
    valid_ = true;
  }
  Position MoveForDirection(Vector direction) {
    this->x += direction.x;
    this->y += direction.y;
    return *this;
  }
  Position MoveAgainstDirection(Vector direction) {
    return MoveForDirection(ReverseVector(direction));
  }
  bool IsInTheBoard() {
    return (x >= 0 && x < kBoardSize) && (y >= 0 && y < kBoardSize);        
  }
  Position SetInvalid() {
    valid_ = false;
    return *this;
  }
  bool IsValid() {
    return valid_;
  }
  
  int x;
  int y;

 private:
  bool valid_;
};

typedef std::list<Position>::iterator PositionIter;

#endif
