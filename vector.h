#ifndef VECTOR_H_
#define VECTOR_H_

class Vector {
 public:
  int x;
  int y;
};

static inline Vector VectorMake(int x, int y) {
  Vector v;
  v.x = x;
  v.y = y;
  return v;
}

static inline Vector ReverseVector(Vector vector) {
  vector.x *= -1;
  vector.y *= -1;
  return vector;
}

#endif
