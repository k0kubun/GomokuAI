
#include "stdlib.h"
#include "string.h"
#include "time.h"
#import "mt19937ar.h"

#define LEVEL_PUZZLE_MODE 1
#define LEVEL_DEFENCE_ONLY 0
#define LEVEL_MAX 2
#define RECURSIVE_LIMIT 3

#define BOARD_SIZE 15

#define S_BLANK 0
#define S_BLACK 1
#define S_WHITE 2

#define BAN_3_3 3
#define BAN_4_4 4
#define BAN_LONG 5

typedef struct _POINT {
    int x;
    int y;
} POINT, VECTOR;

typedef struct _POINTLIST {
    int x;
    int y;
    struct _POINTLIST* next;
} POINTLIST;

typedef struct _GOMOKU {
    int board[BOARD_SIZE][BOARD_SIZE];
    int numOfStone;
    int playStoneType;
    int pid;
    int rating;
    int author;
    POINTLIST* whiteStoneHistory;
    POINTLIST* blackStoneHistory;
} GOMOKU;

int getPlayStoneType(GOMOKU* puzzle);
void printBoard(GOMOKU* puzzle);
void sendPuzzleDataWithDateString(NSString* dateString, int rating);
GOMOKU* getBoardDataFromID(int stageID);
POINTLIST* getKillPointListOf3LineWithoutSplitPoint(GOMOKU* puzzle, POINTLIST* p3Line, int lineStoneType);
bool isEndPointToMakePointListWith4Line(GOMOKU* puzzle, POINTLIST* pointList, int x, int y, int putStoneType);
int numOfStoneOnTheBoard(GOMOKU* puzzle);
int numOfCertainTypeStoneOnTheBoard(GOMOKU* puzzle, int stoneType);
bool isEndPointToMakePointListWith3Line(GOMOKU* puzzle, POINTLIST* pointList, int x, int y, int putStoneType);
bool isAbleToBeInterruptedLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType, int minimumLength);
bool isAliveLinePoint(GOMOKU* puzzle, int x, int y, int lineStoneType, int minimumLength);
POINT* getPointToMakeFinalStyleWith4LineRecursive(GOMOKU* puzzle, int putStoneType);
POINT* getPointToMakeFinalStyleWith3LineRecursive(GOMOKU* puzzle, int putStoneType);
POINT* getPointToMakePointListWith4LineRecursieve(GOMOKU* puzzle, POINTLIST* pointList, int putStoneType);
POINT* getPointToMakePointListWith3LineRecursieve(GOMOKU* puzzle, POINTLIST* pointList, int putStoneType);
bool isPointToMakePointList(GOMOKU* puzzle, POINTLIST* pointList, int x, int y, int putStoneType, int minimumLength);
bool isInPointList(POINTLIST* pointList, int x, int y);
int getStarNum(int stageNum, int stoneRecord);
void freePuzzle(GOMOKU* puzzle);
GOMOKU initBoard(int stage);
void addStoneHistory(GOMOKU* puzzle, int x, int y, int addStoneType);
void removeStoneHistory(GOMOKU* puzzle, int removeStoneType);
POINTLIST* newPointList();
POINTLIST* getIfSameTypeStone(GOMOKU* puzzle, int x, int y, int getStoneType);
bool isPointToPrepareToMakeFinalStyleWith4Line(GOMOKU* puzzle, int x, int y, int putStoneType);
bool isPointToPrepareToMakeFinalStyleWith3Line(GOMOKU* puzzle, int x, int y, int putStoneType);
bool appendPointList(POINTLIST* destList, POINTLIST* newPoint);
POINTLIST* getMaxLengthLine(GOMOKU* puzzle, int x, int y, int lineStoneType);
POINTLIST* getFiveLine(GOMOKU* puzzle);
POINTLIST* getVerticalLine(GOMOKU* puzzle, int x, int y, int lineStoneType);
POINTLIST* get45DegreeLine(GOMOKU* puzzle, int x, int y, int lineStoneType);
POINTLIST* getHorizontalLine(GOMOKU* puzzle, int x, int y, int lineStoneType);
POINTLIST* get135DegreeLine(GOMOKU* puzzle, int x, int y, int lineStoneType);
POINTLIST* getSplitVerticalLine(GOMOKU* puzzle, int x, int y, int lineStoneType);
POINTLIST* getSplit45DegreeLine(GOMOKU* puzzle, int x, int y, int lineStoneType);
POINTLIST* getSplitHorizontalLine(GOMOKU* puzzle, int x, int y, int lineStoneType) ;
POINTLIST* getSplit135DegreeLine(GOMOKU* puzzle, int x, int y, int lineStoneType);
POINTLIST* getLineWithOrientation(GOMOKU* puzzle, int x, int y, int x_orientation, int y_orientation, int lineStoneType);
POINT* getEndOfLineWithOffset(GOMOKU* puzzle, POINTLIST* line, int x_orientation, int y_orientation, int lineStoneType);
bool isSplitLine(POINTLIST* pLine);
POINTLIST* getLineAbleToMake4WithOrientation(GOMOKU* puzzle, int x, int y, int x_orientation, int y_orientation, int lineStoneType);
POINTLIST* getSplitLineWithOrientation(GOMOKU* puzzle, int x, int y, int x_orientation, int y_orientation, int lineStoneType);
int lengthOfPointList(POINTLIST* pointList);
void freePointList(POINTLIST* pointList);
POINTLIST* findSplit4Line(GOMOKU* puzzle, int x, int y, int lineStoneType);
POINTLIST* findSplit3Line(GOMOKU* puzzle, int x, int y, int lineStoneType);
POINTLIST* findLineAbleToMake4(GOMOKU* puzzle, int x, int y, int lineStoneType);
POINTLIST* findAliveSplitLine(GOMOKU* puzzle, int x, int y, int lineStoneType, int lineLength);
int compareXPoint(const POINTLIST** a, const POINTLIST** b);
int compareYPoint(const POINTLIST** a, const POINTLIST** b);
void sortLine(POINTLIST* pLine);
int maxSubOfTwoPoint(int x1, int y1, int x2, int y2);
POINT* getSplitPoint(POINTLIST* pLine);
VECTOR* getLineOrientationVector(POINTLIST* line);
bool isInTheBoard(int x, int y);
POINT* findBlankBesideLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType);
int numOfAlivePoint(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType);
POINTLIST* getBlankPointListBesideLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType);
POINT* findAlivePointBesideLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType);
int numOf2BlankPointListBesideLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType);
POINTLIST* get2BlankPointListBesideLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType);
POINT* getKillPointOf4Line(GOMOKU* puzzle, POINTLIST* p4Line, int lineStoneType);
POINT* getKillPointOf3Line(GOMOKU* puzzle, POINTLIST* p3Line, int lineStoneType);
POINT* findAlivePointOf2Line(GOMOKU* puzzle, POINTLIST* p2Line, int lineStoneType);
POINTLIST* getKillPointListOf3Line(GOMOKU* puzzle, POINTLIST* p3Line, int lineStoneType);

int maxLineLengthOf(GOMOKU* puzzle, int x, int y, int lineStoneType);
bool existFiveLine(GOMOKU* puzzle, int lineStoneType);
void putStone(GOMOKU* puzzle, int x, int y, int putStoneType);
GOMOKU* copyPuzzle(GOMOKU* puzzle);
int isBannedPoint(GOMOKU* puzzle, int x, int y, int putStoneType);
void oneStepBack(GOMOKU* puzzle, int removeStoneType);
int stoneNum(GOMOKU* puzzle);

POINT* getPointToMake5Line(GOMOKU* puzzle, int putStoneType);
int getOppositeStone(int stoneType);
POINT* getPointToKill4Line(GOMOKU* puzzle, int putStoneType);
POINT* getPointToMake4Line(GOMOKU* puzzle, int putStoneType);
POINT* getPointToKillAlive3Line(GOMOKU* puzzle, int putStoneType);

bool isAliveLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType, bool check2Line);
bool isFinalStylePoint(GOMOKU* puzzle, int x, int y, int killedStoneType, bool bAllow3x3);

POINT* getPointToKillFinalStyle(GOMOKU* puzzle, int putStoneType);
POINT* getPointToMake3Line(GOMOKU* puzzle, int putStoneType);
POINT* getPointToPrepareToMakeFinalStyleWith4Line(GOMOKU* puzzle, int putStoneType);
POINT* getPointToPrepareToMakeFinalStyleWith3Line(GOMOKU* puzzle, int putStoneType);
POINT* getPointToMakeFinalStyle(GOMOKU* puzzle, int putStoneType, bool bAllow3x3);
POINT* getPointToKillDead3Line(GOMOKU* puzzle, int putStoneType);
POINT* getPointWithoutThought(GOMOKU* puzzle);
POINT* getAIPutPoint(GOMOKU* puzzle, int putStoneType, bool* bAbleToWin, int AILevel);
bool letAIPutStone(GOMOKU* puzzle, int putStoneType, int AILevel);
POINT* getCenterPointOfTheBoard();
POINT* getPointAroundCenterWithMT();

