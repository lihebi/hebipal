#ifndef __MAP_MANAGER_H__
#define __MAP_MANAGER_H__

#include <SDL.h>

typedef struct tagMap {
  uint32_t tiles[128][64][2];
  uint8_t *sprites;
  int mapId;
} Map;
typedef struct tagScene {
  uint16_t mapId;
  uint16_t scriptOnEnter;
  uint16_t scriptOnTeleport;
  uint16_t eventObjectIndex;
} Scene;

class MapManager {
public:
  static MapManager* Instance() {
    if (!m_instance) {
      m_instance = new MapManager();
    }
    return m_instance;
  }
  void init();
  void load();

  Map* getMap() {
    return m_map;
  }
  void draw(uint8_t layer);
private:
  static MapManager *m_instance;

  Map *m_map;
  Scene m_scene[300];
  uint16_t m_sceneId;
};

typedef MapManager TheMapManager;

#endif /* end of include guard: __MAP_MANAGER_H__ */
