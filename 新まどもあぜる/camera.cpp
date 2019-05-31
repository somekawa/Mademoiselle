#include "Dxlib.h"				// DxLibﾗｲﾌﾞﾗﾘを使用する　独自で準備したﾍｯﾀﾞｰﾌｧｲﾙは""で指定する
#include <math.h>				// 標準ﾍｯﾀﾞｰﾌｧｲﾙは<>で指定する
#include "main.h"
#include"player.h"
#include"camera.h"

MOVE_DIR runDir;

void CameraGameInit(void)
{
	runDir = DIR_DOWN;
}

void CameraControl(int padNo,Position pos,Position moved)
{
	if (PlayerTop(padNo)) {
		if (pos.y > moved.y) {
			ScrollMap(pos, pos.y - moved.y, DIR_DOWN);
		}
		 if (moved.y > pos.y) {
			ScrollMap(pos, moved.y - pos.y, DIR_UP);
		}
		if (pos.x > moved.x) {
			ScrollMap(pos, pos.x - moved.x, DIR_RIGHT);
		}
		 if (moved.x > pos.x) {
			ScrollMap(pos, moved.x - pos.x, DIR_LEFT);
		}
		switch (runDir)
		{
		case DIR_DOWN:
			if (mapPos.y >= PLAY_SIZE_Y - (SCREEN_SIZE_Y - (CHIP_SIZE_Y * 2))) runDir = DIR_RIGHT;
			break;
		case DIR_RIGHT:
			if (pos.x >= moved.x) {
			}
			if (mapPos.x >= PLAY_SIZE_X - SCREEN_SIZE_X) runDir = DIR_UP;
			break;
		case DIR_UP:
			if (mapPos.y <= 0) runDir = DIR_LEFT;
			break;
		case DIR_LEFT:
			if (mapPos.x <= 0) runDir = DIR_DOWN;
			break;
		default:
			break;
		}
	}
}

void ScrollMap(Position pos, int speed, MOVE_DIR dir)
{
	switch (dir)
	{
	case DIR_UP:
		if ((mapPos.x <= 0) || (mapPos.x >= PLAY_SIZE_X - SCREEN_SIZE_X)) {
			if (pos.y <= mapPos.y + (SCREEN_SIZE_Y - SCREEN_SIZE_Y / 4)) {
				mapPos.y -= speed;
			}
		}
		break;
	case DIR_RIGHT:
		if ((mapPos.y <= 0) || (mapPos.y >= PLAY_SIZE_Y - SCREEN_SIZE_Y)) {
			if (pos.x > SCREEN_SIZE_X / 2 + mapPos.x) {
				mapPos.x += speed;
			}
		}
		break;
	case DIR_DOWN:
		if ((mapPos.x <= 0) || (mapPos.x >= PLAY_SIZE_X - SCREEN_SIZE_X)) {
			if (pos.y >= SCREEN_SIZE_Y - (SCREEN_SIZE_Y / 4) + mapPos.y) {
				mapPos.y += speed;	// 下がる
			}
		}
		break;
	case DIR_LEFT:
		if ((mapPos.y <= 0) || (mapPos.y >= PLAY_SIZE_Y - SCREEN_SIZE_Y)) {
			if (pos.x < mapPos.x + SCREEN_SIZE_X / 2) {
				mapPos.x -= speed;
			}
		}
		break;
	default:
		break;
	}
}

bool PlayerTop(int padNo)
{
	bool top = false;
	for (int j = 0; j < PLAYER_MAX; j++) {
		if ((padNo != j) && (GetPassingCnt(padNo) > GetPassingCnt(j))) {
			top=true;
		}
		else if (GetPassingCnt(padNo) == GetPassingCnt(j)) {
			switch (runDir)
			{
			case DIR_UP:
				if (GetPlayerPos(padNo).y <= GetPlayerPos(j).y) {
					top = true;
				}
				else {
					top = false;
				}
				break;
			case DIR_RIGHT:
				if (GetPlayerPos(padNo).x >= GetPlayerPos(j).x) {
					top = true;
				}
				else {
					top = false;
				}
				break;
			case DIR_DOWN:
				if (GetPlayerPos(padNo).y >= GetPlayerPos(j).y) {
					top = true;
				}
				else {
					top = false;
				}
				break;
			case DIR_LEFT:
				if (GetPlayerPos(padNo).x <= GetPlayerPos(j).x) {
					top = true;
				}
				else {
					top = false;
				}
				break;
			default:
				break;
			}
		}
	}
	return top;
}

void CameraDraw(void)
{
	switch (runDir)
	{
	case DIR_DOWN:
		DrawString(600, 40, "DOWN", 0xff0000, true);
		break;
	case DIR_LEFT:
		DrawString(600, 40, "LEFT", 0xff0000, true);
		break;
	case DIR_RIGHT:
		DrawString(600, 40, "RIGHT", 0xff0000, true);
		break;
	case DIR_UP:
		DrawString(600, 40, "UP", 0xff0000, true);
		break;
	default:
		break;
	}
}
