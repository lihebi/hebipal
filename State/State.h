#ifndef __STATE_H__
#define __STATE_H__

class State {
public:
  virtual ~State() {}
  
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void onEnter() = 0;
  virtual void onExit() = 0;
};

#endif /* end of include guard: __STATE_H__ */
