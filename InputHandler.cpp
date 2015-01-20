#include "InputHandler.h"
#include "Game.h"

InputHandler::InputHandler() : m_keystates(0) {}

InputHandler::~InputHandler() {
  delete m_keystates;
}

void InputHandler::onKeyDown() {
  m_keystates = SDL_GetKeyboardState(0);
  if (isKeyDown(SDL_SCANCODE_ESCAPE)) {
    TheGame::Instance()->quit();
  }
}
void InputHandler::onKeyUp() {
  m_keystates = SDL_GetKeyboardState(0);
}

bool InputHandler::isKeyDown(SDL_Scancode key) const {
  if (m_keystates != 0) {
    if (m_keystates[key] == 1) {
      return true;
    } else {
      return false;
    }
  }
  return false;
}

InputHandler *InputHandler::m_instance = 0;

void InputHandler::update() {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT: TheGame::Instance()->quit(); break;
      case SDL_KEYDOWN: onKeyDown(); break;
    }
  }
}
void InputHandler::waitForReturn() {
  while(true) {
    update();
    if (isKeyDown(SDL_SCANCODE_RETURN)) {
      break;
    }
    SDL_Delay(50);
  }
}
