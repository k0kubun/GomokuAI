#ifndef POSITION_H_
#define POSITION_H_

#include "rule.h"

class Position {
 public:
  Position() {}  
  Position(int point_x, int point_y) {
    x = point_x;
    y = point_y;
  }
  Position MoveWithDirection(Vector direction_vector) {
    this->x += direction_vector.x;
    this->y += direction_vector.y;
    return *this;
  }
  bool IsInTheBoard() {
    return (x >= 0 && x < kBoardSize) && (y >= 0 && y < kBoardSize);        
  }
  
  int x;
  int y;
};

typedef std::list<Position>::iterator PositionIter;

#endif
