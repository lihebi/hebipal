#ifndef __TEXT_MANAGER_H__
#define __TEXT_MANAGER_H__

#include <SDL.h>

#define WORD_LENGTH 10


#define DESCTEXT_COLOR 0x2e

#define MAINMENU_LABEL_NEWGAME 7
#define MAINMENU_LABEL_LOADGAME 8

class TextManager {
public:
  static TextManager *Instance() {
    if (!m_instance) {
      m_instance = new TextManager();
    }
    return m_instance;
  }
  void init();
  const char* getWord(uint16_t wordId);
  const char* getMsg(uint16_t msgId);
  void drawText(const char* s, int x, int y, uint8_t color);
  void drawTextById(uint16_t wordId, int x, int y, uint8_t color);
  void drawChinese(uint16_t wchar, SDL_Surface *screen, int x, int y, uint8_t color);
  void drawAscii(uint8_t c, SDL_Surface *screen, int x, int y, uint8_t color);
private:
  TextManager() {}
  ~TextManager() {}

  static TextManager *m_instance;

  int m_nWords;
  int m_nMsg;
  uint8_t *m_wordBuf;
  uint8_t *m_msgBuf;
  uint32_t *m_msgOffset;

  uint16_t *m_bufChar;
  uint8_t *m_bufGlyph;
  int m_nChar;
};

typedef TextManager TheTextManager;

#endif /* end of include guard: __TEXT_MANAGER_H__ */
