#include "ScriptManager.h"
#include <cstdlib>
#include "ResourceManager.h"
#include "ScreenManager.h"
#include "DialogManager.h"
#include "SceneManager.h"
#include "TextManager.h"
#include "InputHandler.h"
#include "driver.h"

#include <iostream>

ScriptManager *ScriptManager::m_instance = 0;
void ScriptManager::init() {
  // allocate eventObjects
  int len = mkf_get_chunk_size(0, TheResourceManager::Instance()->fpSSS);
  m_eventObjects = (EventObject*)malloc(len);
  m_nEventObjects = len / sizeof(EventObject);

  // allocate scriptEntries
  len = mkf_get_chunk_size(4, TheResourceManager::Instance()->fpSSS);
  m_scriptEntries = (ScriptEntry*)malloc(len);
  m_nScriptEntries = len / sizeof(ScriptEntry);
  // load all scriptEntries
  mkf_read_chunk((uint8_t*)m_scriptEntries, m_nScriptEntries * sizeof(ScriptEntry),
  4, TheResourceManager::Instance()->fpSSS);
}
// m_wait will be clear by DialogManager
void ScriptManager::update() {
}

void ScriptManager::render() {}

// int random(int from, int to) {
//   srand(time(NULL));
//   return from + rand() / (RAND_MAX / (to - from + 1));
// }
void ScriptManager::run(uint16_t scriptEntryId, uint16_t eventObjectId) {
  ScriptEntry *script = NULL;
  EventObject *object = NULL;
  bool ended = false;
  uint16_t nextScriptEntryId = scriptEntryId;
  if (eventObjectId != 0) {
    object = &m_eventObjects[eventObjectId - 1];
  }
  while(scriptEntryId != 0 && !ended) {
    script = &m_scriptEntries[scriptEntryId];
    printf("0x%x\n", script->operation);
    switch (script->operation) {
      case 0x0000:
        // stop running
        ended = true;
        break;
      case 0x0001:
        // stop running and replace the entry with next line
        nextScriptEntryId = scriptEntryId + 1;
        ended = true;
        break;
      case 0x0002:
        // stop running and replace the entry with specified one
        if (script->operand[1] == 0 || ++(object->nIdleFrameAuto) < script->operand[1]) {
          TheSceneManager::Instance()->setCurrentScriptOnEntry(script->operand[0]);
          return;
        } else {
          // fail
          object->nIdleFrameAuto = 0;
          scriptEntryId++;
        }
        break;
      case 0x0003:
        // unconditional jump
        if (script->operand[1] == 0 || ++(object->nIdleFrameAuto) < script->operand[1]) {
          scriptEntryId = script->operand[0];
        } else {
          object->nIdleFrameAuto = 0;
          scriptEntryId++;
        }
        break;
      case 0x0004:
        // call script
        run(script->operand[0],
        script->operand[1]==0 ? eventObjectId : script->operand[1]);
        scriptEntryId++;
        return;
      case 0x0005:
        // redraw screen TODO
        // clear_dialog();
        // SDL_BlitSurface();
        // update_screen();
        // m_waitForDialog = true;
        // TheDialogManager::Instance()->wait();
        TheInputHandler::Instance()->waitForReturn();
        TheScreenManager::Instance()->update();
        TheScreenManager::Instance()->render();
        scriptEntryId++;
        break;
      case 0x0006:
        // TODO jump to specified address by specified rate
        // if (random(1,100) >= script->operand[0]) {
        //   // ???
        //   scriptEntryId = script->operand[1];
        //   continue;
        // } else {
        //   scriptEntry++;
        // }
        break;
      case 0x0007:
        // start battle TODO
        scriptEntryId++;
        break;
      case 0x0008:
        // replace entry with next instruction
        scriptEntryId++;
        // nextScriptEntryId = scriptEntryID;
        break;
      case 0x0009:
        // TODO wait for specified number of frames
        scriptEntryId++;
        break;
      case 0x000a:
        // TODO go to specified address if player selected no
        scriptEntryId++;
        break;
      case 0x003b:
        // show dialog in the middle part of the screen
        TheDialogManager::Instance()->clear();
        TheDialogManager::Instance()->start(DIALOG_CENTER, (uint8_t)script->operand[0], 0);
        scriptEntryId++;
        break;
      case 0x003c:
        // show dialog in the upper part of the screen
        TheDialogManager::Instance()->clear();
        TheDialogManager::Instance()->start(DIALOG_UPPER, (uint8_t)script->operand[1], script->operand[0]);
        scriptEntryId++;
        break;
      case 0x003d:
        // show dialog in the  lower part of the screen
        TheDialogManager::Instance()->clear();
        TheDialogManager::Instance()->start(DIALOG_LOWER, (uint8_t)script->operand[1], script->operand[0]);
        scriptEntryId++;
        break;
      case 0x003e:
        // TODO show text in a window at the center of the screen
        scriptEntryId++;
        break;
      case 0x008e:
        // TODO restore the screen
        TheScreenManager::Instance()->update();
        scriptEntryId++;
        break;
      case 0xffff:
        // print dialog text
        TheDialogManager::Instance()->showText(TheTextManager::Instance()->getMsg(script->operand[0]));
        scriptEntryId++;
        break;
      default:
        // TODO clear dialog
        // scriptEntryId = interpretInstruction(scriptEntryId, eventObjectId);
        scriptEntryId++;
        break;
    }
  }
}
