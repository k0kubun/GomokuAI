#ifndef GOMOCUP_H_
#define GOMOCUP_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include "brain.h"

class Gomocup {
 public:
  Gomocup() {};

  void Start();
  void PrintAIPutPosition(StoneType play_stone);
  
 private:
  void ParseTurnString(std::string turn_string, StoneType opponent_stone);
  void ParseBoardString(std::string board_string);
  
  Board board_;
};

#endif
