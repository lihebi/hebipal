#ifndef __SCREEN_MANAGER_H__
#define __SCREEN_MANAGER_H__

#include <SDL.h>

class ScreenManager {
public:
  static ScreenManager* Instance() {
    if (!m_instance) {
      m_instance = new ScreenManager();
    }
    return m_instance;
  }
  void init(SDL_Window *window, SDL_Renderer *renderer);
  void render();
  void clean();
  void palette();
  SDL_Surface* getScreen() {
    return m_screen;
  }
  SDL_Texture* getTexture() {
    return m_texture;
  }
private:
  static ScreenManager *m_instance;

  static SDL_Window *m_window;
  static SDL_Renderer *m_renderer;

  SDL_Surface *m_screen;
  SDL_Surface *m_screenReal;
  SDL_Texture *m_texture;
  SDL_Color *m_color;
};

typedef ScreenManager TheScreenManager;

#endif /* end of include guard: __SCREEN_MANAGER_H__ */
