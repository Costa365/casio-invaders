#include "attack.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iostream>
#include <algorithm>

Attack::Attack(int length) : length(length) {
  reset();
}

void Attack::reset() {
  score = 0;
  level = 0;
  launches = 0;
  attack.clear();
  nextMove();
}

int Attack::getScore() {
  return score;
}

int Attack::getLaunches() {
  return launches;
}

void Attack::fire(int target) {
  ++launches;
  auto iter = std::find(attack.begin(), attack.end(), target);
  if (iter != attack.end()) {
      attack.erase(iter);
      ++score;
  }
}

void Attack::nextMove() {
  if (attack.size() <= length){
    attack.push_back(rand() % 11);
  }
}

std::string Attack::getAttack() const {
  std::stringstream ss;
  for (int num : attack) {
    (num==10)?ss << 'n':ss << num;
  }
  return ss.str();
}
