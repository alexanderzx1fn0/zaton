#ifndef LOAD_LEVEL_H_
#define LOAD_LEVEL_H_

#include "level2.h"
#include "simpGun.h"
bool LoadLevel(const char* filename);
bool LoadModel(const char* filename);
void ClearLevel();

bool LoadCollidableGeometry(const char* filenam);
void ClearCollidableGeometry();

#endif
