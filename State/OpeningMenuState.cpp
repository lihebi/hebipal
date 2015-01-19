#include <iostream>
#include <cstdlib>

#include "OpeningMenuState.h"
#include "StateMachine.h"
#include "PlayState.h"
#include "../Game.h"
#include "../ResourceManager.h"
#include "../ScreenManager.h"
#include "../TextManager.h"
#include "../InputHandler.h"
#include "../driver.h"
#define MAINMENU_BACKGROUND 60
void drawBackground() {
  uint8_t *buf;
  buf = (uint8_t*)malloc(320*200);
  mkf_decompress_chunk(buf, 320*200, MAINMENU_BACKGROUND, TheResourceManager::Instance()->fpFBP);
  fbp_blit_to_surface(buf, TheScreenManager::Instance()->getScreen());
  free(buf);
}
void OpeningMenuState::update() {
  int i = m_menu->update();
  if (i==0) {
    std::cout<<"new game"<<std::endl;
    TheStateMachine::Instance()->change(new PlayState());
  } else if (i==1) {
    std::cout<<"load game"<<std::endl;
  }
}
void OpeningMenuState::render() {
  m_menu->render();
}
void OpeningMenuState::onEnter() {
  std::cout<<"enter opening menu state"<<std::endl;
  drawBackground();
  TheScreenManager::Instance()->palette();
  m_menu = new Menu();
  m_menu->addItem(new MenuItem(MAINMENU_LABEL_NEWGAME, 125, 95), true);
  m_menu->addItem(new MenuItem(MAINMENU_LABEL_LOADGAME, 125, 112));
}
void OpeningMenuState::onExit() {
  std::cout<<"exit opening menu state"<<std::endl;
}
