#include "TextManager.h"
#include "ResourceManager.h"
#include "ScreenManager.h"
#include "driver.h"
#include "ascii.h"
#include <iostream>
#include <cstdio>
#include <cassert>
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
  // fonts
  FILE *fp;
  fp = fopen("data/wor16.asc", "rb");
  fseek(fp, 0, SEEK_END);
  m_nChar = ftell(fp);
  m_nChar /= 2;

  m_bufChar = (uint16_t*)calloc(m_nChar, sizeof(uint16_t));
  fseek(fp, 0, SEEK_SET);
  fread(m_bufChar, sizeof(uint16_t), m_nChar, fp);
  fclose(fp);

  fp = fopen("data/wor16.fon", "rb");
  m_bufGlyph = (uint8_t*)calloc(m_nChar, 30);
  fseek(fp, 0x682, SEEK_SET);
  fread(m_bufGlyph, 30, m_nChar, fp);
  fclose(fp);
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
  static char buf[WORD_LENGTH+1];
  memcpy(buf, &m_wordBuf[wordId*WORD_LENGTH], WORD_LENGTH);
  buf[WORD_LENGTH] = '\0';

  trim(buf);
  if ((strlen(buf) & 1) != 0 && buf[strlen(buf)-1] == '1') {
    buf[strlen(buf)-1] = '\0';
  }
  return buf;
}
const char* TextManager::getMsg(uint16_t msgId) {
  static char buf[256];
  uint32_t offset,size;
  if (msgId > m_nMsg) {
    return NULL;
  }
  offset = m_msgOffset[msgId];
  size = m_msgOffset[msgId + 1] - offset;
  assert(size < 255);
  memcpy(buf, &m_msgBuf[offset], size);
  buf[size] = '\0';
  return buf;
}

void TextManager::drawText(const char *s, int x, int y, uint8_t color) {
  while(*s) {
    if (*s & 0x80) {
      // chinese
      uint16_t wchar = ((uint8_t*)s)[0] | (((uint8_t*)s)[1] << 8);
      drawChinese(wchar, TheScreenManager::Instance()->getScreen(), x, y, color);
      s += 2;
      x += 16;
    } else {
      drawAscii(*s, TheScreenManager::Instance()->getScreen(), x, y, color);
      s++;
      x += 8;
    }
  }
  // render screen after each character display
  TheScreenManager::Instance()->render();
}
void TextManager::drawTextById(uint16_t wordId, int x, int y, uint8_t color) {
  drawText(getWord(wordId), x, y, color);
}
void TextManager::drawChinese(uint16_t wchar, SDL_Surface *screen, int x, int y, uint8_t color) {
  uint8_t *p;
  int i, dx;
  for (i=0;i<m_nChar;i++) {
    if (m_bufChar[i] == wchar) {
      break;
    }
  }
  if (i>m_nChar) return;
  p = m_bufGlyph + i * 30;

  y *= screen->pitch;
  for (int i=0;i<30;i++) {
    dx = x + ((i & 1) << 3);
    for (int j=0;j<8;j++) {
      if (p[i] & (1 << (7 - j))) {
        ((uint8_t*)(screen->pixels))[y+dx] = color;
      }
      dx++;
    }
    y += (i&1) * screen->pitch;
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
