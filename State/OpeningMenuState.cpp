#include <iostream>
#include <cstdlib>

#include "OpeningMenuState.h"
#include "../Game.h"
#include "../ResourceManager.h"
#include "../ScreenManager.h"
#include "../TextManager.h"
#include "../driver.h"

void OpeningMenuState::update() {
  std::cout<<"update opening menu state"<<std::endl;
  uint8_t *buf;
  buf = (uint8_t*)malloc(320*200);
  mkf_decompress_chunk(buf, 320*200, 60, TheResourceManager::Instance()->fpFBP);
  fbp_blit_to_surface(buf, TheScreenManager::Instance()->getScreen());
  free(buf);
  TheScreenManager::Instance()->palette();
  TheTextManager::Instance()->drawTextById(MAINMENU_LABEL_NEWGAME, 20, 20, 0);
}
void OpeningMenuState::render() {
  std::cout<<"render opening menu state"<<std::endl;
  TheScreenManager::Instance()->render();
}
void OpeningMenuState::onEnter() {
  std::cout<<"enter opening menu state"<<std::endl;
}
void OpeningMenuState::onExit() {
  std::cout<<"exit opening menu state"<<std::endl;
}
