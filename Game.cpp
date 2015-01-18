#include "Game.h"
#include "InputHandler.h"
#include "ScreenManager.h"
#include "ResourceManager.h"
#include "TextManager.h"
#include "MapManager.h"

#include "State/StateMachine.h"
#include "State/OpeningMenuState.h"

Game *Game::m_instance = 0;

void Game::init() {
  SDL_Init(SDL_INIT_EVERYTHING);
  TheScreenManager::Instance()->init(m_window, m_renderer);
  m_running = true;
  TheResourceManager::Instance()->init();
  TheTextManager::Instance()->init();
  TheMapManager::Instance()->init();
  TheStateMachine::Instance()->push(new OpeningMenuState());
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
  // SDL_DestroyWindow(m_window);
  // SDL_DestroyRenderer(m_renderer);
  SDL_Quit();
  m_running = false;
}
