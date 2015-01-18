#ifndef __GAME_H__
#define __GAME_H__

#include <SDL.h>

class Game {
public:
  static Game* Instance() {
    if (!m_instance) {
      m_instance = new Game();
    }
    return m_instance;
  }
  void handleEvent();
  void init();
  void update();
  void render();
  void quit();

  bool running() {
    return m_running;
  }
private:
  Game() {}
  ~Game() {}

  static Game* m_instance;
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  bool m_running;
};

typedef Game TheGame;

#endif /* end of include guard: __GAME_H__ */
