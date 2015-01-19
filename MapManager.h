#ifndef __MAP_MANAGER_H__
#define __MAP_MANAGER_H__

#include <SDL.h>

typedef struct tagMap {
  uint32_t tiles[128][64][2];
  uint8_t *sprites;
  int mapId;
} Map;

class MapManager {
public:
  static MapManager* Instance() {
    if (!m_instance) {
      m_instance = new MapManager();
    }
    return m_instance;
  }
  void init();
  void load(int mapId);

  Map* getMap() { return m_map; }
  void draw(uint8_t layer);
  void setViewport(int x, int y);
  void getViewport(int *x, int *y);
private:
  static MapManager *m_instance;

  MapManager();
  ~MapManager();

  Map *m_map;
  int m_mapId;

  int m_viewportX;
  int m_viewportY;
};

typedef MapManager TheMapManager;

#endif /* end of include guard: __MAP_MANAGER_H__ */
