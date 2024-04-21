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
  level = 0;
  attack.clear();
}

void Attack::fire(int n) {
  auto it = std::find(attack.begin(), attack.end(), n);
  if (it != attack.end()) {
      attack.erase(it);
  }
}

void Attack::nextMove() {
  if(attack.size()<=this->size){
    attack.push_back(rand() % 10);
  }
}

string Attack::getAttack() const{
  std::stringstream ss;
  for (int num : attack) {
      ss << num;
  }
  return ss.str();
}