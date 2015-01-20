#include "ScriptManager.h"
#include <cstdlib>
#include "ResourceManager.h"
#include "ScreenManager.h"
#include "DialogManager.h"
#include "SceneManager.h"
#include "TextManager.h"
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
  while(TheDialogManager::Instance()->isReady() && !m_scriptStack.empty()) {
    run();
  }
}

void ScriptManager::render() {}

// int random(int from, int to) {
//   srand(time(NULL));
//   return from + rand() / (RAND_MAX / (to - from + 1));
// }
void ScriptManager::add(uint16_t scriptEntryId, uint16_t eventObjectId) {
  m_scriptStack.push(scriptEntryId);
  // FIXME 0xffff
  m_objectStack.push(eventObjectId);
}
void ScriptManager::run() {
  ScriptEntry *script = NULL;
  EventObject *object = NULL;
  uint16_t scriptEntryId = m_scriptStack.top();
  m_scriptStack.pop();
  uint16_t eventObjectId = m_objectStack.top();
  m_objectStack.pop();
  if (eventObjectId != 0) {
    object = &m_eventObjects[eventObjectId - 1];
  }
  if(scriptEntryId != 0) {
    script = &m_scriptEntries[scriptEntryId];
  }
  // std::cout<<script->operation<<std::endl;
  printf("0x%x\n", script->operation);
  switch (script->operation) {
    case 0x0000:
      // stop running
      break;
    case 0x0001:
      // stop running and replace the entry with next line
      TheSceneManager::Instance()->setCurrentScriptOnEntry(scriptEntryId + 1);
      break;
    case 0x0002:
      // stop running and replace the entry with specified one
      if (script->operand[1] == 0 || ++(object->nIdleFrameAuto) < script->operand[1]) {
        TheSceneManager::Instance()->setCurrentScriptOnEntry(script->operand[0]);
        return;
      } else {
        // fail
        object->nIdleFrameAuto = 0;
        m_scriptStack.push(scriptEntryId+1);
        m_objectStack.push(eventObjectId);
      }
      break;
    case 0x0003:
      // unconditional jump
      if (script->operand[1] == 0 || ++(object->nIdleFrameAuto) < script->operand[1]) {
        scriptEntryId = script->operand[0];
      } else {
        object->nIdleFrameAuto = 0;
        m_scriptStack.push(scriptEntryId+1);
        m_objectStack.push(eventObjectId);
      }
      break;
    case 0x0004:
      // call script
      m_scriptStack.push(scriptEntryId+1);
      m_objectStack.push(eventObjectId);

      m_scriptStack.push(script->operand[0]);
      m_objectStack.push(script->operand[1]==0 ? eventObjectId : script->operand[1]);
      return;
    case 0x0005:
      // redraw screen TODO
      // clear_dialog();
      // SDL_BlitSurface();
      // update_screen();
      // m_waitForDialog = true;
      std::cout<<"wait"<<std::endl;
      TheDialogManager::Instance()->wait();
      m_scriptStack.push(scriptEntryId+1);
      m_objectStack.push(eventObjectId);
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
      m_scriptStack.push(scriptEntryId+1);
      m_objectStack.push(eventObjectId);
      break;
    case 0x000a:
      // TODO go to specified address if player selected no
      scriptEntryId++;
      break;
    case 0x003b:
      // show dialog in the middle part of the screen
      TheDialogManager::Instance()->clear();
      TheDialogManager::Instance()->start(DIALOG_CENTER, (uint8_t)script->operand[0], 0);
      m_scriptStack.push(scriptEntryId+1);
      m_objectStack.push(eventObjectId);
      break;
    case 0x003c:
      // show dialog in the upper part of the screen
      TheDialogManager::Instance()->clear();
      TheDialogManager::Instance()->start(DIALOG_UPPER, (uint8_t)script->operand[1], script->operand[0]);
      m_scriptStack.push(scriptEntryId+1);
      m_objectStack.push(eventObjectId);
      break;
    case 0x003d:
      // show dialog in the  lower part of the screen
      TheDialogManager::Instance()->clear();
      TheDialogManager::Instance()->start(DIALOG_LOWER, (uint8_t)script->operand[1], script->operand[0]);
      m_scriptStack.push(scriptEntryId+1);
      m_objectStack.push(eventObjectId);
      break;
    case 0x003e:
      // TODO show text in a window at the center of the screen
      m_scriptStack.push(scriptEntryId+1);
      m_objectStack.push(eventObjectId);
      break;
    case 0x008e:
      // TODO restore the screen
      TheScreenManager::Instance()->update();
      m_scriptStack.push(scriptEntryId+1);
      m_objectStack.push(eventObjectId);
      break;
    case 0xffff:
      // print dialog text
      TheDialogManager::Instance()->showText(TheTextManager::Instance()->getMsg(script->operand[0]));
      m_scriptStack.push(scriptEntryId+1);
      m_objectStack.push(eventObjectId);
      break;
    default:
      // TODO clear dialog
      // scriptEntryId = interpretInstruction(scriptEntryId, eventObjectId);
      m_scriptStack.push(scriptEntryId+1);
      m_objectStack.push(eventObjectId);
      break;
  }
}
