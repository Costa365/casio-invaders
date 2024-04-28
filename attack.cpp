#include "attack.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

Attack::Attack(int length) : length(length) {
  reset();
}

void Attack::reset() {
  score = 0;
  level = 0;
  attack.clear();
}

int Attack::getScore() {
  return score;
}

void Attack::fire(int target) {
    auto iter = std::find(attack.begin(), attack.end(), target);
    if (iter != attack.end()) {
        attack.erase(iter);
        ++score;
    }
}

void Attack::nextMove() {
  if(attack.size()<=length){
    attack.push_back(rand() % 11);
  }
}

string Attack::getAttack() const {
  std::stringstream ss;
  for (int num : attack) {
    (num==10)?ss << 'n':ss << num;
  }
  return ss.str();
}
