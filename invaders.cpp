#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <string>
#include <iomanip>
#include <ncurses.h>
#include "attack.h"

const int maxAttackNums = 8;
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
  
  while (ch!=quitKey && !gameOver) {
    while ((ch = getch()) == ERR && !gameOver) {
      usleep(sleepus);
    }
    UpdateKey(ch);
  }
}

void displayGame(std::string attackNums, char dispNum) { 
  std::cout << " " << dispNum << " ≡ " << std::setfill(' ') 
            << std::setw(maxAttackNums+1) << attackNums << "\r" << std::flush;
}

void game() {
  const int baseAttackRate = 1000;
  int frame=0, missileNum=0, attackRate=0, lastAttackFrame=0;
  char dispNum='0', keyPress=0;
  std::string attackNums="";
  Attack attack;

  do {
    keyPress=getLastKeyPress();
    if (keyPress == rotateKey) {
      missileNum=(++missileNum)%11;
      missileNum==10?dispNum='n':dispNum=missileNum+'0';
    }
    if (keyPress == fireKey) {
      attack.fire(missileNum);
    }

    attackNums = attack.getAttack();
    displayGame(attackNums, dispNum);
    usleep(sleepus);
    attackRate = baseAttackRate - (attack.getScore()/5)*50;
    ++frame;
    if (frame >= lastAttackFrame+attackRate){
      lastAttackFrame = frame;
      attack.nextMove();
    }
  }while(keyPress!=quitKey && attackNums.size()<=maxAttackNums);
  
  score = attack.getScore();
  if (attack.getLaunches() > 0) {
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
