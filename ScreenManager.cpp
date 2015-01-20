#include "ScreenManager.h"
#include <iostream>
#include <cstdio>

#include "driver.h"
#include "MapManager.h"

ScreenManager *ScreenManager::m_instance = 0;
SDL_Window *ScreenManager::m_window = 0;
SDL_Renderer *ScreenManager::m_renderer = 0;

void ScreenManager::init(SDL_Window *window, SDL_Renderer *renderer) {
  window = SDL_CreateWindow("HebiPAL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 400, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  m_window = window;
  m_renderer = renderer;

  m_screen = SDL_CreateRGBSurface(0, 320, 200, 8, 0, 0, 0, 0);
  m_screenReal = SDL_CreateRGBSurface(0, 320, 200, 32,
  0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888,
  SDL_TEXTUREACCESS_STREAMING, 320, 200);

  m_color = (SDL_Color*)malloc(256*sizeof(SDL_Color));

  palette();
}
void ScreenManager::render() {
  bool locked = false;
  if (SDL_MUSTLOCK(m_screenReal)) {
    if (SDL_LockSurface(m_screenReal)<0) {
      return;
      locked = true;
    }
  }
  SDL_UpperBlit(m_screen, NULL, m_screenReal, NULL);
  if (locked) {
    SDL_UnlockSurface(m_screenReal);
    locked = false;
  }

  SDL_UpdateTexture(m_texture, NULL, m_screenReal->pixels, m_screenReal->pitch);
  SDL_RenderClear(m_renderer);
  SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
  SDL_RenderPresent(m_renderer);
}
void ScreenManager::update() {
  TheMapManager::Instance()->draw(0);
  TheMapManager::Instance()->draw(1);
}
void ScreenManager::clean() {
  SDL_DestroyWindow(m_window);
  SDL_DestroyRenderer(m_renderer);
}
void ScreenManager::palette() {
  FILE *fp = fopen("data/PAT.MKF", "rb");
  uint8_t buf[1536];
  mkf_read_chunk(buf, 1536, 0, fp);
  fclose(fp);
  for (int i=0;i<256;i++) {
    m_color[i].r = buf[i*3]<<2;
    m_color[i].g = buf[i*3+1]<<2;
    m_color[i].b = buf[i*3+2]<<2;
  }
  SDL_Palette *palette = SDL_AllocPalette(256);
  for (int i=0;i<256;i++) {
    palette->colors[i] = m_color[i];
  }
  SDL_SetSurfacePalette(m_screen, palette);
  SDL_SetSurfaceColorMod(m_screen, 0, 0, 0);
  SDL_SetSurfaceColorMod(m_screen, 0xff, 0xff, 0xff);
}
