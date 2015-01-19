#include "PlayState.h"
#include <iostream>

#include "../MapManager.h"
#include "../ScriptManager.h"
#include "../SceneManager.h"
#include "../DialogManager.h"

void PlayState::update() {
  // load map if necessary
  TheMapManager::Instance()->load(TheSceneManager::Instance()->getCurrentMapId());

  // check entering script
  TheSceneManager::Instance()->update();
  // run script
  TheScriptManager::Instance()->update();
  // check for enter
  TheDialogManager::Instance()->update();
}
void PlayState::render() {
  if (!TheDialogManager::Instance()->isReady()) return;
  std::cout<<"draw map"<<std::endl;
  TheMapManager::Instance()->draw(0);
  TheMapManager::Instance()->draw(1);
}

void PlayState::onEnter() {
  // TheMapManager::Instance()->setViewport(0, 0);
  std::cout<<"enter play state"<<std::endl;
  // update because the state enter by another state, and update already skipped.
  update();
}
void PlayState::onExit() {
  std::cout<<"exit play state"<<std::endl;
}
