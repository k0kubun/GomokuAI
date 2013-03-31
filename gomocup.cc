#include "gomocup.h"

using namespace std;

void Gomocup::Start() {
  while (true) {
    Brain brain(kStoneBlack);
    string command;
    getline(cin, command);
    
    if (command.compare(0, 5, "START") == 0) {
      cout << "OK\n";
    } else if (command.compare(0, 4, "TURN") == 0) {
      cout << "1,1\n";
    } else if (command.compare(0, 4, "INFO") == 0) {
      // do nothing  
    } else if (command.compare(0, 3, "END") == 0) {
      break;
    } else if (command.compare(0, 5, "ABOUT") == 0) {
      cout << "name=\"Valkyrie\", version=\"1.0\", "
           << "author=\"Takashi Kokubun\", country=\"Japan\"\n";
    } else {
      cout << "UNKNOWN\n";
    }
    fflush(stdout);
  }
}

void Gomocup::SetStone(int x, int y) {
  ;
}

void Gomocup::LetBrainPlay() {
  ;
}

int main() {
  Gomocup gomocup;
  
  gomocup.Start();  
  return 0;
}
