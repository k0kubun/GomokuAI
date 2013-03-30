
void addStoneHistory(GOMOKU* puzzle, int x, int y, int addStoneType) {
  POINTLIST* newStonePoint = newPointList();
    
  newStonePoint->x = x;
  newStonePoint->y = y;

  if (addStoneType == S_BLACK) {
    newStonePoint->next = puzzle->blackStoneHistory;
    puzzle->blackStoneHistory = newStonePoint;
  } else {
    newStonePoint->next = puzzle->whiteStoneHistory;
    puzzle->whiteStoneHistory = newStonePoint;
  }
}

void removeStoneHistory(GOMOKU* puzzle, int removeStoneType) {
  POINTLIST* removeStonePoint;
    
  if (removeStoneType == S_BLACK) {
    if (puzzle->blackStoneHistory != NULL) {
      removeStonePoint = puzzle->blackStoneHistory;
      puzzle->blackStoneHistory = puzzle->blackStoneHistory->next;
      free(removeStonePoint);
    }
  } else {
    if (puzzle->whiteStoneHistory != NULL) {
      removeStonePoint = puzzle->whiteStoneHistory;
      puzzle->whiteStoneHistory = puzzle->whiteStoneHistory->next;
      free(removeStonePoint);
    }
  }
}

void oneStepBack(GOMOKU* puzzle, int removeStoneType) {
  if (removeStoneType == S_BLACK) {
    if (puzzle->blackStoneHistory != NULL) {
      puzzle->board[puzzle->blackStoneHistory->x][puzzle->blackStoneHistory->y] = S_BLANK;
      removeStoneHistory(puzzle, removeStoneType);
    }
  } else {
    if (puzzle->whiteStoneHistory != NULL) {
      puzzle->board[puzzle->whiteStoneHistory->x][puzzle->whiteStoneHistory->y] = S_BLANK;
      removeStoneHistory(puzzle, removeStoneType);
    }
  }
}

POINTLIST* newPointList() {
  POINTLIST* newStonePoint = (POINTLIST *)malloc(sizeof(POINTLIST));
  newStonePoint->next = NULL;
    
  return newStonePoint;
}

POINTLIST* getIfSameTypeStone(GOMOKU* puzzle, int x, int y, int getStoneType) {
  POINTLIST* newStonePoint;
    
  if (x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE) {
    if (puzzle->board[x][y] == getStoneType) {
      newStonePoint = newPointList();
      newStonePoint->x = x;
      newStonePoint->y = y;
            
      return newStonePoint;
    }
  }
    
  return NULL;
}

bool appendPointList(POINTLIST* destList, POINTLIST* newPoint) {
  POINTLIST* currentPointList = destList;
    
  if (newPoint == NULL) {
    return false;
  }
    
  if (destList == NULL) {
    destList = newPoint;
    return true;
  }
    
  while (currentPointList->next != NULL) {
    //destListにnewPointが含まれていたらappendしない
    if (currentPointList->x == newPoint->x && currentPointList->y == newPoint->y) {
      return false;
    }        
    currentPointList = currentPointList->next;
  }
    
  //最後の要素を上のループではチェックできないのでここでやる
  if (currentPointList->x == newPoint->x && currentPointList->y == newPoint->y) {
    return false;
  }
    
  if (newPoint != NULL) {
    currentPointList->next = newPoint;
    return true;
  } else {
    return false;
  }
}

POINTLIST* getVerticalLine(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  return getLineWithOrientation(puzzle, x, y, 0, 1, lineStoneType);
}


POINTLIST* get45DegreeLine(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  return getLineWithOrientation(puzzle, x, y, 1, 1, lineStoneType);
}

POINTLIST* getHorizontalLine(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  return getLineWithOrientation(puzzle, x, y, 1, 0, lineStoneType);
}

POINTLIST* get135DegreeLine(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  return getLineWithOrientation(puzzle, x, y, 1, -1, lineStoneType);
}

POINTLIST* getSplitVerticalLine(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  return getSplitLineWithOrientation(puzzle, x, y, 0, 1, lineStoneType);
}

POINTLIST* getSplit45DegreeLine(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  return getSplitLineWithOrientation(puzzle, x, y, 1, 1, lineStoneType);
}

POINTLIST* getSplitHorizontalLine(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  return getSplitLineWithOrientation(puzzle, x, y, 1, 0, lineStoneType);
}

POINTLIST* getSplit135DegreeLine(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  return getSplitLineWithOrientation(puzzle, x, y, 1, -1, lineStoneType);
}


POINTLIST* getLineWithOrientation(GOMOKU* puzzle, int x, int y, int x_orientation, int y_orientation, int lineStoneType) {
  bool bAppend;
  POINTLIST* currentPointList;
  POINTLIST* headOfPointList = NULL;
    
  if (puzzle->board[x][y] == lineStoneType) {
    headOfPointList = newPointList();
    headOfPointList->x = x;
    headOfPointList->y = y;
        
    do {
      bAppend = false;
      currentPointList = headOfPointList;
      do {
        if (appendPointList(headOfPointList, getIfSameTypeStone(puzzle, currentPointList->x + x_orientation, currentPointList->y + y_orientation, lineStoneType)) == true) {
          bAppend = true;
        }
        if (appendPointList(headOfPointList, getIfSameTypeStone(puzzle, currentPointList->x - x_orientation, currentPointList->y - y_orientation, lineStoneType)) == true) {
          bAppend = true;
        }
      } while ( (currentPointList = currentPointList->next) != NULL );
            
    } while (bAppend == true);
  }
    
  return headOfPointList;
}

//offsetベクトル側の端の座標を取得する
POINT* getEndOfLineWithOffset(GOMOKU* puzzle, POINTLIST* line, int x_orientation, int y_orientation, int lineStoneType) {
  POINTLIST* currentPoint;
  VECTOR* endOfLine;
  if (line == NULL) {
    return NULL;
  } else {
    endOfLine = (POINT *)malloc(sizeof(POINT));
    //とりあえず最初の要素をいれておく
    endOfLine->x = line->x;
    endOfLine->y = line->y;
  }
    
  currentPoint = line;
  while ( (currentPoint = currentPoint->next) != NULL ) {
    //offsetベクトルと、endOfLineからcurrentPointへのベクトルの内積が正なら変える
    //でも正直ちゃんと線にそったもので取得するなら別に内積計算するまでもないかも？
    POINT currentVector;
    currentVector.x = currentPoint->x - endOfLine->x;
    currentVector.y = currentPoint->y - endOfLine->y;
        
    int innerProduct = x_orientation * currentVector.x + y_orientation * currentVector.y;
    if (innerProduct > 0) {
      endOfLine->x = currentPoint->x;
      endOfLine->y = currentPoint->y;
    }
  }
    
  return endOfLine;
}

bool isSplitLine(POINTLIST* pLine) {
  POINT* splitPoint;
    
  if (pLine != NULL) {
    splitPoint = getSplitPoint(pLine);
    if (splitPoint != NULL) {
      return true;
    } else {
      return false;
    }
  }
  return false;
}

POINTLIST* getLineAbleToMake4WithOrientation(GOMOKU* puzzle, int x, int y, int x_orientation, int y_orientation, int lineStoneType) {
  POINTLIST* pointList;
  POINT* orientationEndOfLine;
  POINT* oppositeOrientationEndOfLine;
  POINT orientationBlankPoint;
  POINT oppositeOrientationBlankPoint;
  POINT orientation2BlankPoint;
  POINT oppositeOrientation2BlankPoint;
  POINT orientation3BlankPoint;
  POINT oppositeOrientation3BlankPoint;
  int lineLength;
  int stoneTypeOrientationBlank, stoneTypeOppositeOrientationBlank, stoneTypeOrientation2Blank, stoneTypeOppositeOrientation2Blank,
    stoneTypeOrientation3Blank, stoneTypeOppositeOrientation3Blank;
    
  POINTLIST* mainLine = NULL;
    
  if (puzzle->board[x][y] != S_BLANK) {
    mainLine = getLineWithOrientation(puzzle, x, y, x_orientation, y_orientation, lineStoneType);
    if (mainLine != NULL) {
      lineLength = lengthOfPointList(mainLine);
            
      //mainLineの前後3マスずつの石や空白の状態を取得しているだけ
            
      orientationEndOfLine = getEndOfLineWithOffset(puzzle, mainLine, x_orientation, y_orientation, lineStoneType);
      oppositeOrientationEndOfLine = getEndOfLineWithOffset(puzzle, mainLine, -x_orientation, -y_orientation, lineStoneType);
            
      orientationBlankPoint.x = orientationEndOfLine->x + x_orientation;
      orientationBlankPoint.y = orientationEndOfLine->y + y_orientation;
      oppositeOrientationBlankPoint.x = oppositeOrientationEndOfLine->x - x_orientation;
      oppositeOrientationBlankPoint.y = oppositeOrientationEndOfLine->y - y_orientation;
      orientation2BlankPoint.x = orientationEndOfLine->x + 2 * x_orientation;
      orientation2BlankPoint.y = orientationEndOfLine->y + 2 * y_orientation;
      oppositeOrientation2BlankPoint.x = oppositeOrientationEndOfLine->x - 2 * x_orientation;
      oppositeOrientation2BlankPoint.y = oppositeOrientationEndOfLine->y - 2 * y_orientation;
      orientation3BlankPoint.x = orientationEndOfLine->x + 3 * x_orientation;
      orientation3BlankPoint.y = orientationEndOfLine->y + 3 * y_orientation;
      oppositeOrientation3BlankPoint.x = oppositeOrientationEndOfLine->x - 3 * x_orientation;
      oppositeOrientation3BlankPoint.y = oppositeOrientationEndOfLine->y - 3 * y_orientation;
            
      stoneTypeOrientationBlank = puzzle->board[orientationBlankPoint.x][orientationBlankPoint.y];
      stoneTypeOppositeOrientationBlank = puzzle->board[oppositeOrientationBlankPoint.x][oppositeOrientationBlankPoint.y];
      stoneTypeOrientation2Blank = puzzle->board[orientation2BlankPoint.x][orientation2BlankPoint.y];
      stoneTypeOppositeOrientation2Blank = puzzle->board[oppositeOrientation2BlankPoint.x][oppositeOrientation2BlankPoint.y];
      stoneTypeOrientation3Blank = puzzle->board[orientation3BlankPoint.x][orientation3BlankPoint.y];
      stoneTypeOppositeOrientation3Blank = puzzle->board[oppositeOrientation3BlankPoint.x][oppositeOrientation3BlankPoint.y];
            
      free(orientationEndOfLine);
      free(oppositeOrientationEndOfLine);
            
      //ここから線の判別
            
      //TODO: 長連の判定をつける
            
      if (lineLength == 3) {
        if ( //・●●●・
          (stoneTypeOrientationBlank == S_BLANK && stoneTypeOppositeOrientationBlank == S_BLANK)
          || //・・●●●
          (stoneTypeOrientationBlank == S_BLANK && stoneTypeOrientation2Blank == S_BLANK)
          || //●●●・・
          (stoneTypeOppositeOrientationBlank == S_BLANK && stoneTypeOppositeOrientation2Blank == S_BLANK)
        ) {
          return mainLine;
        }
      } else if (lineLength == 2) {
        if ( //・●・●●
          (stoneTypeOrientationBlank == S_BLANK && stoneTypeOrientation2Blank == lineStoneType && stoneTypeOrientation3Blank == S_BLANK)
          || //●・●●・
          (stoneTypeOrientationBlank == S_BLANK &&
           stoneTypeOrientation2Blank == lineStoneType &&
           stoneTypeOppositeOrientationBlank == S_BLANK)
        ) {
          pointList = newPointList();
          pointList->x = orientation2BlankPoint.x;
          pointList->y = orientation2BlankPoint.y;
                    
          appendPointList(mainLine, pointList);
          return mainLine;
        }
                
        if ( //●●・●・
          (stoneTypeOppositeOrientationBlank == S_BLANK && stoneTypeOppositeOrientation2Blank == lineStoneType && stoneTypeOppositeOrientation3Blank == S_BLANK)
          || //・●●・●
          (stoneTypeOppositeOrientationBlank == S_BLANK &&
           stoneTypeOppositeOrientation2Blank == lineStoneType &&
           stoneTypeOrientationBlank == S_BLANK)
        ) {
          pointList = newPointList();
          pointList->x = oppositeOrientation2BlankPoint.x;
          pointList->y = oppositeOrientation2BlankPoint.y;
                    
          appendPointList(mainLine, pointList);
          return mainLine;
        }
                
        if ( //●・・●●
          stoneTypeOrientationBlank == S_BLANK && stoneTypeOrientation2Blank == S_BLANK && stoneTypeOrientation3Blank == lineStoneType
        ) {
          pointList = newPointList();
          pointList->x = orientation3BlankPoint.x;
          pointList->y = orientation3BlankPoint.y;
                    
          appendPointList(mainLine, pointList);
          return mainLine;
        }
                
        if ( //●●・・●
          stoneTypeOppositeOrientationBlank == S_BLANK && stoneTypeOppositeOrientation2Blank == S_BLANK && stoneTypeOppositeOrientation3Blank == lineStoneType
        ) {
          pointList = newPointList();
          pointList->x = oppositeOrientation3BlankPoint.x;
          pointList->y = oppositeOrientation3BlankPoint.y;
                    
          appendPointList(mainLine, pointList);
          return mainLine;
        }
      } else if (lineLength == 1) {
        if ( //●・●・●
          stoneTypeOrientation2Blank == lineStoneType &&
          stoneTypeOrientationBlank == S_BLANK &&
          stoneTypeOppositeOrientation2Blank == lineStoneType &&
          stoneTypeOppositeOrientationBlank == S_BLANK
        ) {
          pointList = newPointList();
          pointList->x = orientation2BlankPoint.x;
          pointList->y = orientation2BlankPoint.y;
                    
          appendPointList(mainLine, pointList);
                    
          pointList = newPointList();
          pointList->x = oppositeOrientation2BlankPoint.x;
          pointList->y = oppositeOrientation2BlankPoint.y;
                    
          appendPointList(mainLine, pointList);
          return mainLine;
        }
      }
            
      freePointList(mainLine);
      mainLine = NULL;
    }
  }
    
  return mainLine;
}

POINTLIST* getSplitLineWithOrientation(GOMOKU* puzzle, int x, int y, int x_orientation, int y_orientation, int lineStoneType) {
  POINTLIST* offsetSplitLine = NULL;
  POINTLIST* oppositeOffsetSplitLine = NULL;
  POINT* orientationEndOfLine;
  POINT* oppositeOrientationEndOfLine;
    
  POINTLIST* mainLine = NULL;
    
  if (puzzle->board[x][y] != S_BLANK) {
    mainLine = getLineWithOrientation(puzzle, x, y, x_orientation, y_orientation, lineStoneType);
        
    //本線の端の座標を取得
    orientationEndOfLine = getEndOfLineWithOffset(puzzle, mainLine, x_orientation, y_orientation, lineStoneType);
    oppositeOrientationEndOfLine = getEndOfLineWithOffset(puzzle, mainLine, -x_orientation, -y_orientation, lineStoneType);
        
    int checkX1, checkX2, checkY1, checkY2;
        
    if (orientationEndOfLine != NULL && oppositeOrientationEndOfLine != NULL) {
      //offset側
      if (isInTheBoard(orientationEndOfLine->x + x_orientation, orientationEndOfLine->y + y_orientation) && puzzle->board[orientationEndOfLine->x + x_orientation][orientationEndOfLine->y + y_orientation] == S_BLANK) {
        checkX1 = orientationEndOfLine->x + 2 * x_orientation;
        checkY1 = orientationEndOfLine->y + 2 * y_orientation;
                
        if (isInTheBoard(checkX1, checkY1) == true) {
          offsetSplitLine = getLineWithOrientation(puzzle, checkX1, checkY1, x_orientation, y_orientation, lineStoneType);
        }
      }
            
      //oppositeOffset側
      if (isInTheBoard(oppositeOrientationEndOfLine->x - x_orientation, oppositeOrientationEndOfLine->y - y_orientation) && puzzle->board[oppositeOrientationEndOfLine->x - x_orientation][oppositeOrientationEndOfLine->y - y_orientation] == S_BLANK) {
        checkX2 = oppositeOrientationEndOfLine->x - 2 * x_orientation;
        checkY2 = oppositeOrientationEndOfLine->y - 2 * y_orientation;
                
        if (isInTheBoard(checkX2, checkY2) == true) {
          oppositeOffsetSplitLine = getLineWithOrientation(puzzle, checkX2, checkY2, x_orientation, y_orientation, lineStoneType);
        }
      }
            
            
      int lengthOfOffsetSplitLine, lengthOfOppositeSpliteLine;
      lengthOfOffsetSplitLine = lengthOfPointList(offsetSplitLine);
      lengthOfOppositeSpliteLine = lengthOfPointList(oppositeOffsetSplitLine);
            
      if (lengthOfOffsetSplitLine != 0 || lengthOfOppositeSpliteLine != 0) {
        if (lengthOfOffsetSplitLine > lengthOfOppositeSpliteLine) {
          appendPointList(mainLine, offsetSplitLine);
        } else {
          appendPointList(mainLine, oppositeOffsetSplitLine);
        }
      }
      free(orientationEndOfLine);
      free(oppositeOrientationEndOfLine);
    } else {
      freePointList(mainLine);
      mainLine = NULL;
    }
  }
    
  return mainLine;
}

int lengthOfPointList(POINTLIST* pointList) {
  POINTLIST* currentPointList = pointList;
  int ret = 0;
    
  if (pointList != NULL) {
    do {
      ret++;
    } while ( (currentPointList = currentPointList->next) != NULL );
  }
    
  return ret;
}

void freePointList(POINTLIST* pointList) {
  POINTLIST* tmp;
    
  while (pointList != NULL) {
    tmp = pointList;
    pointList = pointList->next;
    free(tmp);
  }
}

void freePuzzle(GOMOKU* puzzle) {
  if (puzzle != NULL) {
    if (puzzle->blackStoneHistory != NULL) {
      freePointList(puzzle->blackStoneHistory);
      puzzle->blackStoneHistory = NULL;
    }
    if (puzzle->whiteStoneHistory != NULL) {
      freePointList(puzzle->whiteStoneHistory);
      puzzle->whiteStoneHistory = NULL;
    }
        
    free(puzzle);
  }
}

//白の飛び5は押さえにいかないといけない
POINTLIST* findSplit4Line(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  POINTLIST* pLine;
  int length;
    
  pLine = getSplitVerticalLine(puzzle, x, y, lineStoneType);
  length = lengthOfPointList(pLine);
  if (length == 4 || (lineStoneType == S_WHITE && length > 4)) {
    return pLine;
  }
    
  pLine = getSplit45DegreeLine(puzzle, x, y, lineStoneType);
  length = lengthOfPointList(pLine);
  if (length == 4 || (lineStoneType == S_WHITE && length > 4)) {
    return pLine;
  }
    
  pLine = getSplitHorizontalLine(puzzle, x, y, lineStoneType);
  length = lengthOfPointList(pLine);
  if (length == 4 || (lineStoneType == S_WHITE && length > 4)) {
    return pLine;
  }
    
  pLine = getSplit135DegreeLine(puzzle, x, y, lineStoneType);
  length = lengthOfPointList(pLine);
  if (length == 4 || (lineStoneType == S_WHITE && length > 4)) {
    return pLine;
  }
    
  return NULL;
}

POINTLIST* findSplit3Line(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  POINTLIST* pLine;
    
  pLine = getSplitVerticalLine(puzzle, x, y, lineStoneType);
  if (lengthOfPointList(pLine) == 3) {
    return pLine;
  }
    
  pLine = getSplit45DegreeLine(puzzle, x, y, lineStoneType);
  if (lengthOfPointList(pLine) == 3) {
    return pLine;
  }
    
  pLine = getSplitHorizontalLine(puzzle, x, y, lineStoneType);
  if (lengthOfPointList(pLine) == 3) {
    return pLine;
  }
    
  pLine = getSplit135DegreeLine(puzzle, x, y, lineStoneType);
  if (lengthOfPointList(pLine) == 3) {
    return pLine;
  }
    
  return NULL;
}

POINTLIST* findLineAbleToMake4(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  POINTLIST* pLine;
    
  pLine = getLineAbleToMake4WithOrientation(puzzle, x, y, 1, 0, lineStoneType);
  if (pLine != NULL) {
    return pLine;
  }
    
  pLine = getLineAbleToMake4WithOrientation(puzzle, x, y, 0, 1, lineStoneType);
  if (pLine != NULL) {
    return pLine;
  }
    
  pLine = getLineAbleToMake4WithOrientation(puzzle, x, y, 1, 1, lineStoneType);
  if (pLine != NULL) {
    return pLine;
  }
    
  pLine = getLineAbleToMake4WithOrientation(puzzle, x, y, 1, -1, lineStoneType);
  if (pLine != NULL) {
    return pLine;
  }

  return NULL;
}

POINTLIST* findAliveSplitLine(GOMOKU* puzzle, int x, int y, int lineStoneType, int lineLength) {
  POINTLIST* pLine;
  bool bCheck2Line = false;
    
  if (lineLength == 2) {
    bCheck2Line = true;
  }
    
  pLine = getSplitVerticalLine(puzzle, x, y, lineStoneType);
  if (pLine != NULL) {
    if (lengthOfPointList(pLine) == lineLength) {
      if (isAliveLine(puzzle, pLine, lineStoneType, bCheck2Line) == true) {
        return pLine;
      }
    }
    freePointList(pLine);
  }
    
  pLine = getSplit45DegreeLine(puzzle, x, y, lineStoneType);
  if (pLine != NULL) {
    if (lengthOfPointList(pLine) == lineLength) {
      if (isAliveLine(puzzle, pLine, lineStoneType, bCheck2Line) == true) {
        return pLine;
      }
    }
    freePointList(pLine);
  }
    
  pLine = getSplitHorizontalLine(puzzle, x, y, lineStoneType);
  if (pLine != NULL) {
    if (lengthOfPointList(pLine) == lineLength) {
      if (isAliveLine(puzzle, pLine, lineStoneType, bCheck2Line) == true) {
        return pLine;
      }
    }
    freePointList(pLine);
  }
    
  pLine = getSplit135DegreeLine(puzzle, x, y, lineStoneType);
  if (pLine != NULL) {
    if (lengthOfPointList(pLine) == lineLength) {
      if (isAliveLine(puzzle, pLine, lineStoneType, bCheck2Line) == true) {
        return pLine;
      }
    }
    freePointList(pLine);
  }
    
  return NULL;
}

int compareXPoint(const POINTLIST** a, const POINTLIST** b) {
  return (*a)->x - (*b)->x;
}

int compareYPoint(const POINTLIST** a, const POINTLIST** b) {
  return (*a)->y - (*b)->y;
}

void sortLine(POINTLIST* pLine) {
  POINTLIST* pPointList[BOARD_SIZE];
  POINTLIST* pCurrentPoint;
  POINTLIST* pTempPoint;
  int i = 0;
    
  if (pLine != NULL) {
    pCurrentPoint = pLine;
    pPointList[i] = pCurrentPoint;
    while ( (pCurrentPoint = pCurrentPoint->next) != NULL) {
      i++;
      pPointList[i] = pCurrentPoint;
    }
        
    //xとyについて順番にクイックソート
    qsort(pPointList, i + 1, sizeof(POINTLIST*), (int(*)(const void*, const void*))compareXPoint);
    qsort(pPointList, i + 1, sizeof(POINTLIST*), (int(*)(const void*, const void*))compareYPoint);
        
    for (int j = 0; j < i; j++) {
      pPointList[j]->next = pPointList[j + 1];
    }
    pPointList[i]->next = NULL;
        
    //もともと線の先頭だったものをソート後の先頭と内容を置き換える。
    for (int j = 0; j <= i; j++) {
      pCurrentPoint = pPointList[j];
      if (pCurrentPoint == pLine) {
        if (j != 0) {
          pTempPoint = newPointList();
                    
          pTempPoint->x = pPointList[0]->x;
          pTempPoint->y = pPointList[0]->y;
          pTempPoint->next = pPointList[0]->next;
                    
          pPointList[0]->x = pLine->x;
          pPointList[0]->y = pLine->y;
          pPointList[0]->next = pLine->next;
                    
          pLine->x = pTempPoint->x;
          pLine->y = pTempPoint->y;
          pLine->next = pTempPoint->next;
                    
          if (j == 1) {
            pPointList[1]->next = pPointList[0];
          }
          else if (j > 1) {
            pPointList[j - 1]->next = pPointList[0];
          }
                    
          pTempPoint->next = NULL;
          freePointList(pTempPoint);
        }
        break;
      }
    }
  }
    
  return;
}

int maxSubOfTwoPoint(int x1, int y1, int x2, int y2) {
  int x_sub, y_sub;
    
  x_sub = abs(x1 - x2);
  y_sub = abs(y1 - y2);
    
  if (x_sub > y_sub) {
    return x_sub;
  } else {
    return y_sub;
  }
}

POINT* getSplitPoint(POINTLIST* pLine) {
  POINTLIST* pNextPoint;
  POINTLIST* pCurrentPoint;
  int maxSub;
    
  sortLine(pLine);
  pCurrentPoint = pLine;
    
  if (pCurrentPoint != NULL) {
    while ( (pNextPoint = pCurrentPoint->next) != NULL) {
      maxSub = maxSubOfTwoPoint(pCurrentPoint->x, pCurrentPoint->y, pNextPoint->x, pNextPoint->y);
      if ( maxSub == 2) {
        POINT* splitPoint = (POINT *)malloc(sizeof(POINT));
                
        splitPoint->x = (pCurrentPoint->x + pNextPoint->x) / 2;
        splitPoint->y = (pCurrentPoint->y + pNextPoint->y) / 2;
                
        return splitPoint;
      } else if (maxSub == 3) {
        POINT* splitPoint = (POINT *)malloc(sizeof(POINT));
        int x_orientation = 0;
        int y_orientation = 0;
        if (pCurrentPoint->x - pNextPoint->x != 0) {
          x_orientation = (pNextPoint->x - pCurrentPoint->x) / abs(pNextPoint->x - pCurrentPoint->x);
        }
        if (pCurrentPoint->y - pNextPoint->y != 0) {
          y_orientation = (pNextPoint->y - pCurrentPoint->y) / abs(pNextPoint->y - pCurrentPoint->y);
        }
                
        splitPoint->x = pCurrentPoint->x + x_orientation;
        splitPoint->y = pCurrentPoint->y + y_orientation;
                
        return splitPoint;
      }
            
      pCurrentPoint = pNextPoint;
    }
  }
  return NULL;
}

VECTOR* getLineOrientationVector(POINTLIST* line) {
  sortLine(line);
    
  if (line != NULL && line->next != NULL) {
    VECTOR* orientationVector = (VECTOR *)malloc(sizeof(VECTOR));
        
    if (line->x != line->next->x) {
      orientationVector->x = (line->x - line->next->x) / abs(line->x - line->next->x);
    } else {
      orientationVector->x = 0;
    }
        
    if (line->y != line->next->y) {
      orientationVector->y = (line->y - line->next->y) / abs(line->y - line->next->y);
    } else {
      orientationVector->y = 0;
    }
        
    return orientationVector;
  }
    
  return NULL;
}

bool isInTheBoard(int x, int y) {
  if (x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE) {
    return true;
  } else {
    return false;
  }
}

POINT* findBlankBesideLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType) {
  VECTOR* orientationVector = getLineOrientationVector(pLine);
  POINT* directionalEndPoint;
  POINT* oppositeDirectionalEndPoint;
    
  if (orientationVector != NULL) {
    directionalEndPoint = getEndOfLineWithOffset(puzzle, pLine, orientationVector->x, orientationVector->y, lineStoneType);
    oppositeDirectionalEndPoint = getEndOfLineWithOffset(puzzle, pLine, -orientationVector->x, -orientationVector->y, lineStoneType);
        
    POINT* blankPoint = (POINT *)malloc(sizeof(POINT));
        
    int checkX1, checkX2, checkY1, checkY2;
    if (directionalEndPoint != NULL && oppositeDirectionalEndPoint != NULL) {
      checkX1 = directionalEndPoint->x + orientationVector->x;
      checkY1 = directionalEndPoint->y + orientationVector->y;
      checkX2 = oppositeDirectionalEndPoint->x - orientationVector->x;
      checkY2 = oppositeDirectionalEndPoint->y - orientationVector->y;
            
      if (isInTheBoard(checkX1, checkY1) && puzzle->board[checkX1][checkY1] == S_BLANK) {
        blankPoint->x = checkX1;
        blankPoint->y = checkY1;
      } else if (isInTheBoard(checkX2, checkY2) && puzzle->board[checkX2][checkY2] == S_BLANK) {
        blankPoint->x = checkX2;
        blankPoint->y = checkY2;
      } else {
        free(blankPoint);
        blankPoint = NULL;
      }
            
      free(directionalEndPoint);
      free(oppositeDirectionalEndPoint);
            
      return blankPoint;
    }
  }
    
  return NULL;
}

int numOfAlivePoint(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType) {
  int num = 0;
  POINTLIST* alivePointList;
    
  alivePointList = getBlankPointListBesideLine(puzzle, pLine, lineStoneType);
  num = lengthOfPointList(alivePointList);
  freePointList(alivePointList);
    
  return num;
}

POINTLIST* getBlankPointListBesideLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType) {
  VECTOR* orientationVector = getLineOrientationVector(pLine);
  POINT* directionalEndPoint;
  POINT* oppositeDirectionalEndPoint;
  bool bExistOneBlank = false;
    
  if (orientationVector != NULL) {
    directionalEndPoint = getEndOfLineWithOffset(puzzle, pLine, orientationVector->x, orientationVector->y, lineStoneType);
    oppositeDirectionalEndPoint = getEndOfLineWithOffset(puzzle, pLine, -orientationVector->x, -orientationVector->y, lineStoneType);
        
    POINTLIST* blankPointList = newPointList();
        
    int checkX1, checkX2, checkY1, checkY2;
    if (directionalEndPoint != NULL && oppositeDirectionalEndPoint != NULL) {
      checkX1 = directionalEndPoint->x + orientationVector->x;
      checkY1 = directionalEndPoint->y + orientationVector->y;
      checkX2 = oppositeDirectionalEndPoint->x - orientationVector->x;
      checkY2 = oppositeDirectionalEndPoint->y - orientationVector->y;
            
      if (isInTheBoard(checkX1, checkY1) && puzzle->board[checkX1][checkY1] == S_BLANK) {
        blankPointList->x = checkX1;
        blankPointList->y = checkY1;
        bExistOneBlank = true;
      }
            
      if (isInTheBoard(checkX2, checkY2) && puzzle->board[checkX2][checkY2] == S_BLANK) {
        if (bExistOneBlank == true) {
          POINTLIST* nextBlankPointList = newPointList();
          nextBlankPointList->x = checkX2;
          nextBlankPointList->y = checkY2;
          blankPointList->next = nextBlankPointList;
        } else {
          blankPointList->x = checkX2;
          blankPointList->y = checkY2;
          bExistOneBlank = true;
        }
      }
            
      if (bExistOneBlank == false) {
        freePointList(blankPointList);
        blankPointList = NULL;
      }
            
      free(directionalEndPoint);
      free(oppositeDirectionalEndPoint);
            
      return blankPointList;
    }
  }
    
  return NULL;
}

POINT* findAlivePointBesideLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType) {
  VECTOR* orientationVector = getLineOrientationVector(pLine);
  POINT* directionalEndPoint;
  POINT* oppositeDirectionalEndPoint;
  POINT* alivePoint;
    
  if (orientationVector != NULL) {
    directionalEndPoint = getEndOfLineWithOffset(puzzle, pLine, orientationVector->x, orientationVector->y, lineStoneType);
    oppositeDirectionalEndPoint = getEndOfLineWithOffset(puzzle, pLine, -orientationVector->x, -orientationVector->y, lineStoneType);
        
    int checkX1_1, checkY1_1, checkX1_2, checkY1_2, checkX2_1, checkY2_1, checkX2_2, checkY2_2;
        
    if (directionalEndPoint != NULL && oppositeDirectionalEndPoint != NULL) {
      checkX1_1 = directionalEndPoint->x + orientationVector->x;
      checkY1_1 = directionalEndPoint->y + orientationVector->y;
      checkX1_2 = directionalEndPoint->x + 2 * orientationVector->x;
      checkY1_2 = directionalEndPoint->y + 2 * orientationVector->y;
      checkX2_1 = oppositeDirectionalEndPoint->x - orientationVector->x;
      checkY2_1 = oppositeDirectionalEndPoint->y - orientationVector->y;
      checkX2_2 = oppositeDirectionalEndPoint->x - 2 * orientationVector->x;
      checkY2_2 = oppositeDirectionalEndPoint->y - 2 * orientationVector->y;
            
      free(orientationVector);
            
      if (isInTheBoard(checkX1_1, checkY1_1) && puzzle->board[checkX1_1][checkY1_1] == S_BLANK) {
        if (isInTheBoard(checkX1_2, checkY1_2) && puzzle->board[checkX1_2][checkY1_2] == S_BLANK) {
          if (isBannedPoint(puzzle, checkX1_1, checkY1_1, lineStoneType) == false) {
            alivePoint = (POINT *)malloc(sizeof(POINT));
            alivePoint->x = checkX1_1;
            alivePoint->y = checkY1_1;
            return alivePoint;
          }
        }
      }
            
      if (isInTheBoard(checkX2_1, checkY2_1) && puzzle->board[checkX2_1][checkY2_1] == S_BLANK) {
        if (isInTheBoard(checkX2_2, checkY2_2) && puzzle->board[checkX2_2][checkY2_2] == S_BLANK) {
          if (isBannedPoint(puzzle, checkX1_1, checkY1_1, lineStoneType) == false) {
            alivePoint = (POINT *)malloc(sizeof(POINT));
            alivePoint->x = checkX2_1;
            alivePoint->y = checkY2_1;
            return alivePoint;
          }
        }
      }
            
      free(directionalEndPoint);
      free(oppositeDirectionalEndPoint);
    }
        
  }
    
  return NULL;
}


int numOf2BlankPointListBesideLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType) {
  int num = 0;
  POINTLIST* alivePointList;
    
  alivePointList = get2BlankPointListBesideLine(puzzle, pLine, lineStoneType);
  num = lengthOfPointList(alivePointList);
  freePointList(alivePointList);
    
  return num;
}

POINTLIST* get2BlankPointListBesideLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType) {
  VECTOR* orientationVector = getLineOrientationVector(pLine);
  POINT* directionalEndPoint;
  POINT* oppositeDirectionalEndPoint;
  bool bExistOneBlank = false;
    
  if (orientationVector != NULL) {
    directionalEndPoint = getEndOfLineWithOffset(puzzle, pLine, orientationVector->x, orientationVector->y, lineStoneType);
    oppositeDirectionalEndPoint = getEndOfLineWithOffset(puzzle, pLine, -orientationVector->x, -orientationVector->y, lineStoneType);
        
    POINTLIST* blankPointList = newPointList();
        
    int checkX1_1, checkY1_1, checkX1_2, checkY1_2, checkX2_1, checkY2_1, checkX2_2, checkY2_2;
        
    if (directionalEndPoint != NULL && oppositeDirectionalEndPoint != NULL) {
      checkX1_1 = directionalEndPoint->x + orientationVector->x;
      checkY1_1 = directionalEndPoint->y + orientationVector->y;
      checkX1_2 = directionalEndPoint->x + 2 * orientationVector->x;
      checkY1_2 = directionalEndPoint->y + 2 * orientationVector->y;
      checkX2_1 = oppositeDirectionalEndPoint->x - orientationVector->x;
      checkY2_1 = oppositeDirectionalEndPoint->y - orientationVector->y;
      checkX2_2 = oppositeDirectionalEndPoint->x - 2 * orientationVector->x;
      checkY2_2 = oppositeDirectionalEndPoint->y - 2 * orientationVector->y;
            
      if (isInTheBoard(checkX1_1, checkY1_1) && puzzle->board[checkX1_1][checkY1_1] == S_BLANK) {
        blankPointList->x = checkX1_1;
        blankPointList->y = checkY1_1;
        bExistOneBlank = true;
        if (isInTheBoard(checkX1_2, checkY1_2) && puzzle->board[checkX1_2][checkY1_2] == S_BLANK) {
          POINTLIST* nextBlankPointList = newPointList();
          nextBlankPointList->x = checkX1_2;
          nextBlankPointList->y = checkY1_2;
          blankPointList->next = nextBlankPointList;
        }
      }
            
      if (isInTheBoard(checkX2_1, checkY2_1) && puzzle->board[checkX2_1][checkY2_1] == S_BLANK) {
        if (bExistOneBlank == true) {
          POINTLIST* nextBlankPointList = newPointList();
          nextBlankPointList->x = checkX2_1;
          nextBlankPointList->y = checkY2_1;
          appendPointList(blankPointList, nextBlankPointList);
        } else {
          blankPointList->x = checkX2_1;
          blankPointList->y = checkY2_1;
          bExistOneBlank = true;
        }
                
        if (isInTheBoard(checkX2_2, checkY2_2) && puzzle->board[checkX2_2][checkY2_2] == S_BLANK) {
          POINTLIST* nextBlankPointList = newPointList();
          nextBlankPointList->x = checkX2_2;
          nextBlankPointList->y = checkY2_2;
          appendPointList(blankPointList, nextBlankPointList);
        }
      }
            
      if (bExistOneBlank == false) {
        freePointList(blankPointList);
        blankPointList = NULL;
      }
      free(orientationVector);
            
      free(directionalEndPoint);
      free(oppositeDirectionalEndPoint);
            
      return blankPointList;
    }
  }
    
  return NULL;
}

POINT* getKillPointOf4Line(GOMOKU* puzzle, POINTLIST* p4Line, int lineStoneType) {
  POINT* killPoint = NULL;
    
  killPoint = getSplitPoint(p4Line);
    
  if (killPoint != NULL) {
    return killPoint;
  } else {
    killPoint = findBlankBesideLine(puzzle, p4Line, lineStoneType);
  }
  return killPoint;
}

POINT* getKillPointOf3Line(GOMOKU* puzzle, POINTLIST* p3Line, int lineStoneType) {
  POINT* killPoint = NULL;
    
  killPoint = getSplitPoint(p3Line);
    
  if (killPoint != NULL) {
    return killPoint;
  } else {
    killPoint = findBlankBesideLine(puzzle, p3Line, lineStoneType);
  }
  return killPoint;
}

POINT* findAlivePointOf2Line(GOMOKU* puzzle, POINTLIST* p2Line, int lineStoneType) {
  POINT* splitPoint;
    
  splitPoint = getSplitPoint(p2Line);
    
  if (splitPoint != NULL && isBannedPoint(puzzle, splitPoint->x, splitPoint->y, lineStoneType)) {
    free(splitPoint);
    splitPoint = NULL;
  }
    
  if (splitPoint != NULL) {
    return splitPoint;
  } else {
    return findAlivePointBesideLine(puzzle, p2Line, lineStoneType);
  }
}

POINTLIST* getKillPointListOf3Line(GOMOKU* puzzle, POINTLIST* p3Line, int lineStoneType) {
  POINTLIST* splitPointList;
  POINTLIST* killPointList = newPointList();
  POINT* splitPoint;
    
  splitPoint = getSplitPoint(p3Line);
    
  if (splitPoint != NULL) {
    splitPointList = newPointList();
    splitPointList->x = splitPoint->x;
    splitPointList->y = splitPoint->y;
    free(splitPoint);
    killPointList = getBlankPointListBesideLine(puzzle, p3Line, lineStoneType);
    splitPointList->next = killPointList;

    return splitPointList;
  } else {
    killPointList = getBlankPointListBesideLine(puzzle, p3Line, lineStoneType);
  }
  return killPointList;
}

POINTLIST* getKillPointListOf3LineWithoutSplitPoint(GOMOKU* puzzle, POINTLIST* p3Line, int lineStoneType) {
  POINTLIST* killPointList;
  killPointList = getBlankPointListBesideLine(puzzle, p3Line, lineStoneType);
    
  return killPointList;
}

int maxLineLengthOf(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  POINTLIST* plVerticalLine;
  POINTLIST* pl45degreeLine;
  POINTLIST* plHorizontalLine;
  POINTLIST* pl135degreeLine;
  int length;
  int maxLineLength = 0;
    
  if (lineStoneType != S_BLANK) {
    plVerticalLine = getVerticalLine(puzzle, x, y, lineStoneType);
    length = lengthOfPointList(plVerticalLine);
    freePointList(plVerticalLine);
    maxLineLength = length;
        
    pl45degreeLine = get45DegreeLine(puzzle, x, y, lineStoneType);
    length = lengthOfPointList(pl45degreeLine);
    freePointList(pl45degreeLine);
    if (maxLineLength < length) maxLineLength = length;
        
    plHorizontalLine = getHorizontalLine(puzzle, x, y, lineStoneType);
    length = lengthOfPointList(plHorizontalLine);
    freePointList(plHorizontalLine);
    if (maxLineLength < length) maxLineLength = length;
        
    pl135degreeLine = get135DegreeLine(puzzle, x, y, lineStoneType);
    length = lengthOfPointList(pl135degreeLine);
    freePointList(pl135degreeLine);
    if (maxLineLength < length) maxLineLength = length;
  }
    
  return maxLineLength;
}

POINTLIST* getMaxLengthLine(GOMOKU* puzzle, int x, int y, int lineStoneType) {
  POINTLIST* plVerticalLine;
  POINTLIST* pl45degreeLine;
  POINTLIST* plHorizontalLine;
  POINTLIST* pl135degreeLine;
  POINTLIST* plReturn = NULL;
  int maxLineLength = 0;
  int length;
    
  if (lineStoneType != S_BLANK) {
    plVerticalLine = getVerticalLine(puzzle, x, y, lineStoneType);
    maxLineLength = lengthOfPointList(plVerticalLine);
    plReturn = plVerticalLine;
        
    pl45degreeLine = get45DegreeLine(puzzle, x, y, lineStoneType);
    length = lengthOfPointList(pl45degreeLine);
    if (maxLineLength < length) {
      maxLineLength = length;
      plReturn = pl45degreeLine;
    }

    plHorizontalLine = getHorizontalLine(puzzle, x, y, lineStoneType);
    length = lengthOfPointList(plHorizontalLine);
    if (maxLineLength < length) {
      maxLineLength = length;
      plReturn = plHorizontalLine;
    }

    pl135degreeLine = get135DegreeLine(puzzle, x, y, lineStoneType);
    length = lengthOfPointList(pl135degreeLine);
    if (maxLineLength < length) {
      plReturn = pl135degreeLine;
    }

    if (plVerticalLine != NULL && plReturn != plVerticalLine) {
      freePointList(plVerticalLine);
    }
    if (pl45degreeLine != NULL && plReturn != pl45degreeLine){
      freePointList(pl45degreeLine);
    }
    if (plHorizontalLine != NULL && plReturn != plHorizontalLine) {
      freePointList(plHorizontalLine);
    }
    if (pl135degreeLine != NULL && plReturn != pl135degreeLine) {
      freePointList(pl135degreeLine);
    }
  }
    
  return plReturn;
}

POINTLIST* getFiveLine(GOMOKU* puzzle) {
  POINTLIST* plLine;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] != S_BLANK) {
        plLine = getMaxLengthLine(puzzle, i, j, puzzle->board[i][j]);
        if (lengthOfPointList(plLine) == 5 || (lengthOfPointList(plLine) >= 6 && plLine != NULL && puzzle->board[plLine->x][plLine->y] == S_WHITE) ) {
          return plLine;
        } else {
          if (plLine != NULL) {
            freePointList(plLine);
          }
        }
      }
    }
  }
    
  return NULL;
}

bool existFiveLine(GOMOKU* puzzle, int lineStoneType) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == lineStoneType) {
        if (lineStoneType == S_BLACK) {
          if (maxLineLengthOf(puzzle, i, j, lineStoneType) == 5) {
            return true;
          }
        } else {
          if (maxLineLengthOf(puzzle, i, j, lineStoneType) >= 5) {
            return true;
          }
        }
      }
    }
  }
    
  return false;
}

void putStone(GOMOKU* puzzle, int x, int y, int putStoneType) {
  puzzle->board[x][y] = putStoneType;
  addStoneHistory(puzzle, x, y, putStoneType);
}

GOMOKU* copyPuzzle(GOMOKU* puzzle) {
  if (puzzle != NULL) {
    GOMOKU* copyPuzzle = (GOMOKU *)malloc(sizeof(GOMOKU));
        
    //これは意味ない　バグの元なのでNULL代入する
    //copyPuzzle->whiteStoneHistory = puzzle->whiteStoneHistory;
    //copyPuzzle->blackStoneHistory = puzzle->blackStoneHistory;
        
    copyPuzzle->whiteStoneHistory = NULL;
    copyPuzzle->blackStoneHistory = NULL;
    copyPuzzle->numOfStone = puzzle->numOfStone;
        
    for (int i = 0; i < BOARD_SIZE; i++) {
      for (int j = 0; j < BOARD_SIZE; j++) {
        copyPuzzle->board[i][j] = puzzle->board[i][j];
      }
    }
        
    return copyPuzzle;
  }
    
  return NULL;
}

int isBannedPoint(GOMOKU* puzzle, int x, int y, int putStoneType) {
  GOMOKU* assumptionPuzzle;
  POINTLIST* pLine;
  int numOf3Line, numOf4Line, lengthOfLine;
  int bannedReason = 0;
    
  if (putStoneType == S_WHITE) {
    return 0;
  }
    
  numOf3Line = 0;
  numOf4Line = 0;
    
  assumptionPuzzle = copyPuzzle(puzzle);
  assumptionPuzzle->board[x][y] = putStoneType;
    
  pLine = getSplitVerticalLine(assumptionPuzzle, x, y, putStoneType);
  lengthOfLine = lengthOfPointList(pLine);
  if (lengthOfLine == 3) {
    if (numOfAlivePoint(puzzle, pLine, putStoneType) == 2) {
      numOf3Line++;
    }
  } else if (lengthOfLine == 4) {
    numOf4Line++;
  } else if (lengthOfLine > 5) {
    bannedReason = BAN_LONG;
  }
  freePointList(pLine);
    
  pLine = getSplit45DegreeLine(assumptionPuzzle, x, y, putStoneType);
  lengthOfLine = lengthOfPointList(pLine);
  if (lengthOfLine == 3) {
    if (numOfAlivePoint(puzzle, pLine, putStoneType) == 2) {
      numOf3Line++;
    }
  } else if (lengthOfLine == 4) {
    numOf4Line++;
  } else if (lengthOfLine > 5) {
    bannedReason = BAN_LONG;
  }
  freePointList(pLine);
    
  pLine = getSplit135DegreeLine(assumptionPuzzle, x, y, putStoneType);
  lengthOfLine = lengthOfPointList(pLine);
  if (lengthOfLine == 3) {
    if (numOfAlivePoint(puzzle, pLine, putStoneType) == 2) {
      numOf3Line++;
    }
  } else if (lengthOfLine == 4) {
    numOf4Line++;
  } else if (lengthOfLine > 5) {
    bannedReason = BAN_LONG;
  }
  freePointList(pLine);
    
  pLine = getSplitHorizontalLine(assumptionPuzzle, x, y, putStoneType);
  lengthOfLine = lengthOfPointList(pLine);
  if (lengthOfLine == 3) {
    if (numOfAlivePoint(puzzle, pLine, putStoneType) == 2) {
      numOf3Line++;
    }
  } else if (lengthOfLine == 4) {
    numOf4Line++;
  } else if (lengthOfLine > 5) {
    bannedReason = BAN_LONG;
  }
  freePointList(pLine);
    
  if (numOf4Line >= 2) {
    bannedReason = BAN_4_4;
  } else if (numOf3Line >= 2) {
    bannedReason = BAN_3_3;
  }
    
  free(assumptionPuzzle);
    
  return bannedReason;
}

POINT* getPointToMake5Line(GOMOKU* puzzle, int putStoneType) {
  POINTLIST* p4Line;
  POINT* killPoint;
    
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == putStoneType) {
        p4Line = findSplit4Line(puzzle, i, j, putStoneType);
        if (p4Line != NULL) {
          killPoint = getKillPointOf4Line(puzzle, p4Line, putStoneType);
                
          if (killPoint != NULL) {
            freePointList(p4Line);
            return killPoint;
          }
          freePointList(p4Line);
        }
      }
    }
  }
  return NULL;
}

int getOppositeStone(int stoneType) {
  if (stoneType == S_BLACK) {
    return S_WHITE;
  } else {
    return S_BLACK;
  }
}

POINT* getPointToKill4Line(GOMOKU* puzzle, int putStoneType) {
  POINTLIST* p4Line;
  POINT* killPoint;
  int killedStoneType = getOppositeStone(putStoneType);
    
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == killedStoneType) {
        p4Line = findSplit4Line(puzzle, i, j, killedStoneType);
        if (p4Line != NULL) {
          killPoint = getKillPointOf4Line(puzzle, p4Line, killedStoneType);
                
          //おいたら三三の場合はあきらめる
          if (killPoint != NULL && isBannedPoint(puzzle, killPoint->x, killPoint->y, putStoneType)) {
            free(killPoint);
            killPoint = NULL;
          }
                    
          if (killPoint != NULL) {
            freePointList(p4Line);
            return killPoint;
          }
          freePointList(p4Line);
        }
      }
    }
  }
  return NULL;
}

POINT* getPointToMake4Line(GOMOKU* puzzle, int putStoneType) {
  POINTLIST* pLine;
  POINT* splitPoint;
  POINT* alivePoint;

  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == putStoneType) {
        pLine = findLineAbleToMake4(puzzle, i, j, putStoneType);
                
        if (pLine != NULL) {
          splitPoint = getSplitPoint(pLine);
          if (splitPoint != NULL) {
            return splitPoint;
          }
                    
          alivePoint = findAlivePointBesideLine(puzzle, pLine, putStoneType);
          if (alivePoint != NULL) {
            return alivePoint;
          }
          freePointList(pLine);
        }
      }
    }
  }
    
  return NULL;
}

POINT* getPointToMake4LineFromAlive3Line(GOMOKU* puzzle, int putStoneType) {
  POINTLIST* pLine;
  POINT* splitPoint;
  POINT* alivePoint;
    
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == putStoneType) {
        pLine = findAliveSplitLine(puzzle, i, j, putStoneType, 3);
                
        if (pLine != NULL) {
          splitPoint = getSplitPoint(pLine);
          if (splitPoint != NULL) {
            return splitPoint;
          }
                    
          alivePoint = findAlivePointBesideLine(puzzle, pLine, putStoneType);
          if (alivePoint != NULL) {
            return alivePoint;
          }
          freePointList(pLine);
        }
      }
    }
  }
    
  return NULL;
}

POINT* getKillPointOfAlive3LineWith4Line(GOMOKU* puzzle, POINTLIST* p3Line, int lineStoneType) {
  POINTLIST* killPointList;
  POINTLIST* headOfKillPointList;
  GOMOKU* assumptionPuzzle;
  POINT* putPoint;
  POINT* killPoint;
    
  headOfKillPointList = getKillPointListOf3Line(puzzle, p3Line, lineStoneType);
  killPointList = headOfKillPointList;
    
  while (killPointList != NULL) {
    assumptionPuzzle = copyPuzzle(puzzle);
    assumptionPuzzle->board[killPointList->x][killPointList->y] = getOppositeStone(lineStoneType);
    putPoint = getPointToKill4Line(assumptionPuzzle, lineStoneType);
    freePuzzle(assumptionPuzzle);
    if (putPoint != NULL) {
      free(putPoint);
      killPoint = (POINT *)malloc(sizeof(POINT));
      killPoint->x = killPointList->x;
      killPoint->y = killPointList->y;

      freePointList(headOfKillPointList);
      return killPoint;
    }
    killPointList = killPointList->next;
  }
    
  freePointList(headOfKillPointList);
  return NULL;
}

bool areTheSameLine(POINTLIST* pLine1, POINTLIST* pLine2) {
  POINTLIST* currentPoint1;
  POINTLIST* currentPoint2;
  bool bExistPoint;
    
  currentPoint1 = pLine1;
  while (currentPoint1 != NULL) {
    currentPoint2 = pLine2;
    bExistPoint = false;
    while (currentPoint2 != NULL) {
      if (currentPoint1->x == currentPoint2->x
          && currentPoint1->y == currentPoint2->y) {
        bExistPoint = true;
        break;
      }
      currentPoint2 = currentPoint2->next;
    }
        
    if (bExistPoint == false) {
      return false;
    }
        
    currentPoint1 = currentPoint1->next;
  }
    
  return true;
}

POINT* getPointToKillAlive3Line(GOMOKU* puzzle, int putStoneType) {
  GOMOKU* assumptionPuzzle;
  POINTLIST* p3Line;
  POINTLIST* killPointList;
  POINT* splitPoint;
  POINT* killPoint;
  POINT* enemyPutPoint;
  int killedStoneType = getOppositeStone(putStoneType);
  bool bExistLine;
    
  int numOf3Line = 0;
  POINTLIST* p3Lines[10]; //11個以上同時に三作られると困るけど、難しそう

  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == killedStoneType) {
        //TODO: 線の重複をみてない
        p3Line = findAliveSplitLine(puzzle, i, j, killedStoneType, 3);
        if (p3Line != NULL) {
          //すでに線があるか調べる
          bExistLine = false;
          for (int i = 0; i < numOf3Line; i++) {
            if (areTheSameLine(p3Line, p3Lines[i]) == true) {
              bExistLine = true;
              break;
            }
          }
          if (bExistLine == false) {
            p3Lines[numOf3Line] = p3Line;
            numOf3Line++;
          }
        }
      }
    }
  }
    
  //2つ以上あった場合は4で止められる場所がないか先に調べる
  if (numOf3Line >= 2) {
    for (int i = 0; i < numOf3Line; i++) {
      killPoint = getKillPointOfAlive3LineWith4Line(puzzle, p3Lines[i], killedStoneType);
      if (killPoint != NULL) {
        for (int i = 0; i < numOf3Line; i++) {
          freePointList(p3Lines[i]);
        }
        return killPoint;
      }
      if (i > 0) {
        //このループで処理できなかったら負け　0だけとっておく
        freePointList(p3Lines[i]);
      }
    }
  }
    

    
  //4で止められる場所がなかったか、1つしかないなら普通に処理
  if (numOf3Line >= 1) {
    p3Line = p3Lines[0];
        
    splitPoint = getSplitPoint(p3Line);
    if (splitPoint != NULL) {
      if (isBannedPoint(puzzle, splitPoint->x, splitPoint->y, putStoneType) == false) {
        freePointList(p3Line);
        return splitPoint;
      }
      free(splitPoint);
    }
        
    killPointList = getKillPointListOf3LineWithoutSplitPoint(puzzle, p3Line, killedStoneType);
    freePointList(p3Line);
        
    if (killPointList != NULL) {
      int numOfKillPoint = lengthOfPointList(killPointList);
            
      if (numOfKillPoint == 2) {
        //置いたら相手が最終形を作れる方にはおかない
        assumptionPuzzle = copyPuzzle(puzzle);
        assumptionPuzzle->board[killPointList->x][killPointList->y] = putStoneType;
        enemyPutPoint = getPointToMakeFinalStyle(assumptionPuzzle, getOppositeStone(putStoneType), false);
        freePuzzle(assumptionPuzzle);
        if (enemyPutPoint != NULL) {
          free(enemyPutPoint);
          if (isBannedPoint(puzzle, killPointList->next->x, killPointList->next->y, putStoneType) == false) {
            killPoint = (POINT *)malloc(sizeof(POINT));
            killPoint->x = killPointList->next->x;
            killPoint->y = killPointList->next->y;
            freePointList(killPointList);
            return killPoint;
          }
        }
                
        assumptionPuzzle = copyPuzzle(puzzle);
        assumptionPuzzle->board[killPointList->next->x][killPointList->next->y] = putStoneType;
        enemyPutPoint = getPointToMakeFinalStyle(assumptionPuzzle, getOppositeStone(putStoneType), false);
        freePuzzle(assumptionPuzzle);
        if (enemyPutPoint != NULL) {
          free(enemyPutPoint);
          if (isBannedPoint(puzzle, killPointList->x, killPointList->y, putStoneType) == false) {
            killPoint = (POINT *)malloc(sizeof(POINT));
            killPoint->x = killPointList->x;
            killPoint->y = killPointList->y;
            freePointList(killPointList);
            return killPoint;
          }
        }
                
        //最終形関係なかったらどっちでもいいからおけるとこにおく
        if (isBannedPoint(puzzle, killPointList->x, killPointList->y, putStoneType) == false) {
          killPoint = (POINT *)malloc(sizeof(POINT));
          killPoint->x = killPointList->x;
          killPoint->y = killPointList->y;
          freePointList(killPointList);
          return killPoint;
        } else if (isBannedPoint(puzzle, killPointList->next->x, killPointList->next->y, putStoneType) == false) {
          killPoint = (POINT *)malloc(sizeof(POINT));
          killPoint->x = killPointList->next->x;
          killPoint->y = killPointList->next->y;
          freePointList(killPointList);
          return killPoint;
        }
      }
            
      freePointList(killPointList);
    }
  }
    
  return NULL;
}

bool isAliveLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType, bool check2Line) {
  int lineLength = lengthOfPointList(pLine);
  POINT* splitPoint;
  int numOf2Blank;
    
  if (lineLength == 4) {
    splitPoint = getSplitPoint(pLine);
    if (splitPoint != NULL) {
      free(splitPoint);
      return true;
    }
        
    if (numOfAlivePoint(puzzle, pLine, lineStoneType) > 0) {
      return true;
    }
  } else if (lineLength == 3) {
    splitPoint = getSplitPoint(pLine);
    if (splitPoint != NULL) {
      if (numOfAlivePoint(puzzle, pLine, lineStoneType) == 2) {
        free(splitPoint);
        return true;
      }
    } else {
      if (numOf2BlankPointListBesideLine(puzzle, pLine, lineStoneType) >= 3) {
        return true;
      }
    }
  } else if (check2Line == true && lineLength == 2) {
    splitPoint = getSplitPoint(pLine);
    numOf2Blank = numOf2BlankPointListBesideLine(puzzle, pLine, lineStoneType);
    if (splitPoint != NULL) {
      if (numOf2Blank >= 3) {
        free(splitPoint);
        return true;
      }
      free(splitPoint);
    } else {
      if (numOf2Blank == 4) {
        return true;
      }
    }
  }
  return false;
}

bool isAbleToBeInterruptedLine(GOMOKU* puzzle, POINTLIST* pLine, int lineStoneType, int minimumLength) {
  int lineLength = lengthOfPointList(pLine);
    
  if (lineLength == 3) {
    POINTLIST* killPointList;
    POINTLIST* currentPoint;
    killPointList = getKillPointListOf3Line(puzzle, pLine, lineStoneType);
    currentPoint = killPointList;
    while (currentPoint != NULL) {
      if (isAliveLinePoint(puzzle, currentPoint->x, currentPoint->y, getOppositeStone(lineStoneType), minimumLength) == true) {
        freePointList(killPointList);
        return true;
      }
      currentPoint = currentPoint->next;
    }
    freePointList(killPointList);
  } else if (lineLength == 4) {
    POINT* killPoint = getPointToKill4Line(puzzle, getOppositeStone(lineStoneType));
    if (killPoint != NULL) {
      if (isAliveLinePoint(puzzle, killPoint->x, killPoint->y, getOppositeStone(lineStoneType), minimumLength) == true) {
        return true;
      }
      free(killPoint);
    }
  }
  return false;
}

bool isAliveLinePoint(GOMOKU* puzzle, int x, int y, int lineStoneType, int minimumLength) {
  GOMOKU* assumptionPuzzle;
  POINTLIST* pLine;
  bool bRet = false;
    
  if (puzzle->board[x][y] == S_BLANK) {
    assumptionPuzzle = copyPuzzle(puzzle);
    assumptionPuzzle->board[x][y] = lineStoneType;
    pLine = findAliveSplitLine(assumptionPuzzle, x, y, lineStoneType, 4);
    if (pLine == NULL && minimumLength == 3) {
      
      pLine = findAliveSplitLine(assumptionPuzzle, x, y, lineStoneType, 3);
    }
    if (pLine != NULL) {
      freePointList(pLine);
      bRet = true;
    }
    freePuzzle(assumptionPuzzle);
  }

  return bRet;
}

//四三と四四をfinalStyleとする
bool isFinalStylePoint(GOMOKU* puzzle, int x, int y, int putStoneType, bool bAllow3x3) {
  POINTLIST* pLine;
  GOMOKU* assumptionPuzzle;
  POINT* putPoint;
  int fatalCount = 0;
  int numOf3Line = 0;

  assumptionPuzzle = copyPuzzle(puzzle);
  assumptionPuzzle->board[x][y] = putStoneType;
    
  pLine = getSplitVerticalLine(assumptionPuzzle, x, y, putStoneType);
  if (isAliveLine(puzzle, pLine, putStoneType, false) == true) {
    fatalCount++;
    if (lengthOfPointList(pLine) == 3) {
      numOf3Line++;
    }
  }
  freePointList(pLine);
    
  pLine = getSplit45DegreeLine(assumptionPuzzle, x, y, putStoneType);
  if (isAliveLine(puzzle, pLine, putStoneType, false) == true) {
    fatalCount++;
    if (lengthOfPointList(pLine) == 3) {
      numOf3Line++;
    }
  }
  freePointList(pLine);
    
  pLine = getSplitHorizontalLine(assumptionPuzzle, x, y, putStoneType);
  if (isAliveLine(puzzle, pLine, putStoneType, false) == true) {
    fatalCount++;
    if (lengthOfPointList(pLine) == 3) {
      numOf3Line++;
    }
  }
  freePointList(pLine);

  pLine = getSplit135DegreeLine(assumptionPuzzle, x, y, putStoneType);
  if (isAliveLine(puzzle, pLine, putStoneType, false) == true) {
    fatalCount++;
    if (lengthOfPointList(pLine) == 3) {
      numOf3Line++;
    }
  }
  freePointList(pLine);
    
  freePuzzle(assumptionPuzzle);

  if (fatalCount >= 2) {
    //禁手をはじく
    if (isBannedPoint(puzzle, x, y, putStoneType) == false) {
      return false;
    } else {
      //KillAlive3Lineより三三が優先されるとまずいので場合によってははじく
      if (bAllow3x3 == false && numOf3Line == 2) {
        return false;
      }
            
      //四四以外で、相手が止めたときに四ができる場合、かつことができないのではじく
      if (numOf3Line > 0) {
        assumptionPuzzle = copyPuzzle(puzzle);
        assumptionPuzzle->board[x][y] = putStoneType;
        letAIPutStone(assumptionPuzzle, getOppositeStone(putStoneType), LEVEL_DEFENCE_ONLY);//これはパズルモードで使う関数なのでMAXにできない
        putPoint = getPointToKill4Line(assumptionPuzzle, putStoneType);
        freePuzzle(assumptionPuzzle);
        if (putPoint != NULL) {
          free(putPoint);
          return false;
        }
      }
            
      return true;
    }
  } else {
    return false;
  }
}

POINT* getPointToMakeFinalStyle(GOMOKU* puzzle, int putStoneType, bool bAllow3x3) {
    
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == S_BLANK) {
        //すべての空白を調べるのはやめたほうがいいかも
        if (isFinalStylePoint(puzzle, i, j, putStoneType, bAllow3x3) == true) {
          POINT* finalStylePoint = (POINT *)malloc(sizeof(POINT));
          finalStylePoint->x = i;
          finalStylePoint->y = j;
          return finalStylePoint;
        }
      }
    }
  }
  return NULL;
}

bool isPointToPrepareToMakeFinalStyleWith4Line(GOMOKU* puzzle, int x, int y, int putStoneType) {
  POINTLIST* aliveLine;
  GOMOKU* assumptionPuzzle;
  POINT* finalStylePoint;
  bool bRet = false;
    
  assumptionPuzzle = copyPuzzle(puzzle);
  assumptionPuzzle->board[x][y] = putStoneType;
  aliveLine = findAliveSplitLine(assumptionPuzzle, x, y, putStoneType, 4);
  if (aliveLine != NULL) {
    letAIPutStone(assumptionPuzzle, getOppositeStone(putStoneType), LEVEL_MAX);//パズル生成用関数なのでMAX
    finalStylePoint = getPointToMakeFinalStyle(assumptionPuzzle, putStoneType, false);
    if (finalStylePoint != NULL) {
      bRet = true;
      free(finalStylePoint);
    }
  }
    
  freePuzzle(assumptionPuzzle);
  freePointList(aliveLine);
  return bRet;
}

bool isPointToPrepareToMakeFinalStyleWith3Line(GOMOKU* puzzle, int x, int y, int putStoneType) {
  POINTLIST* aliveLine;
  GOMOKU* assumptionPuzzle;
  POINT* finalStylePoint;
  bool bRet = false;
    
  assumptionPuzzle = copyPuzzle(puzzle);
  assumptionPuzzle->board[x][y] = putStoneType;
  aliveLine = findAliveSplitLine(assumptionPuzzle, x, y, putStoneType, 3);
  if (aliveLine != NULL) {
    letAIPutStone(assumptionPuzzle, getOppositeStone(putStoneType), LEVEL_MAX);//パズル生成用の関数なのでMAX
    finalStylePoint = getPointToMakeFinalStyle(assumptionPuzzle, putStoneType, true);
    if (finalStylePoint != NULL) {
      bRet = true;
      free(finalStylePoint);
    }
  }
    
  freePuzzle(assumptionPuzzle);
  freePointList(aliveLine);
  return bRet;
}

POINT* getPointToPrepareToMakeFinalStyleWith4Line(GOMOKU* puzzle, int putStoneType) {
  POINT* pointToMakeFinalStylePoint;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == S_BLANK && isPointToPrepareToMakeFinalStyleWith4Line(puzzle, i, j, putStoneType) == true && isBannedPoint(puzzle, i, j, putStoneType) == false) {
        pointToMakeFinalStylePoint = (POINT *)malloc(sizeof(POINT));
        pointToMakeFinalStylePoint->x = i;
        pointToMakeFinalStylePoint->y = j;
        return pointToMakeFinalStylePoint;
      }
    }
  }
  return NULL;
}

//TODO: これを完成させる どうみてもメモリリークする　再帰折りたたむときにpointListをへらさないと
//再帰着火

POINT* getPointToMakeFinalStyleWith4LineRecursive(GOMOKU* puzzle, int putStoneType) {
  GOMOKU* assumptionPuzzle;
  POINT* pointToMakeFinalStyle;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == S_BLANK
          && isBannedPoint(puzzle, i, j, putStoneType) == false) {
        assumptionPuzzle = copyPuzzle(puzzle);
        assumptionPuzzle->board[i][j] = putStoneType; //ここに石があったら最終型が作れるか
        pointToMakeFinalStyle = getPointToMakeFinalStyle(assumptionPuzzle, putStoneType, false);
        //作れるなら再帰を走らせる
        if (pointToMakeFinalStyle != NULL) {
          POINTLIST* finalStylePointList = (POINTLIST *)malloc(sizeof(POINTLIST));
          finalStylePointList->x = pointToMakeFinalStyle->x;
          finalStylePointList->y = pointToMakeFinalStyle->y;
          finalStylePointList->next = NULL;
          free(pointToMakeFinalStyle);
                    
          pointToMakeFinalStyle = getPointToMakePointListWith4LineRecursieve(puzzle, finalStylePointList, putStoneType);
          freePointList(finalStylePointList);
          if (pointToMakeFinalStyle != NULL) {
            return pointToMakeFinalStyle;
          }
        }
      }
    }
  }
  return NULL;
}

POINT* getPointToMakeFinalStyleWith3LineRecursive(GOMOKU* puzzle, int putStoneType) {
  GOMOKU* assumptionPuzzle;
  POINT* pointToMakeFinalStyle;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == S_BLANK
          && isBannedPoint(puzzle, i, j, putStoneType) == false) {
        assumptionPuzzle = copyPuzzle(puzzle);
        assumptionPuzzle->board[i][j] = putStoneType; //ここに石があったら最終型が作れるか
        pointToMakeFinalStyle = getPointToMakeFinalStyle(assumptionPuzzle, putStoneType, true);
        //作れるなら再帰を走らせる
        if (pointToMakeFinalStyle != NULL) {
          POINTLIST* finalStylePointList = (POINTLIST *)malloc(sizeof(POINTLIST));
          finalStylePointList->x = pointToMakeFinalStyle->x;
          finalStylePointList->y = pointToMakeFinalStyle->y;
          finalStylePointList->next = NULL;
          free(pointToMakeFinalStyle);
                    
          pointToMakeFinalStyle = getPointToMakePointListWith3LineRecursieve(puzzle, finalStylePointList, putStoneType);
          freePointList(finalStylePointList);
          if (pointToMakeFinalStyle != NULL) {
            return pointToMakeFinalStyle;
          }
        }
      }
    }
  }
  return NULL;
}

//再帰するやつ
POINT* getPointToMakePointListWith4LineRecursieve(GOMOKU* puzzle, POINTLIST* pointList, int putStoneType) {
  POINT* pointToMakeFinalStyle;
    
  //再帰に制限をつける
  if (lengthOfPointList(pointList) > RECURSIVE_LIMIT) {
    return NULL;
  }
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == S_BLANK
          && isInPointList(pointList, i, j) == false
          && isBannedPoint(puzzle, i, j, putStoneType) == false) {
        if (isEndPointToMakePointListWith4Line(puzzle, pointList, i, j, putStoneType) == true) {
          pointToMakeFinalStyle = (POINT *)malloc(sizeof(POINT));
          pointToMakeFinalStyle->x = i;
          pointToMakeFinalStyle->y = j;
          return pointToMakeFinalStyle;
        } else if (isPointToMakePointList(puzzle, pointList, i, j, putStoneType, 4) == true) {
          POINTLIST* newPoint = (POINTLIST *)malloc(sizeof(POINTLIST));
          newPoint->x = i;
          newPoint->y = j;
          newPoint->next = pointList;
          pointToMakeFinalStyle = getPointToMakePointListWith4LineRecursieve(puzzle, newPoint, putStoneType);
          if (pointToMakeFinalStyle != NULL) {
            return pointToMakeFinalStyle;
          }
        }
      }
    }
  }
  return NULL;
}

POINT* getPointToMakePointListWith3LineRecursieve(GOMOKU* puzzle, POINTLIST* pointList, int putStoneType) {
  POINT* pointToMakeFinalStyle;
    
  //再帰に制限をつける
  if (lengthOfPointList(pointList) > RECURSIVE_LIMIT) {
    return NULL;
  }
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == S_BLANK
          && isInPointList(pointList, i, j) == false
          && isBannedPoint(puzzle, i, j, putStoneType) == false) {
        if (isEndPointToMakePointListWith3Line(puzzle, pointList, i, j, putStoneType) == true) {
          pointToMakeFinalStyle = (POINT *)malloc(sizeof(POINT));
          pointToMakeFinalStyle->x = i;
          pointToMakeFinalStyle->y = j;
          return pointToMakeFinalStyle;
        } else if (isPointToMakePointList(puzzle, pointList, i, j, putStoneType, 3) == true) {
          POINTLIST* newPoint = (POINTLIST *)malloc(sizeof(POINTLIST));
          newPoint->x = i;
          newPoint->y = j;
          newPoint->next = pointList;
          pointToMakeFinalStyle = getPointToMakePointListWith3LineRecursieve(puzzle, newPoint, putStoneType);
          if (pointToMakeFinalStyle != NULL) {
            return pointToMakeFinalStyle;
          }
        }
      }
    }
  }
  return NULL;
}

bool isInPointList(POINTLIST* pointList, int x, int y) {
  POINTLIST* currentPoint = pointList;
  bool bRet = false;
    
  while (currentPoint != NULL) {
    if (currentPoint->x == x && currentPoint->y == y) {
      bRet = true;
      break;
    }
    currentPoint = currentPoint->next;
  }
    
  return bRet;
}

//コア関数
//TODO: ちゃんとつくる interruptedまわり
bool isEndPointToMakePointListWith4Line(GOMOKU* puzzle, POINTLIST* pointList, int x, int y, int putStoneType) {
  POINTLIST* aliveLine;
  POINT* putPoint;
  GOMOKU* assumptionPuzzle;
  bool dummy;
  bool bRet = false;
    
  assumptionPuzzle = copyPuzzle(puzzle);
  assumptionPuzzle->board[x][y] = putStoneType;
  aliveLine = findAliveSplitLine(assumptionPuzzle, x, y, putStoneType, 4);
  if (aliveLine != NULL) {
    freePointList(aliveLine);
    putPoint = getAIPutPoint(assumptionPuzzle, getOppositeStone(putStoneType), &dummy, 2);//ここでMAXにするとやばい
    if (putPoint != NULL && isInPointList(pointList, putPoint->x, putPoint->y) == false) {
      assumptionPuzzle->board[putPoint->x][putPoint->y] = getOppositeStone(putStoneType);
      free(putPoint);
      assumptionPuzzle->board[pointList->x][pointList->y] = putStoneType;
      aliveLine = findAliveSplitLine(assumptionPuzzle, x, y, putStoneType, 4);
      if (aliveLine != NULL) {
        freePointList(aliveLine);
        bRet = true;
      }
    }
        
  }
    
  freePuzzle(assumptionPuzzle);
  return bRet;
}

bool isEndPointToMakePointListWith3Line(GOMOKU* puzzle, POINTLIST* pointList, int x, int y, int putStoneType) {
  POINTLIST* aliveLine;
  POINT* putPoint;
  GOMOKU* assumptionPuzzle;
  bool dummy;
  bool bRet = false;
    
  assumptionPuzzle = copyPuzzle(puzzle);
  assumptionPuzzle->board[x][y] = putStoneType;
  aliveLine = findAliveSplitLine(assumptionPuzzle, x, y, putStoneType, 4);
  if (aliveLine == NULL) {
    aliveLine = findAliveSplitLine(assumptionPuzzle, x, y, putStoneType, 3);
  }
  if (aliveLine != NULL) {
    freePointList(aliveLine);
    putPoint = getAIPutPoint(assumptionPuzzle, getOppositeStone(putStoneType), &dummy, 2);//ここでMAXにするとやばい
    if (putPoint != NULL && isInPointList(pointList, putPoint->x, putPoint->y) == false) {
      assumptionPuzzle->board[putPoint->x][putPoint->y] = getOppositeStone(putStoneType);
      free(putPoint);
      assumptionPuzzle->board[pointList->x][pointList->y] = putStoneType;
      aliveLine = findAliveSplitLine(assumptionPuzzle, x, y, putStoneType, 4);
      if (aliveLine == NULL) {
        aliveLine = findAliveSplitLine(assumptionPuzzle, x, y, putStoneType, 3);
      }
      if (aliveLine != NULL) {
        freePointList(aliveLine);
        bRet = true;
      }
    }
        
  }
    
  freePuzzle(assumptionPuzzle);
  return bRet;
}

//もしそこにあったらうれしい点を探す関数
bool isPointToMakePointList(GOMOKU* puzzle, POINTLIST* pointList, int x, int y, int putStoneType, int minimumLength) {
  POINTLIST* aliveLine;
  GOMOKU* assumptionPuzzle;
  bool bRet = false;
    
  if (isInPointList(pointList, x, y) == true) {
    return false;
  }
    
  assumptionPuzzle = copyPuzzle(puzzle);
  assumptionPuzzle->board[x][y] = putStoneType;//もしここにあったら
  assumptionPuzzle->board[pointList->x][pointList->y] = putStoneType;//ここが好きにおけて嬉しい
  aliveLine = findAliveSplitLine(assumptionPuzzle, x, y, putStoneType, 4);
  if (aliveLine == NULL && minimumLength == 3) {
    aliveLine = findAliveSplitLine(assumptionPuzzle, x, y, putStoneType, 3);
  }
  if (aliveLine != NULL
      && isAbleToBeInterruptedLine(assumptionPuzzle, aliveLine, putStoneType, minimumLength) == false) {
    freePointList(aliveLine);
    bRet = true;        
  }
    
  freePuzzle(assumptionPuzzle);
  return bRet;
}

POINT* getPointToPrepareToMakeFinalStyleWith3Line(GOMOKU* puzzle, int putStoneType) {
  POINT* pointToMakeFinalStylePoint;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == S_BLANK && isPointToPrepareToMakeFinalStyleWith3Line(puzzle, i, j, putStoneType) == true && isBannedPoint(puzzle, i, j, putStoneType) == false) {
        pointToMakeFinalStylePoint = (POINT *)malloc(sizeof(POINT));
        pointToMakeFinalStylePoint->x = i;
        pointToMakeFinalStylePoint->y = j;
        return pointToMakeFinalStylePoint;
      }
    }
  }
  return NULL;
}

POINT* getPointToKillFinalStyle(GOMOKU* puzzle, int putStoneType) {
  int killedStoneType = getOppositeStone(putStoneType);
    
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == S_BLANK) {
        //すべての空白を調べるのはやめたほうがいいかも
        if (isFinalStylePoint(puzzle, i, j, killedStoneType, true) == true) {
          POINT* finalStylePoint = (POINT *)malloc(sizeof(POINT));
          finalStylePoint->x = i;
          finalStylePoint->y = j;
          return finalStylePoint;
        }
      }
    }
  }
  return NULL;
}

POINT* getPointToMake3Line(GOMOKU* puzzle, int putStoneType) {
  POINTLIST* p2Line;
  POINT* alivePoint;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == putStoneType) {
        p2Line = findAliveSplitLine(puzzle, i, j, putStoneType, 2);
        if (p2Line != NULL) {
          alivePoint = findAlivePointOf2Line(puzzle, p2Line, putStoneType);
          freePointList(p2Line);
          if (alivePoint != NULL) {
            return alivePoint;
          }
        }
      }
    }
  }
    
  return NULL;
}

POINT* getPointToKillDead3Line(GOMOKU* puzzle, int putStoneType) {
  POINTLIST* p3Line;
  POINT* killPoint;
  int killedStoneType = getOppositeStone(putStoneType);
    
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == killedStoneType) {
        p3Line = findSplit3Line(puzzle, i, j, killedStoneType);
        if (p3Line != NULL) {
          killPoint = getKillPointOf3Line(puzzle, p3Line, killedStoneType);
                    
          //おいたら三三の場合はあきらめる
          if (killPoint != NULL && isBannedPoint(puzzle, killPoint->x, killPoint->y, putStoneType)) {
            free(killPoint);
            killPoint = NULL;
          }
                    
          freePointList(p3Line);
                    
          if (killPoint != NULL) {
            return killPoint;
          }
        }
      }
    }
  }
    
  return NULL;
}

//getPointToMake2LineWithSeed用
void checkBlankAndAppendPointList(GOMOKU* puzzle, POINTLIST** stoneList, int blankCheckX, int blankCheckY, int putX, int putY) {
  if (isInTheBoard(putX, putY) && puzzle->board[blankCheckX][blankCheckY] == S_BLANK) {
    POINTLIST* newPoint = newPointList();
    newPoint->x = putX;
    newPoint->y = putY;
        
    if (*stoneList == NULL) {
      *stoneList = newPoint;
    } else {
      appendPointList(*stoneList, newPoint);
    }
  }
}

POINT* getPointToMake2LineWithMT(GOMOKU* puzzle, int putStoneType) {
  POINT* putPoint = NULL;
  POINTLIST* stoneList = NULL;
  POINTLIST* newStoneList = NULL;

  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (puzzle->board[i][j] == putStoneType) {
        newStoneList = newPointList();
        newStoneList->x = i;
        newStoneList->y = j;
        if (stoneList == NULL) {
          stoneList = newStoneList;
        } else {
          appendPointList(stoneList, newStoneList);
        }
      }
    }
  }
    
  putPoint = (POINT *)malloc(sizeof(POINT));
    
  //のばす石を乱択
  int numOfStone = lengthOfPointList(stoneList);
  if (numOfStone == 1) {
    putPoint->x = stoneList->x;
    putPoint->y = stoneList->y;
    freePointList(stoneList);
  } else {
    int getStone = genrand_int32() % numOfStone;
    POINTLIST* getStonePoint = stoneList;
    for (int i = 0; i < getStone; i++) {
      getStonePoint = getStonePoint->next;
    }
    putPoint->x = getStonePoint->x;
    putPoint->y = getStonePoint->y;
    freePointList(stoneList);
  }
    
  stoneList = NULL;
    
  //のばす場所のリストを作成
  checkBlankAndAppendPointList(puzzle, &stoneList, putPoint->x - 1, putPoint->y - 1, putPoint->x - 1, putPoint->y - 1);
  checkBlankAndAppendPointList(puzzle, &stoneList, putPoint->x, putPoint->y - 1, putPoint->x, putPoint->y - 1);
  checkBlankAndAppendPointList(puzzle, &stoneList, putPoint->x + 1, putPoint->y - 1, putPoint->x + 1, putPoint->y - 1);
  checkBlankAndAppendPointList(puzzle, &stoneList, putPoint->x - 1, putPoint->y, putPoint->x - 1, putPoint->y);
  checkBlankAndAppendPointList(puzzle, &stoneList, putPoint->x + 1, putPoint->y, putPoint->x + 1, putPoint->y);
  checkBlankAndAppendPointList(puzzle, &stoneList, putPoint->x - 1, putPoint->y + 1, putPoint->x - 1, putPoint->y + 1);
  checkBlankAndAppendPointList(puzzle, &stoneList, putPoint->x, putPoint->y + 1, putPoint->x, putPoint->y + 1);
  checkBlankAndAppendPointList(puzzle, &stoneList, putPoint->x + 1, putPoint->y + 1, putPoint->x + 1, putPoint->y + 1);
    
  //のばす場所を乱択
  numOfStone = lengthOfPointList(stoneList);
  if (numOfStone == 0) {
    return NULL;
  } else if (numOfStone == 1) {
    putPoint->x = stoneList->x;
    putPoint->y = stoneList->y;
    freePointList(stoneList);
  } else {
    int getStone = genrand_int32() % numOfStone;
    POINTLIST* getStonePoint = stoneList;
    for (int i = 0; i < getStone; i++) {
      getStonePoint = getStonePoint->next;
    }
    putPoint->x = getStonePoint->x;
    putPoint->y = getStonePoint->y;
    freePointList(stoneList);
  }
    

  return putPoint;
}

//意味もなくてきとうにおく
POINT* getPointWithoutThought(GOMOKU* puzzle) {
  for (int i = 4; i < 9; i++) {
    for (int j = 7; j < 14; j++) {
      if (puzzle->board[i][j] == S_BLANK) {
        POINT* putPoint = (POINT *)malloc(sizeof(POINT));
        putPoint->x = i;
        putPoint->y = j;
        return putPoint;
      }
    }
  }
    
  return NULL;
}

double intervalTime(clock_t start, clock_t end) {
  return (double)(end-start)/CLOCKS_PER_SEC;
}

//=======================
//　　　AIのメイン関数
//=======================
//　AILevelで強さが変わります。
//
// ・特殊レベル
// LEVEL_PUZZLE_MODE : パズルで使うレベル
// LEVEL_DEFENCE_ONLY: PUZZLE_MODEで呼ばれる関数で速度を制限しつつ変なところに置かないようにするための値
// LEVEL_MAX         : 現行実装された最強のレベル　思考速度最遅
//
// ・実装済みレベル
// LEVEL 0: 4があっても無意味にのばさないが　隙があったら3を作る ほぼ防御オンリー
// LEVEL 1: 詰ませにいく最低のレベル　最終形が作れるなら作る　ここからLEVEL_MAXの間で生成するパズルに幅をもたせる
// LEVEL 2: 最終形の一手前から詰ませることができる
// LEVEL 3: 再帰による無限読み(時間がかかるので使わない)
//
POINT* getAIPutPoint(GOMOKU* puzzle, int putStoneType, bool* bAbleToWin, int AILevel) {
  POINT* putPoint;
  int num;
    
  //生成にのみ使うのでMAX
  if (AILevel == LEVEL_MAX) {
    num = stoneNum(puzzle);
        
    if (num == 0) {
      return getCenterPointOfTheBoard();
    } else if (num == 1) {
      return getPointAroundCenterWithMT();
    }
  }
    
  putPoint = getPointToMake5Line(puzzle, putStoneType);
  if (putPoint != NULL) {
    return putPoint;
  }
    
  putPoint = getPointToKill4Line(puzzle, putStoneType);
  if (putPoint != NULL) {
    return putPoint;
  }
    
  //相手の手数を増やせるが…生成にのみ使う
  if (AILevel == LEVEL_MAX) {
    if (num < 10) {
      putPoint = getPointToMake4Line(puzzle, putStoneType);
      if (putPoint != NULL) {
        return putPoint;
      }
    }
  }
    
  putPoint = getPointToMake4LineFromAlive3Line(puzzle, putStoneType);
  if (putPoint != NULL) {
    return putPoint;
  }
    
  //最終形が作れるなら作る
  if (AILevel >= 1) {
    putPoint = getPointToMakeFinalStyle(puzzle, putStoneType, false);//白の三三を作らない
    if (putPoint != NULL) {
      *bAbleToWin = true;
      return putPoint;
    }
  }
    
  //最終形の一手前から最終形を作りにいく
  if (AILevel == 2) {
    putPoint = getPointToPrepareToMakeFinalStyleWith4Line(puzzle, putStoneType);
    if (putPoint != NULL) {
      *bAbleToWin = true;
      return putPoint;
    }
  }
    
  //無限再帰
  if (AILevel >= 3) {
    putPoint = getPointToMakeFinalStyleWith4LineRecursive(puzzle, putStoneType);
    if (putPoint != NULL) {
      *bAbleToWin = true;
      return putPoint;
    }
  }
    
  putPoint = getPointToKillAlive3Line(puzzle, putStoneType);
  if (putPoint != NULL) {
    return putPoint;
  }
    
  //最終形破壊　必ず要る
  putPoint = getPointToKillFinalStyle(puzzle, putStoneType);
  if (putPoint != NULL) {
    return putPoint;
  }
    
  //三三用
  if (AILevel >= 1) {
    //三三を作る用なので白でなければただの重複
    if (putStoneType == S_WHITE) {
      putPoint = getPointToMakeFinalStyle(puzzle, putStoneType, true);//白の三三を許す
      if (putPoint != NULL) {
        *bAbleToWin = true;
        return putPoint;
      }
    }
  }
    
  //最終形の一手前から最終形を作りにいく
  if (AILevel == 2) {
    putPoint = getPointToPrepareToMakeFinalStyleWith3Line(puzzle, putStoneType);
    if (putPoint != NULL) {
      *bAbleToWin = true;
      return putPoint;
    }
  }
    
  //無限再帰
  if (AILevel >= 3) {
    putPoint = getPointToMakeFinalStyleWith3LineRecursive(puzzle, putStoneType);
    if (putPoint != NULL) {
      *bAbleToWin = true;
      return putPoint;
    }
  }
    
  putPoint = getPointToMake3Line(puzzle, putStoneType);
  if (putPoint != NULL) {
    return putPoint;
  }
    
  //ここまできてしまった場合はどうしようもないので乱数に頼る
  putPoint = getPointToMake2LineWithMT(puzzle, putStoneType);
  if (putPoint != NULL) {
    return putPoint;
  }
    
  putPoint = getPointWithoutThought(puzzle);
  if (putPoint != NULL) {
    return putPoint;
  }
    
  return NULL;
}

POINT* getCenterPointOfTheBoard() {
  POINT* putPoint = (POINT *)malloc(sizeof(POINT));
  putPoint->x = (BOARD_SIZE - 1) / 2;
  putPoint->y = (BOARD_SIZE - 1) / 2;
    
  return putPoint;
}

POINT* getPointAroundCenterWithMT() {
  POINT* putPoint = getCenterPointOfTheBoard();
  VECTOR changeVector;
    
  switch (genrand_int32() % 8) {
  case 0:
    changeVector.x = -1;
    changeVector.y = -1;
    break;
  case 1:
    changeVector.x = 0;
    changeVector.y = -1;
    break;
  case 2:
    changeVector.x = 1;
    changeVector.y = -1;
    break;
  case 3:
    changeVector.x = -1;
    changeVector.y = 0;
    break;
  case 4:
    changeVector.x = 1;
    changeVector.y = 0;
    break;
  case 5:
    changeVector.x = -1;
    changeVector.y = 1;
    break;
  case 6:
    changeVector.x = 0;
    changeVector.y = 1;
    break;
  case 7:
    changeVector.x = 1;
    changeVector.y = 1;
    break;
  }
    
  putPoint->x += changeVector.x;
  putPoint->y += changeVector.y;
    
  return putPoint;
}

//この手で勝ちが決まる場合trueを返す
bool letAIPutStone(GOMOKU* puzzle, int putStoneType, int AILevel) {
  POINT* putPoint;
  bool bAbleToWin = false;
    
  putPoint = getAIPutPoint(puzzle, putStoneType, &bAbleToWin, AILevel);
    
  if (putPoint != NULL) {
    putStone(puzzle, putPoint->x, putPoint->y, putStoneType);
  }

  return bAbleToWin;
}

void printBoard(GOMOKU* puzzle) {
  char printChar;
  printf("432109876543210\n");
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      switch (puzzle->board[BOARD_SIZE - j - 1][i]) {
      case S_BLANK:
        printChar = '.';
        break;
      case S_WHITE:
        printChar = 'o';
        break;
      case S_BLACK:
        printChar = 'x';
        break;
      default:
        break;
      }
      putchar(printChar);
    }
    printf("%d", i);
    putchar('\n');
  }
  putchar('\n');
}

void showClearProcess(int mode, int stage) {
  GOMOKU* puzzle;
  int putStoneType;
    
  puzzle = getBoardData(mode, stage);
  putStoneType = puzzle->playStoneType;
    
  printBoard(puzzle);
    
  for (int i = 0; i < BOARD_SIZE * BOARD_SIZE / 2; i++) {
    letAIPutStone(puzzle, putStoneType, LEVEL_MAX);//デバッグ用なのでMAX　そもそもMAXじゃないと解けないはず
    printBoard(puzzle);
    if (existFiveLine(puzzle, putStoneType) == true) {
      break;
    }
        
    letAIPutStone(puzzle, getOppositeStone(putStoneType), LEVEL_MAX);
    printBoard(puzzle);
    if (existFiveLine(puzzle, putStoneType) == true) {
      printf("This is a bug puzzle.\n");
      break;
    }
  }
}

GOMOKU* generatePuzzle(unsigned long seed, bool bShowProcess) {
  GOMOKU* puzzle = (GOMOKU *)malloc(sizeof(GOMOKU));
    
  //乱数初期化
  init_genrand(seed);
    
  //初期化
  puzzle->playStoneType = S_BLACK;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      puzzle->board[i][j] = S_BLANK;
    }
  }
    
  puzzle->whiteStoneHistory = NULL;
  puzzle->blackStoneHistory = NULL;

  bool bBlackWin = false;
  bool bWinningRun = false;
  int stoneNum = -1;
    
  //AIにプレーさせる
  for (int i = 0; i < BOARD_SIZE * BOARD_SIZE / 2; i++) {
    if (letAIPutStone(puzzle, S_BLACK, LEVEL_MAX) == true && bWinningRun == false) {
      if (bShowProcess == true) {
        printf("\nI can solve the puzzle.\n");
      }
      bBlackWin = true;
      bWinningRun = true;
      stoneNum = 0;
    }
        
    if (bShowProcess == true) {
      printBoard(puzzle);
    }
        
    if (bWinningRun == true && bBlackWin == true) {
      stoneNum++;
    }
        
    if (existFiveLine(puzzle, S_BLACK) == true) {
      bBlackWin = true;
      break;
    }
        
    if (letAIPutStone(puzzle, S_WHITE, LEVEL_MAX) == true && bWinningRun == false) {
      if (bShowProcess == true) {
        printf("\nI can solve the puzzle.\n");
      }
      bBlackWin = false;
      bWinningRun = true;
      stoneNum = 0;
    }
        
    if (bShowProcess == true) {
      printBoard(puzzle);
    }
        
    if (bWinningRun == true && bBlackWin == false) {
      stoneNum++;
    }
        
    if (existFiveLine(puzzle, S_WHITE) == true) {
      bBlackWin = false;
      break;
    }
  }
    
  if (stoneNum != -1) {
    if (bBlackWin == true) {
      oneStepBack(puzzle, S_BLACK);
      for (int i = 0; i < stoneNum -1; i++) {
        oneStepBack(puzzle, S_WHITE);
        oneStepBack(puzzle, S_BLACK);
      }
      puzzle->playStoneType = S_BLACK;
    } else {
      oneStepBack(puzzle, S_WHITE);
      for (int i = 0; i < stoneNum - 1; i++) {
        oneStepBack(puzzle, S_BLACK);
        oneStepBack(puzzle, S_WHITE);
      }
      puzzle->playStoneType = S_WHITE;
    }
  }
    
  freePointList(puzzle->blackStoneHistory);
  freePointList(puzzle->whiteStoneHistory);
  puzzle->blackStoneHistory = NULL;
  puzzle->whiteStoneHistory = NULL;
    
  puzzle->numOfStone = stoneNum;
    
  return puzzle;
}

int numOfCertainTypeStoneOnTheBoard(GOMOKU* puzzle, int stoneType) {
  int num = 0;
    
  if (puzzle != NULL) {
    for (int i = 0; i < BOARD_SIZE; i++) {
      for (int j = 0; j < BOARD_SIZE; j++) {
        if (puzzle->board[i][j] == stoneType) {
          num++;
        }
      }
    }
  }
    
  return num;
}


int numOfStoneOnTheBoard(GOMOKU* puzzle) {
  int num = 0;
    
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      switch (puzzle->board[i][j]) {
      case S_WHITE:
      case S_BLACK:
        num++;
        break;
      default:
        break;
      }
    }
  }
    
  return num;
}

int compareStoneNum(const POINT* a, const POINT* b) {
  return a->y - b->y;
}
