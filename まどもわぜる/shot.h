#pragma once

#define SHOT_MAX 10

void ShotSystmInit(void);
void ShotGameInit(void);
void ShotControl(void);
void ShotDraw(void);

void Shoot(XY pPos, MOVE_DIR pDir, bool flag);
void DeleteShot(int index);
CHARACTER GetShot(int index);