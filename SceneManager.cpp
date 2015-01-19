#include "SceneManager.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "driver.h"

SceneManager *SceneManager::m_instance = 0;

void SceneManager::init() {
  mkf_read_chunk((uint8_t*)(m_scene), sizeof(m_scene), 1, TheResourceManager::Instance()->fpSSS);
  m_sceneId = 1;
  m_entering = true;
}
int SceneManager::getCurrentMapId() {
  return m_scene[m_sceneId - 1].mapId;
}
void SceneManager::update() {
  if (m_entering) {
    m_entering = false;
    TheScriptManager::Instance()->add(m_scene[m_sceneId - 1].scriptOnEnter, 0xffff);
  }
}
// replace current scene's scriptOnEntry. Maybe used for JuQing.
void SceneManager::setCurrentScriptOnEntry(uint16_t scriptId) {
  m_scene[m_sceneId - 1].scriptOnEnter = scriptId;
}
