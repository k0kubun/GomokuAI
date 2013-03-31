#ifndef BRAIN_H_
#define BRAIN_H_

#include "board.h"

class Brain {
 public:
  Brain(StoneType play_stone);
  
  void PutStone(Board board);
  Position GetPutPosition(Board board);

  StoneType play_stone();
  void set_play_stone(StoneType play_stone);
  
 private:
  Position GetEmptyPosition(Board board);

  StoneType play_stone_;
};

#endif
