#include "StateMachine.h"

StateMachine *StateMachine::m_instance = 0;

void StateMachine::push(State *state) {
  m_states.push_back(state);
  m_states.back()->onEnter();
}
void StateMachine::pop() {
  if (!m_states.empty()) {
    m_states.back()->onExit();
    delete m_states.back();
    m_states.pop_back();
  }
}
void StateMachine::change(State *state) {
  pop();
  push(state);
}
void StateMachine::update() {
  if (!m_states.empty()) {
    m_states.back()->update();
  }
}
void StateMachine::render() {
  if (!m_states.empty()) {
    m_states.back()->render();
  }
}
