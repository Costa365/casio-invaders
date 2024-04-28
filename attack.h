#ifndef _ATTACK_
#define _ATTACK_

#include <vector>
#include <string>

class Attack   
{
public:
  Attack(int length=8);
  void fire(int target);
  void nextMove();
  void reset();
  int getScore();
  std::string getAttack() const;

private:
    std::vector<int> attack;
    int level;
    int length;
    int score;
};
 
#endif