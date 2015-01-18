#include "PlayState.h"
#include <iostream>

void PlayState::update() {}
void PlayState::render() {}

void PlayState::onEnter() {
  std::cout<<"enter play state"<<std::endl;
}
void PlayState::onExit() {
  std::cout<<"exit play state"<<std::endl;
}
