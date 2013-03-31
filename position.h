#ifndef LINE_POINT_H_
#define LINE_POINT_H_

#include "rule.h"

class LinePoint {
 public:
  LinePoint() {}  
  LinePoint(int point_x, int point_y) {
    x = point_x;
    y = point_y;
  }
  LinePoint MoveWithDirection(Vector direction_vector) {
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

typedef std::list<LinePoint>::iterator LinePointIter;

#endif
