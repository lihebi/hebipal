#ifndef __SCRIPT_MANAGER_H__
#define __SCRIPT_MANAGER_H__

#include <stack>

class ScriptEntry {
public:
  uint16_t operation;
  uint16_t operand[3];
};

class EventObject {
public:
  int16_t vanishTime;
  uint16_t x;
  uint16_t y;
  int16_t layer;
  uint16_t triggerScript;
  uint16_t autoScript;
  int16_t state;
  uint16_t triggerMode;
  uint16_t spriteId;
  uint16_t nSpriteFrame; // number of sprite frames
  uint16_t spritePtrOffset; // FIXME
  uint16_t nSpriteFrameAuto; // number of sprite frames used by auto script
  uint16_t nIdleFrameAuto; // number of idle frames used by auto script
};

class ScriptManager {
public:
  static ScriptManager* Instance() {
    if (!m_instance) {
      m_instance = new ScriptManager();
    }
    return m_instance;
  }
  void init();
  void update();
  void render();
  void add(uint16_t scriptEntryId, uint16_t eventObjectId);
  void run();
private:
  static ScriptManager *m_instance;

  EventObject *m_eventObjects;
  ScriptEntry *m_scriptEntries;

  int m_nEventObjects;
  int m_nScriptEntries;

  uint16_t m_scriptEntryId;
  uint16_t m_eventObjectId;

  // bool m_wait;
  // bool m_waitForDialog;
  // int m_count;

  std::stack<uint16_t> m_scriptStack;
  std::stack<uint16_t> m_objectStack;
};

typedef ScriptManager TheScriptManager;

#endif /* end of include guard: __SCRIPT_MANAGER_H__ */
