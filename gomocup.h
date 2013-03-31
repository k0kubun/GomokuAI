#ifndef GOMOCUP_H_
#define GOMOCUP_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include "board.h"

class Gomocup {
 public:
  Gomocup() {};

  void Start();
  void SetStone(int x, int y);
  void LetBrainPlay();
 
 private:
  Board board_;
};

#endif
