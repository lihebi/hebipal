#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include <SDL.h>

typedef struct tagScene {
  uint16_t mapId;
  uint16_t scriptOnEnter;
  uint16_t scriptOnTeleport;
  uint16_t eventObjectIndex;
} Scene;

class SceneManager {
public:
  static SceneManager* Instance() {
    if (!m_instance) {
      m_instance = new SceneManager();
    }
    return m_instance;
  }
  void init();
  void update();
  int getCurrentMapId();
  void setCurrentScriptOnEntry(uint16_t scriptId);

private:
  static SceneManager *m_instance;

  Scene m_scene[300];
  uint16_t m_sceneId;

  bool m_entering;
};

typedef SceneManager TheSceneManager;

#endif /* end of include guard: __SCENE_MANAGER_H__ */
