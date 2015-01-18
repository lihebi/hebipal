#include "ResourceManager.h"

ResourceManager *ResourceManager::m_instance = 0;

void ResourceManager::init() {
  fpFBP = fopen("data/FBP.MKF", "rb");
  fpMGO = fopen("data/MGO.MKF", "rb");
  fpBALL = fopen("data/BALL.MKF", "rb");
  fpDATA = fopen("data/DATA.MKF", "rb");
  fpF = fopen("data/F.MKF", "rb");
  fpFIRE = fopen("data/FIRE.MKF", "rb");
  fpRGM = fopen("data/RGM.MKF", "rb");
  fpSSS = fopen("data/SSS.MKF", "rb");
}
