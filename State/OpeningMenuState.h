#ifndef __OPENING_MENU_STATE_H__
#define __OPENING_MENU_STATE_H__

#include "State.h"
#include <vector>
#include "../Object/Menu.h"

class OpeningMenuState : public State {
public:
  virtual void update();
  virtual void render();
  virtual void onEnter();
  virtual void onExit();
  OpeningMenuState() {}
  ~OpeningMenuState() {}
private:
  Menu *m_menu;
};

#endif /* end of include guard: __OPENING_MENU_STATE_H__ */
