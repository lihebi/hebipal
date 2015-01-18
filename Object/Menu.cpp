#include "Menu.h"
#include "../InputHandler.h"

void Menu::addItem(MenuItem *item, bool active) {
  if (active) item->activate();
  m_items.push_back(item);
}
int Menu::update() {
  if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_S)) {
    next();
  } else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_W)) {
    previous();
  } else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN)) {
    for (int i=0;i<m_items.size();i++) {
      if (m_items[i]->isActive()) {
        return i;
      }
    }
  } else {
    m_keydown = false;
  }
  return -1;
}
void Menu::render() {
  for (MenuItem *i : m_items) {
    i->draw();
  }
}
void Menu::previous() {
  if (!m_keydown) {
    m_keydown = true;
    doPrevious();
  }
}
void Menu::doPrevious() {
  int i;
  for (i=m_items.size()-1;i>0;i--) {
    if (m_items[i]->isActive()) {
      m_items[i]->inactivate();
      m_items[i-1]->activate();
      break;
    }
  }
  if (i==0) {
    m_items[0]->inactivate();
    m_items[m_items.size()-1]->activate();
  }
}
void Menu::next() {
  if (!m_keydown) {
    m_keydown = true;
    doNext();
  }
}
void Menu::doNext() {
  int i;
  for (i=0;i<m_items.size()-1;i++) {
    if (m_items[i]->isActive()) {
      m_items[i]->inactivate();
      m_items[i+1]->activate();
      break;
    }
  }
  if (i==m_items.size()-1) {
    m_items[i]->inactivate();
    m_items[0]->activate();
  }
}
