#ifndef BRAIN_H_
#define BRAIN_H_

#include "board.h"

class Brain {
 public:
  Brain(StoneType own_stone);

  void     PrintBoard(Board board);
  Position MinMaxSearch(Board board);
  Position AlphaBetaPruning(Board board);
  Position GetPutPoint(Board board);

  void      set_own_stone(StoneType own_stone);
  StoneType own_stone();
  StoneType opponent_stone();

 private:
  bool      IsMyTurn(int count);
  StoneType TurnStone(int count);
  int       Heuristic(Board board, int count);
  int       MinMax(Board board, int count, bool alpha_beta);
  Position  GetMinMaxPoint(Board board, bool alpha_beta);
  Position  GetEmptyPoint(Board board);
  StoneType OppositeStone(StoneType stone);

  StoneType own_stone_;
};

#endif
