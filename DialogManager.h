#ifndef __DIALOG_MANAGER_H__
#define __DIALOG_MANAGER_H__

#define FONT_COLOR_CYAN 0x8d
#define FONT_COLOR_CYAN_ALT 0x8c
#define FONT_COLOR_DEFAULT 0x4f
#define FONT_COLOR_YELLOW 0x2d
#define FONT_COLOR_RED 0x1a

#include <SDL.h>

typedef enum tagDialogLocation {
  DIALOG_UPPER = 0,
  DIALOG_CENTER,
  DIALOG_LOWER,
  DIALOG_CENTER_WINDOW
} DialogLocation;

class DialogManager {
public:

  static DialogManager* Instance() {
    if (!m_instance) {
      m_instance = new DialogManager();
    }
    return m_instance;
  }

  void update();
  void render();

  void start(uint8_t location, uint8_t color, int charId);
  void showText(const char *text);
  void clear();
  void wait();
  bool isReady();

private:

  DialogManager();
  ~DialogManager();

  static DialogManager *m_instance;

  // for dialog
  uint8_t m_color;
  int m_delay;
  uint8_t m_location; // upper, center, lower
  int m_line;
  int m_iconX;
  int m_iconY;
  int m_titleX;
  int m_titleY;
  int m_textX; // text x
  int m_textY; // text y
  uint8_t m_icon; // FIXME ID?
  bool m_userSkip;
  uint8_t m_iconBuf[282];

  // FIXME globals
  uint16_t m_paletteId;

  bool m_ready;

  const char *m_text;
};

typedef DialogManager TheDialogManager;

#endif /* end of include guard: __DIALOG_MANAGER_H__ */
