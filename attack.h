#ifndef _ATTACK_
#define _ATTACK_

#include <vector>
#include <string>

class Attack   
{
public:
  Attack(int size=8);
  void fire(int n);
  void nextMove();
  void reset();
  std::string getAttack() const;

private:
    std::vector<int> attack;
    int level;
    int size;
};
 
#endif