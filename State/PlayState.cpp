#include "PlayState.h"
#include <iostream>

#include "../MapManager.h"

void PlayState::update() {
  // load_resource();
  // game_update();
  // update_party();
  // make_scene();
}
void PlayState::render() {
  TheMapManager::Instance()->draw(0);
  TheMapManager::Instance()->draw(1);
}

void PlayState::onEnter() {
  std::cout<<"enter play state"<<std::endl;
}
void PlayState::onExit() {
  std::cout<<"exit play state"<<std::endl;
}
