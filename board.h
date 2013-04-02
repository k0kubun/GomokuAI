#ifndef BOARD_H_
#define BOARD_H_

#include <list>
#include "vector.h"
#include "position.h"
#include "rule.h"

enum StoneType {
  kStoneBlack = 0,
  kStoneWhite = 1,
  kStoneBlank = 2,
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
    bool Exists();
    static Line Null();
    bool IsLine();
    bool IsContinuous();
    int  BlankNumIn(Board board);
    bool IsAliveIn(Board board);
    Vector DirectionVector();
    void Sort();
    static bool ComparePoint(Position point_a, Position point_b);
    Position EdgeWithDirection(Vector direction);
    Position DirectionalEdge(Vector direction);
    Position UndirectionalEdge(Vector direction);
    Position DirectionalBlank();
    Position DirectionalBlank(Vector direction);
    Position UndirectionalBlank();
    Position UndirectionalBlank(Vector direction);
    Position SplitPoint();

    StoneType stone();
    void set_stone(StoneType stone);
    static Line LineMake(Position point_a, Position point_b);
 
   private:
    bool AreContinuousPoints(PositionIter point_a, PositionIter point_b);
    
    std::list<Position> point_list_;
    StoneType stone_;
  };
  
  Board();

  bool HasWinner();
  bool HasWinner(StoneType stone);
  bool IsBannedPoint(int x, int y, StoneType stone);
  bool IsBannedPoint(Position point, StoneType stone);
  bool AllowsToPut(Position point);
  BannedReason GetBannedReason(int x, int y, StoneType stone);
  int  StoneNum();
  int  NumOf(StoneType stone);
  int  MaxLineLength();
  Line FindAliveDiscontinuousLine(int length, StoneType stone);
  Line FindContinuousLineByLength(int length, StoneType stone);
  Line FindContinuousLineWithLengthAtLeast(int least_length);
  Line FindContinuousLineWithLengthAtLeast(int least_length, StoneType stone);
  Line GetMaxLengthAliveDiscontinuousLine(int x, int y, StoneType stone);
  std::list<int> GetAliveDiscontinuousLineLengthList(int x, int y,
                                                     StoneType stone);
  Position FindMultipleLineMakablePoint(int first_length, int second_length,
                                        StoneType stone);
  Position FindMultipleLineMakablePointMakablePoint(int least_length,
                                                    StoneType stone);
  Position GetExtendPoint(Line line);
  
  static bool IsInTheBoard(int x, int y) {
    return x >= 0 && x < kBoardSize && y >= 0 && y < kBoardSize;
  }
  static bool IsInTheBoard(Position point) {
    return IsInTheBoard(point.x, point.y);
  }
  
  StoneType stone(int x, int y);
  StoneType stone(Position point);
  void set_stone(int x, int y, StoneType stone);
  void set_stone(Position point, StoneType stone);
  
 protected:
  Line GetContinuousLineWithDirection(Position point, StoneType stone,
                                      Vector direction);  
  Line GetContinuousLineWithDirection(int x, int y, StoneType stone,
                                      Vector direction);
  Line GetDiscontinuousLineWithDirection(int x, int y, StoneType stone,
                                         Vector direction);
  Line GetMaxLengthContinuousLine(int x, int y, StoneType stone);
  
  StoneType stone_[kBoardSize][kBoardSize];
};

#endif
