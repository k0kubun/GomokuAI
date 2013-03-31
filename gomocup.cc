#include "gomocup.h"

using namespace std;

void Gomocup::Start() {
  board_ = Board();
  while (true) {
    string line;
    getline(cin, line);
    
    if (line.compare(0, 5, "START") == 0) {
      cout << "OK\n";
    } else if (line.compare(0, 4, "TURN") == 0) {
      ParseTurnString(line, kStoneWhite);
      PrintAIPutPosition(kStoneBlack);
    } else if (line.compare(0, 5, "BOARD") == 0) {
      while (true) {
        getline(cin, line);
        if (line.compare(0, 4, "DONE") == 0) {
          break;
        } else {
          ParseBoardString(line);
        }
      }
      PrintAIPutPosition(kStoneBlack);
    } else if (line.compare(0, 4, "INFO") == 0) {
      // do nothing  
    } else if (line.compare(0, 3, "END") == 0) {
      break;
    } else if (line.compare(0, 5, "ABOUT") == 0) {
      cout << "name=\"Valkyrie\", version=\"1.0\", "
           << "author=\"Takashi Kokubun\", country=\"Japan\"\n";
    } else {
      cout << "UNKNOWN\n";
    }
    fflush(stdout);
  }
}

void Gomocup::PrintAIPutPosition(StoneType play_stone) {
  Position put_position = Brain(play_stone).GetPutPosition(board_);
  board_.set_stone(put_position, play_stone);
  cout << put_position.x << "," << put_position.y << "\n";
}

void Gomocup::ParseTurnString(string turn_string, StoneType opponent_stone) {
  int comma_position = turn_string.find(",");
  int x = atoi(turn_string.substr(5, comma_position - 1).c_str());
  int y = atoi(turn_string.substr(comma_position + 1,
                                  turn_string.length()).c_str());
  board_.set_stone(x, y, opponent_stone);
}

void Gomocup::ParseBoardString(string board_string) {
  StoneType stone_type;
  int first_comma_position = board_string.find(",");
  int second_comma_position =
      board_string.substr(first_comma_position + 1, board_string.length())
      .find(",");
  int x = atoi(board_string.substr(0, first_comma_position - 1).c_str());
  int y = atoi(board_string.substr(first_comma_position + 1, 
                                   second_comma_position - 1).c_str());
  int stone = atoi(board_string.substr(second_comma_position + 1,
                                       board_string.length()).c_str());
  if (stone == 1) {
    stone_type = kStoneBlack;
  } else {
    stone_type = kStoneWhite;
  }
  board_.set_stone(x, y, stone_type);
}

int main() {
  Gomocup gomocup;
  
  gomocup.Start();  
  return 0;
}
