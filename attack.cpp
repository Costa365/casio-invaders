#include "attack.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

Attack::Attack(int size) {
  reset();
  srand(time(NULL));
  this->size = size;
}

void Attack::reset() {
  score = 0;
  level = 0;
  attack.clear();
}

int Attack::getScore() {
  return score;
}

void Attack::fire(int n) {
  auto it = std::find(attack.begin(), attack.end(), n);
  if (it != attack.end()) {
      attack.erase(it);
      score++;
  }
}

void Attack::nextMove() {
  if(attack.size()<=this->size){
    attack.push_back(rand() % 11);
  }
}

string Attack::getAttack() const{
  std::stringstream ss;
  for (int num : attack) {
    (num==10)?ss << 'n':ss << num;
  }
  return ss.str();
}