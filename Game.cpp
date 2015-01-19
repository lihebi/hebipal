#include "Game.h"
#include "InputHandler.h"
#include "ScreenManager.h"
#include "ResourceManager.h"
#include "TextManager.h"
#include "MapManager.h"
#include "SceneManager.h"
#include "ScriptManager.h"

#include "State/StateMachine.h"
#include "State/OpeningMenuState.h"

#include <iostream>

Game *Game::m_instance = 0;

void Game::init() {
  SDL_Init(SDL_INIT_EVERYTHING);
  // create window and renderer
  TheScreenManager::Instance()->init(m_window, m_renderer);
  m_running = true;
  // open files
  TheResourceManager::Instance()->init();
  // load font and msg file
  TheTextManager::Instance()->init();
  // push statemachine
  TheStateMachine::Instance()->push(new OpeningMenuState());
  // load scenes from file
  TheSceneManager::Instance()->init();
  // load scripts and event objects
  TheScriptManager::Instance()->init();
}
void Game::handleEvent() {
  TheInputHandler::Instance()->update();
}
void Game::update() {
  TheStateMachine::Instance()->update();
}
void Game::render() {
  TheStateMachine::Instance()->render();
  TheScreenManager::Instance()->render();
}
void Game::quit() {
  TheScreenManager::Instance()->clean();
  SDL_Quit();
  m_running = false;
}
