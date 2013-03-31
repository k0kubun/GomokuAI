#ifndef GOMOCUP_H_
#define GOMOCUP_H_

class Gomocup {
 public:
  Line() {};

  void Start();
  void SetStone();
  void LetBrainPlay();
 
 private:
  Board board_;
};

#endif
