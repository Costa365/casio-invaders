#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <string>
#include <iomanip>
#include "attack.h"

using namespace std;

char key = 0;
std::mutex m;
const int maxNums = 8;

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
  struct termios oldt, newt;
  char ch=0;
  int frame=0;

  // Save the current terminal settings
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);

  // Set non-blocking mode
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  while (ch!='q') {
    // Check if a key has been pressed
    if (read(STDIN_FILENO, &ch, 1) > 0) {
      keyp(ch);
    }
    usleep(1000);
  }

  // Restore original terminal settings
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
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
}

void info() {
  std::cout << "----------------" << std::endl;
  std::cout << " Casio Invaders " << std::endl << std::endl;
  std::cout << " q: Exit " << std::endl;
  std::cout << " z: Increment " << std::endl;
  std::cout << " <space>: Fire" << std::endl;
  std::cout << "----------------" << std::endl;
}

int main() {
  info();
  std::thread kp (getKeyPress);
  std::thread gm (game);
  gm.join();
  std::cout << std::endl << "----------------" << std::endl;
  std::cout << "   Game Over" << std::endl;
  std::cout << "----------------" << std::endl;
  kp.join();
  return 0;
}
