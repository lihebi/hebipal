#include "TextManager.h"
#include "ResourceManager.h"
#include "ScreenManager.h"
#include "driver.h"
#include "ascii.h"
// #include "big5font.h"
#include "gbfont.h"
#include <iostream>
#include <cstdio>
TextManager *TextManager::m_instance = 0;

void TextManager::init() {
  // open files
  FILE *fpMsg = fopen("data/M.MSG", "rb");
  FILE *fpWord = fopen("data/WORD.DAT", "rb");
  // read nWord
  fseek(fpWord, 0, SEEK_END);
  int i = ftell(fpWord);
  m_nWords = (i + (WORD_LENGTH -1)) / WORD_LENGTH;
  m_wordBuf = (uint8_t*)malloc(i);
  // read wordBuf
  fseek(fpWord, 0, SEEK_SET);
  fread(m_wordBuf, i, 1, fpWord);
  fclose(fpWord);
  // read msg offset
  i = mkf_get_chunk_size(3, TheResourceManager::Instance()->fpSSS) / sizeof(uint32_t);
  m_nMsg = i-1;
  m_msgOffset = (uint32_t*)malloc(i*sizeof(uint32_t));
  mkf_read_chunk((uint8_t*)m_msgOffset, i*sizeof(uint32_t), 3, TheResourceManager::Instance()->fpSSS);
  // read msg
  fseek(fpMsg, 0, SEEK_END);
  i = ftell(fpMsg);
  m_msgBuf = (uint8_t*)malloc(i);
  fseek(fpMsg, 0, SEEK_SET);
  fread(m_msgBuf, 1, i, fpMsg);
  fclose(fpMsg);
}
void trim(char *str) {
  int pos=0;
  char *dest = str;
  while(str[pos] <= ' ' && str[pos] >0) {
    pos++;
  }
  while (str[pos]) {
    *(dest++) = str[pos];
    pos++;
  }
  *(dest--) = '\0';
  while(dest >= str && *dest <= ' ' && *dest >0) {
    *(dest--) = '\0';
  }
}
const char* TextManager::getWord(uint16_t wordId) {
  // char buf[WORD_LENGTH +1];
  // char *buf = (char*)malloc(WORD_LENGTH+1);
  static char buf[WORD_LENGTH+1];
  memcpy(buf, &m_wordBuf[wordId*WORD_LENGTH], WORD_LENGTH);
  buf[WORD_LENGTH] = '\0';
  trim(buf);
  if ((strlen(buf) & 1) != 0 && buf[strlen(buf)-1] == '1') {
    buf[strlen(buf)-1] = '\0';
  }
  return buf;
}
void TextManager::drawText(const char *s, int x, int y, uint8_t color) {
  while(*s) {
    if (*s & 0x80) {
      // chinese
      uint16_t wchar = ((uint8_t*)s)[0] | ((uint8_t*)s)[1] << 8;
      drawChinese(wchar, TheScreenManager::Instance()->getScreen(), x, y, color);
      s += 2;
      x += 16;
    } else {
      drawAscii(*s, TheScreenManager::Instance()->getScreen(), x, y, color);
      s++;
      x += 8;
    }
  }
}
void TextManager::drawTextById(uint16_t wordId, int x, int y, uint8_t color) {
  drawText(getWord(wordId), x, y, color);
  printf("%x\n", getWord(7));
}
void TextManager::drawChinese(uint16_t wchar, SDL_Surface *screen, int x, int y, uint8_t color) {
  std::cout<<"drawing chinese"<<wchar<<std::endl;
  int dx;
  uint8_t *p;
  uint8_t ch1,ch2;
  ch1 = wchar & 0xff;
  ch2 = wchar >> 8;
  // p = &big5font[((ch1 - 0xa1) * 157 + ch2 - 0x40) << 5] + 8;
  // p = &big5font[((ch1 - 0xa1) * 157 +63 + ch2 - 0xa1) << 5] + 8;
  p = &gbfont[((ch1 -0xa1) *94 + (ch2 - 0xa1) ) *32];
  y *= screen->pitch;
  for (int i=0;i<32;i++) {
    dx = x + ((i & 1) << 3);
    for (int j=0;j<8;j++) {
      if (p[i] & (1 << (7-j))) {
        if (dx<screen->w) {
          ((char*)(screen->pixels))[y+dx] = color;
        } else {
          break;
        }
      }
      dx++;
    }
    y += (i & 1) * screen->pitch;
    if (y / screen->pitch >= screen->h) {
      break;
    }
  }
}
void TextManager::drawAscii(uint8_t c, SDL_Surface *screen, int x, int y, uint8_t color) {
  int dx;
  uint8_t *p = &iso_font[(int)(c & ~128) * 15];
  y *= screen->pitch;
  for (int i=0;i<15;i++) {
    dx = x;
    for (int j=0;j<8;j++) {
      if (p[i] & (1<<j)) {
        ((char*)(screen->pixels))[y+dx] = color;
      }
      dx++;
    }
    y += screen->pitch;
  }
}
