#ifndef LOAD_LEVEL_H_
#define LOAD_LEVEL_H_

bool LoadLevel(const char* filename);
bool LoadModel(const char* filename);
void ClearLevel();

bool LoadCollidableGeometry(const char* filenam);
void ClearCollidableGeometry();

#endif
