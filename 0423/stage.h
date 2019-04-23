#pragma once

#define CHIP_SIZE_X 32
#define CHIP_SIZE_Y 32

#define MAP_X 50
//#define MAP_Y 14
#define MAP_Y 30


void StageSystmInit(void);
void StageGameInit(void);
void StageControl(void);
void StageDraw(void);

XY MapPosToIndex(XY pos);
bool IsPass(XY pos);

XY MapIndexToPos(XY index);
XY MapPos(XY pos, MOVE_DIR der);