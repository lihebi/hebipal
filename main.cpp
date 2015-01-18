#include "main.h"

int main() {
  TheGame::Instance()->init();
  while(TheGame::Instance()->running()) {
    TheGame::Instance()->handleEvent();
    TheGame::Instance()->update();
    TheGame::Instance()->render();
    SDL_Delay(50);
  }
  return 0;
}
