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

char keyp(char k = 0)
{
  std::lock_guard<std::mutex> lock(m);
  if (k == 0)
  {
    char lastkey = key;
    key = 0;
    return lastkey;
  }
  else
  {
    key = k;
    return 0;
  }
}

void getKeyPress()
{
  // Init ncurses mode
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);

  int ch=0;
  
  while(ch!='q' && !gameOver) {
    while ((ch = getch()) == ERR && !gameOver) {
      usleep(1000);
    }
    keyp(ch);
  }
  // End curses mode
  endwin(); 
}

void game() {
  int frame=0;
  int playNum=0;
  char dispNum='0';
  char c=0;
  string nums="";
  Attack attack;

  do {
    c=keyp();
    if(c=='z') {
      playNum=(++playNum)%11;
      playNum==10?dispNum='n':dispNum=playNum+'0';
    }
    if(c==' ') {
      attack.fire(playNum);
    }

    nums = attack.getAttack();
    std::cout << " " << dispNum << " ≡ " << std::setfill(' ') <<
      std::setw(maxNums+1) << nums << "\r" << std::flush;

    usleep(1000);
    if(++frame % 1000 == 0){
      attack.nextMove();
    }
  }while(c!='q' && nums.size()<=maxNums);
  score = attack.getScore();
  gameOver = true;
}

void info() {
  std::cout << "Casio Invaders" << std::endl << std::endl;
  std::cout << "Keys:" << std::endl;
  std::cout << "  q: Exit" << std::endl;
  std::cout << "  z: Increment" << std::endl;
  std::cout << "  <space>: Fire" << std::endl;
}

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help") {
      info();
      return 0;
    }
  }
  std::thread kp (getKeyPress);
  std::thread gm (game);
  gm.join();
  std::cout << "\rScore: " << score << "          " << std::endl;
  kp.join();
  return 0;
}
