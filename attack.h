#ifndef _ATTACK_
#define _ATTACK_

#include <vector>
#include <string>

class Attack   
{
public:
  Attack(int length=8);
  void fire(int target);
  int getScore();
  int getLaunches();
  std::string getAttack() const;
  void nextMove();

private:
  std::vector<int> attack;
  int level;
  int length;
  int score;
  int launches;
  void reset();
};
 
#endif