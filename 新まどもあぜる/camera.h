#pragma once

void CameraGameInit(void);
void CameraControl(int padNo, Position pos, Position moved);
void ScrollMap(Position pos, int speed, MOVE_DIR dir);
bool PlayerTop(int padNo);
void CameraDraw(void);