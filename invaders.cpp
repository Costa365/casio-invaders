#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <string>
#include <iomanip>
#include <ncurses.h>
#include "attack.h"

using namespace std;

char key = 0;
std::mutex m;
const int maxNums = 8;
int score = 0;
int gameOver = false;
const int sleepus = 1000;
double hitRate = 0.0;

char getLastKeyPress() {
  std::lock_guard<std::mutex> lock(m);
  char last_key = key;
  key = 0;
  return last_key;
}

char UpdateKey(char newKey) {
  std::lock_guard<std::mutex> lock(m);
  key = newKey;
  return 0;
}

void getKeyPress() {
  // Init ncurses mode
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);

  int ch=0;
  
  while(ch!='q' && !gameOver) {
    while ((ch = getch()) == ERR && !gameOver) {
      usleep(sleepus);
    }
    UpdateKey(ch);
  }
  // End curses mode
  endwin(); 
}

void game() {
  int frame=0;
  int playNum=0;
  char dispNum='0';
  char ch=0;
  string nums="";
  Attack attack;
  const int baseMoveRate = 1000;
  int moveRate = 0;

  do {
    ch=getLastKeyPress();
    if(ch=='z') {
      playNum=(++playNum)%11;
      playNum==10?dispNum='n':dispNum=playNum+'0';
    }
    if(ch==' ') {
      attack.fire(playNum);
    }

    nums = attack.getAttack();
    std::cout << " " << dispNum << " ≡ " << std::setfill(' ') <<
      std::setw(maxNums+1) << nums << "\r" << std::flush;

    usleep(sleepus);
    moveRate = baseMoveRate - (attack.getScore()/5)*50;
    if(++frame % moveRate == 0){
      attack.nextMove();
    }
  }while(ch!='q' && nums.size()<=maxNums);
  score = attack.getScore();
  hitRate = ((double)score/(double)attack.getLaunches()) * 100.0 ;
  gameOver = true;
}

void printInstructions() {
  std::cout << "Casio Invaders\n\n"
            << "Keys:\n"
            << "  q: Exit\n"
            << "  z: Change number\n"
            << "  <space>: Fire\n";
}

int main(int argc, char* argv[]) {
  srand(time(NULL));
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--help") {
      printInstructions();
      return 0;
    }
  }
  std::thread keyPressThread (getKeyPress);
  std::thread gameThread (game);
  gameThread.join();
  keyPressThread.join();
  std::cout << std::fixed;
  std::cout << std::setprecision(2);
  std::cout << "\rScore: " << score << " | Hit Rate: " << hitRate << "%       \n";
  return 0;
}
