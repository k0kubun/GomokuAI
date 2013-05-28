#ifndef BRAIN_H_
#define BRAIN_H_

#include "board.h"
#include "math.h"

enum {
  kHeuristicMin = 0,
  kHeuristicMax = 1000,
};

const int kRecursiveNum = 0;

class Brain {
 public:
  Brain(StoneType own_stone);

  void     PrintBoard(Board board);
  Position MiniMaxSearch(Board board);
  Position AlphaBetaPruning(Board board);
  Position GetPutPoint(Board board);

  //For Bench
  Position  GetSearchPoint(Board board, int count, bool alpha_beta);
  int       Heuristic(Board board);

  void      set_own_stone(StoneType own_stone);
  StoneType own_stone();
  StoneType opponent_stone();

 private:
  bool      IsPointToPut(Board board, int x, int y);
  int       HeuristicCenterHigh(Board board);
  //int       Heuristic(Board board, StoneType own_stone);
  //Position  GetSearchPoint(Board board, int count, bool alpha_beta);
  int       MiniMax(Board board, int depth);
  int       AlphaBeta(Board board, int depth, int cut);
  Position  GetEmptyPoint(Board board);
  StoneType OppositeStone(StoneType stone);

  StoneType own_stone_;
  int       cut_num_[kRecursiveNum];
};

#endif
