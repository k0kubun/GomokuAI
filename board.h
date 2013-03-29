#ifndef BOARD_H_
#define BOARD_H_

#include <list>
#include "vector.h"
#include "line_point.h"

// You can decide rule by changing these consts
const int  kBoardSize = 15;
const bool kWhiteAllowed3x3  = true;
const bool kWhiteAllowed4x4  = true;
const bool kWhiteAllowedLong = true;
const bool kBlackAllowed3x3  = false;
const bool kBlackAllowed4x4  = false;
const bool kBlackAllowedLong = false;

enum StoneType {
  kStoneBlank = 0,
  kStoneBlack = 1,
  kStoneWhite = 2,
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
  
    LinePointIter PointBegin();
    LinePointIter PointEnd();
    void Append(int x, int y);
    void Append(LinePoint point);
    bool HasPoint(int x, int y);
    int  ContinuousLength();
    int  DiscontinuousLength();
    bool IsLine();
    Vector DirectionVector();
    void Sort();
    static bool ComparePoint(LinePoint point_a, LinePoint point_b);

    StoneType stone();
    void set_stone(StoneType stone);
 
   private:
    bool AreContinuousPoints(LinePointIter point_a, LinePointIter point_b);
    static Line LineMake(LinePoint point_a, LinePoint point_b);
    
    std::list<LinePoint> point_list_;
    StoneType stone_;
  };
  
  Board();

  bool HasWinner(StoneType stone);
  bool IsBannedPoint(int x, int y, StoneType stone);
  BannedReason GetBannedReason(int x, int y, StoneType stone);
  int  StoneNum();
  Line GetContinuousLineWithDirection(int x, int y, StoneType stone,
                                      Vector direction_vector);
  Line GetMaxLengthContinuousLine(int x, int y, StoneType stone);
  Line FindContinuousLineByLength(int length, StoneType stone);
  Line FindContinuousLineWithLengthAtLeast(int least_length, StoneType stone);
  Line FindDiscontinuousLine(int x, int y, StoneType stone);
  Line FindDiscontinuousLine(int length, StoneType stone);

  static bool IsInTheBoard(int x, int y) {
    return x >= 0 && x < kBoardSize && y >= 0 && y < kBoardSize;
  }
  static bool IsInTheBoard(LinePoint point) {
    return (point.x >= 0 && point.x < kBoardSize) &&
        (point.y >= 0 && point.y < kBoardSize);
  }
  
  StoneType stone(int x, int y);
  StoneType stone(LinePoint point);
  void set_stone(int x, int y, StoneType stone);
  
 private:
  StoneType stone_[kBoardSize][kBoardSize];
};

#endif
