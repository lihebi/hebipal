#ifndef __OPENING_MENU_STATE_H__
#define __OPENING_MENU_STATE_H__

#include "State.h"

class OpeningMenuState : public State {
public:
  virtual void update();
  virtual void render();
  virtual void onEnter();
  virtual void onExit();
};

#endif /* end of include guard: __OPENING_MENU_STATE_H__ */
