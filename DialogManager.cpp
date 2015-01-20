#include "DialogManager.h"

#include "driver.h"
#include "ResourceManager.h"
#include "ScreenManager.h"
#include "TextManager.h"
#include "InputHandler.h"

#include <iostream>

DialogManager *DialogManager::m_instance = 0;

DialogManager::DialogManager() {
  m_color = FONT_COLOR_DEFAULT;
  m_text = NULL;
}

void DialogManager::start(uint8_t location, uint8_t color, int charId) {
  uint8_t buf[16384];
  SDL_Rect rect;
  m_icon = 0;
  m_iconX = m_iconY = 0;
  m_line = 0;
  m_titleX = 12;
  m_titleY = 8;
  m_userSkip = false;

  m_color = color!=0 ? color : m_color;

  switch (location) {
    case DIALOG_UPPER:
      if (charId > 0) {
        mkf_read_chunk(buf, 16384, charId, TheResourceManager::Instance()->fpRGM);
        rect.w = rle_get_width((const uint8_t *)buf);
        rect.h = rle_get_height((const uint8_t *)buf);
        rect.x = 48 - rect.w / 2;
        rect.y = 55 - rect.h / 2;
        rect.x = rect.x<0 ? 0 : rect.x;
        rect.y = rect.y<0 ? 0 : rect.y;
        rle_blit_to_surface((const uint8_t *)buf, TheScreenManager::Instance()->getScreen(), rect.x, rect.y);
      }
      m_titleX = charId>0 ? 80 : 12;
      m_titleY = 8;
      m_textX = charId>0 ? 96 : 44;
      m_textY = 26;
      break;
    case DIALOG_CENTER:
      m_textX = 80;
      m_textY = 40;
      break;
    case DIALOG_LOWER:
      if (charId > 0) {
        mkf_read_chunk(buf, 16384, charId, TheResourceManager::Instance()->fpRGM);
        rect.x = 270 - rle_get_width((const uint8_t *)buf) / 2;
        rect.y = 144 - rle_get_width((const uint8_t *)buf) / 2;
        rle_blit_to_surface((const uint8_t *)buf, TheScreenManager::Instance()->getScreen(), rect.x, rect.y);
      }
      m_titleX = charId>0 ? 4 : 12;
      m_titleY = 108;
      m_textX = charId>0 ? 20 : 44;
      m_textY = 126;
      break;
    case DIALOG_CENTER_WINDOW:
      m_textX = 160;
      m_textY = 40;
      break;
  }
  m_location = location;
}

void DialogManager::showText(const char *text) {
  char tmp[3];

  if (m_line>3) {
    m_line = 0;
    while(true) {
      TheInputHandler::Instance()->update();
      if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN)) {
        break;
      }
      SDL_Delay(50);
    }
    TheScreenManager::Instance()->update();
  }
  int x = m_textX;
  int y = m_textY + m_line * 18;
  while(text != NULL && *text != '\0') {
    switch (*text) {
      case '-':
        // set font color to cyan
        m_color = m_color == FONT_COLOR_CYAN ? FONT_COLOR_DEFAULT : FONT_COLOR_CYAN;
        text++;
        break;
      case '\'':
        // set font color to red
        m_color = m_color == FONT_COLOR_RED ? FONT_COLOR_DEFAULT : FONT_COLOR_RED;
        text++;
        break;
      case '\"':
        // set font color to yellow
        m_color = m_color == FONT_COLOR_YELLOW ? FONT_COLOR_DEFAULT : FONT_COLOR_YELLOW;
        text++;
        break;
      case '$':
        // set delay time of text-displaying
        m_delay = atoi(text+1) * 10 / 7;
        text += 3;
        break;
      case '~':
        // delay for a period and quit
        SDL_Delay(atoi(text+1) * 80 / 7);
        return;
      case ')':
        // TODO set waiting icon
        text++;
        break;
      case '(':
        // TODO set waiting icon
        text++;
        break;
      case '\\':
        text++;
      default:
        if (*text & 0x80) {
          tmp[0] = text[0];
          tmp[1] = text[1];
          tmp[2] = '\0';
          text += 2;
        } else {
          tmp[0] = *text;
          tmp[1] = '\0';
          text++;
        }
        TheTextManager::Instance()->drawText(tmp, x, y, m_color);
        x += (text[0] & 0x80) ? 16 : 8;
        // draw character one by one
        SDL_Delay(30);
        // while (true) {
        //   TheInputHandler::Instance()->update();
        //   if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN)) break;
        //   SDL_Delay(50);
        // }
    }
  }
  m_line++;
}

// clear state of dialog
void DialogManager::clear() {
  m_line = 0;
  if (m_location == DIALOG_CENTER) {
    m_titleX = 12;
    m_titleY = 8;
    m_textX = 44;
    m_textY = 26;
    m_color = FONT_COLOR_DEFAULT;
    m_location = DIALOG_UPPER;
  }
}
