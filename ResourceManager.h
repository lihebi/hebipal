#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <cstdio>

class ResourceManager {
public:
  static ResourceManager* Instance() {
    if (!m_instance) {
      m_instance = new ResourceManager();
    }
    return m_instance;
  }
  void init();
  FILE *fpFBP;
  FILE *fpMGO;
  FILE *fpBALL;
  FILE *fpDATA;
  FILE *fpF;
  FILE *fpFIRE;
  FILE *fpRGM;
  FILE *fpSSS;

private:
  static ResourceManager *m_instance;
};

typedef ResourceManager TheResourceManager;

#endif /* end of include guard: __RESOURCE_MANAGER_H__ */
