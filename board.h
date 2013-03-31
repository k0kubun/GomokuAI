#ifndef BOARD_H_
#define BOARD_H_

#include <list>
#include "vector.h"
#include "position.h"
#include "rule.h"

enum StoneType {
  kStoneBlank = -1,
  kStoneBlack = 0,
  kStoneWhite = 1,
};

enum BannedReason {
  kNoBan   = 0,
  kBan3x3  = 1,
  kBan4x4  = 2,
  kBanLong = 3,
};

const int kDirectionHorizontal = 0;
const int kDirection45Degree   = 1;
const int kDirectionVertical   = 2;
const int kDirection135Degree  = 3;
const int kDirectionVectorNum  = 4;
const Vector kDirectionVector[kDirectionVectorNum] = {
  {  1, 0 }, // kDirectionHorizontal
  {  1, 1 }, // kDirection45Degree
  {  0, 1 }, // kDirectionVertical
  { -1, 1 }, // kDirection135Degree
};

class Board {
 public:
  class Line {
   public:
    Line() {};
    Line(StoneType stone);
  
    PositionIter PointBegin();
    PositionIter PointEnd();
    Position PointFront();
    Position PointBack();
    void Append(Position point);
    void Append(int x, int y);
    void Append(Line line);
    bool HasPoint(Position point);
    bool HasPoint(int x, int y);
    int  ContinuousLength();
    int  DiscontinuousLength();
    bool IsLine();
    Vector DirectionVector();
    void Sort();
    static bool ComparePoint(Position point_a, Position point_b);
    Position EdgeWithDirection(Vector direction);

    StoneType stone();
    void set_stone(StoneType stone);
    static Line LineMake(Position point_a, Position point_b);
 
   private:
    bool AreContinuousPoints(PositionIter point_a, PositionIter point_b);
    
    std::list<Position> point_list_;
    StoneType stone_;
  };
  
  Board();

  bool HasWinner(StoneType stone);
  bool IsBannedPoint(int x, int y, StoneType stone);
  BannedReason GetBannedReason(int x, int y, StoneType stone);
  int  StoneNum();
  Line GetContinuousLineWithDirection(Position point, StoneType stone,
                                      Vector direction);  
  Line GetContinuousLineWithDirection(int x, int y, StoneType stone,
                                      Vector direction);
  Line GetDiscontinuousLineWithDirection(int x, int y, StoneType stone,
                                         Vector direction);
  Line GetMaxLengthContinuousLine(int x, int y, StoneType stone);
  Line FindContinuousLineByLength(int length, StoneType stone);
  Line FindContinuousLineWithLengthAtLeast(int least_length, StoneType stone);
  Line FindDiscontinuousLine(int x, int y, StoneType stone);
  Line FindDiscontinuousLine(int length, StoneType stone);

  static bool IsInTheBoard(int x, int y) {
    return x >= 0 && x < kBoardSize && y >= 0 && y < kBoardSize;
  }
  static bool IsInTheBoard(Position point) {
    return (point.x >= 0 && point.x < kBoardSize) &&
        (point.y >= 0 && point.y < kBoardSize);
  }
  
  StoneType stone(int x, int y);
  StoneType stone(Position point);
  void set_stone(int x, int y, StoneType stone);
  
 private:
  StoneType stone_[kBoardSize][kBoardSize];
};

#endif
