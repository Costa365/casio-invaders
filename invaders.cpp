#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <string>
#include <iomanip>
#include <ncurses.h>
#include "attack.h"

const int maxNums = 8;
const int sleepus = 1000;
const char rotateKey = 'z', fireKey = ' ', quitKey = 'q';

char key = 0;
std::mutex m;
int score = 0;
int gameOver = false;
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

void initNcursesConsole() {
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
}

void deinitNcursesConsole() {
  endwin();
}

void getKeyPress() {
  int ch=0;
  
  while(ch!=quitKey && !gameOver) {
    while ((ch = getch()) == ERR && !gameOver) {
      usleep(sleepus);
    }
    UpdateKey(ch);
  }
}

void game() {
  int frame=0;
  int playNum=0;
  char dispNum='0';
  char ch=0;
  std::string nums="";
  Attack attack;
  const int baseAttackRate = 1000;
  int attackRate = 0;
  int lastAttackFrame = 0;

  do {
    ch=getLastKeyPress();
    if(ch==rotateKey) {
      playNum=(++playNum)%11;
      playNum==10?dispNum='n':dispNum=playNum+'0';
    }
    if(ch==fireKey) {
      attack.fire(playNum);
    }

    nums = attack.getAttack();
    std::cout << " " << dispNum << " ≡ " << std::setfill(' ') 
              << std::setw(maxNums+1) << nums << "\r" << std::flush;
    usleep(sleepus);
    attackRate = baseAttackRate - (attack.getScore()/5)*50;
    ++frame;
    if(frame >= lastAttackFrame+attackRate){
      lastAttackFrame = frame;
      attack.nextMove();
    }
  }while(ch!=quitKey && nums.size()<=maxNums);
  
  score = attack.getScore();
  if(attack.getLaunches() > 0) {
    hitRate = ((double)score/(double)attack.getLaunches()) * 100.0 ;
  }
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

  initNcursesConsole();
  std::thread keyPressThread (getKeyPress);
  std::thread gameThread (game);
  gameThread.join();
  keyPressThread.join();
  deinitNcursesConsole();

  std::cout << std::fixed << std::setprecision(2) 
            << "\rScore: " << score << " | Hit Rate: " 
            << hitRate << "%       \n";
  return 0;
}
