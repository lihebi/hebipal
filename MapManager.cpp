#include "MapManager.h"
#include "driver.h"
#include "ScreenManager.h"
#include "ResourceManager.h"
#include <iostream>

MapManager *MapManager::m_instance = 0;

Map *load_map(int mapId, FILE *fpMAP, FILE *fpGOP) {
  uint8_t *buf;
  int size;
  Map *map;
  size = mkf_get_chunk_size(mapId, fpMAP);
  buf = (uint8_t*)malloc(size);
  map = (Map*)malloc(sizeof(Map));
  mkf_read_chunk(buf, size, mapId, fpMAP);
  Decompress(buf, (uint8_t*)(map->tiles), sizeof(map->tiles));
  free(buf);

  size = mkf_get_chunk_size(mapId, fpGOP);
  map->sprites = (uint8_t*)malloc(size);
  mkf_read_chunk(map->sprites, size, mapId, fpGOP);
  map->mapId = mapId;
  return map;
}
MapManager::MapManager() : m_map(NULL), m_mapId(0) {}
void MapManager::init() {
}
void MapManager::setViewport(int x, int y) {
  m_viewportX = x;
  m_viewportY = y;
}
void MapManager::getViewport(int *x, int *y) {
  *x = m_viewportX;
  *y = m_viewportY;
}
// load map. If current map is the map, just return.
void MapManager::load(int mapId) {
  if (m_mapId == mapId) return;
  if (m_map && m_map->sprites) free(m_map->sprites);
  free(m_map);
  FILE *fpMAP = fopen("data/MAP.MKF", "rb");
  FILE *fpGOP = fopen("data/GOP.MKF", "rb");
  m_map = load_map(mapId, fpMAP, fpGOP);
  m_mapId = m_map->mapId;
  fclose(fpMAP);
  fclose(fpGOP);
}
uint8_t* sprite_get_frame(uint8_t *sprite, int frameId) {
  int imagecount, offset;
  imagecount = (sprite[0] | (sprite[1] << 8));
  if (frameId<0 || frameId >= imagecount) return NULL;
  frameId <<= 1;
  offset = (uint16_t)((sprite[frameId] | (sprite[frameId+1] << 8)) << 1);
  return &sprite[offset];
}
uint8_t* get_tile_bitmap(uint8_t x, uint8_t y, uint8_t h, uint8_t layer, Map *map) {
  uint32_t d;
  if (x>=64 || y>=128 || h>1 || map==NULL) return NULL;
  d = map->tiles[y][x][h];
  if (layer==0) {
    //bottom layer
    return sprite_get_frame(map->sprites, (d & 0xff) | ((d >> 4) & 0x100));
  } else {
    // top layer
    d >>= 16;
    return sprite_get_frame(map->sprites, ((d & 0xff) | ((d >> 4) & 0x100)) - 1);
  }
}
void MapManager::draw(uint8_t layer) {
  int x,y,h,sx,sy,dx,dy,xPos,yPos;
  const uint8_t *bitmap = NULL;
  SDL_Rect rect = {0, 0, 320, 200};
  // rect.x = m_viewportX;
  // rect.y = m_viewportY;

  int tmp = 11535488;
  rect.x = tmp & 0xffff;
  rect.y = tmp >> 16;

  sy = rect.y / 16 -1;
  dy = (rect.y + rect.h) / 16 + 2;
  sx = rect.x / 32 - 1;
  dx = (rect.x + rect.w) / 32 + 2;


  yPos = sy * 16 - 8 - rect.y;
  for (y=sy;y<dy;y++) {
    for (h=0;h<2;h++,yPos+=8) {
      xPos = sx * 32 + h * 16 - 16 - rect.x;
      for (x=sx;x<dx;x++, xPos += 32) {
        bitmap = get_tile_bitmap((uint8_t)x, (uint8_t)y, (uint8_t)h, layer, m_map);
        if (bitmap == NULL) {
          if (layer) {
            continue;
          }
          bitmap = get_tile_bitmap(0, 0, 0, layer, m_map);
        }
        rle_blit_to_surface(bitmap, TheScreenManager::Instance()->getScreen(), xPos, yPos);
      }
    }
  }
}
