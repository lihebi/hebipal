#ifndef __MENU_H__
#define __MENU_H__

#include <vector>
#include "../TextManager.h"

#define MENUITEM_COLOR 0x4f
#define MENUITEM_COLOR_INACTIVE 0x1c
#define MENUITEM_COLOR_CONFIRMED 0x2c
#define MENUITEM_COLOR_SELECTED_INACTIVE 0x1f
#define MENUITEM_COLOR_SELECTED_FIRST 0xf9
#define MENUITEM_COLOR_SELECTED_TOTALNUM 6
#define MENUITEM_COLOR_EQUIPPEDITEM 0xc8

class MenuItem {
public:
  MenuItem(uint16_t wordId, int x, int y)
  : wordId(wordId), x(x), y(y), color(MENUITEM_COLOR), active(false) {}
  void activate() {
    color = MENUITEM_COLOR_SELECTED_INACTIVE;
    active = true;
  }
  void inactivate() {
    color = MENUITEM_COLOR;
    active = false;
  }
  void draw() {
    TheTextManager::Instance()->drawTextById(this->wordId, this->x, this->y, this->color);
  }
  bool isActive() {
    return active;
  }
private:
  uint16_t wordId;
  int x;
  int y;
  uint8_t color;
  bool active;
};

class Menu {
public:
  Menu() : m_keydown(false) {}
  ~Menu() {}

  void addItem(MenuItem *item, bool active=false);

  int update();
  void render();
private:
  std::vector<MenuItem*> m_items;

  void doNext();
  void doPrevious();
  void next();
  void previous();

  bool m_keydown;
};

#endif /* end of include guard: __MENU_H__ */
