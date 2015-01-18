#ifndef __PLAY_STATE_H__
#define __PLAY_STATE_H__

#include "State.h"

class PlayState : public State {
public:
  PlayState() {}
  ~PlayState() {}

  virtual void update();
  virtual void render();

  virtual void onEnter();
  virtual void onExit();
};

#endif /* end of include guard: __PLAY_STATE_H__ */
