#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <vector>
#include "State.h"

class StateMachine {
public:
  static StateMachine* Instance() {
    if (!m_instance) {
      m_instance = new StateMachine();
    }
    return m_instance;
  }
  void push(State *state);
  void pop();
  void change(State *state);

  void update();
  void render();
private:
  StateMachine() {}
  ~StateMachine() {}

  static StateMachine *m_instance;

  std::vector<State*> m_states;
};

typedef StateMachine TheStateMachine;

#endif /* end of include guard: __STATE_MACHINE_H__ */
