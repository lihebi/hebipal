#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__

#include <SDL.h>

class InputHandler {
public:
  static InputHandler* Instance() {
    if (!m_instance) {
      m_instance = new InputHandler();
    }
    return m_instance;
  }
  void update();
  void onKeyDown();
  void onKeyUp();
  bool isKeyDown(SDL_Scancode key) const;
private:
  InputHandler();
  ~InputHandler();
  static InputHandler* m_instance;
  const uint8_t *m_keystates;
};

typedef InputHandler TheInputHandler;

#endif /* end of include guard: __INPUT_HANDLER_H__ */
