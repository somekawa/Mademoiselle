#include "Dxlib.h"	//DxLibﾗｲﾌﾞﾗﾘを使用する　独自で準備したﾍｯﾀﾞｰﾌｧｲﾙは""で指定する
#include <math.h>	// 標準ﾍｯﾀﾞｰﾌｧｲﾙは<>で指定する
#include"main.h"
#include"stage.h"
#include"shot.h"

#define SHOT_LIFE_MAX 20

CHARACTER shot[SHOT_MAX];
int shotTimIng;
int shotCnt;
int shotImage;

void ShotSystmInit(void)
{
	shotImage = LoadGraph("image/shot.png");
}
void ShotGameInit(void)
{
	for (int i = 0; i < SHOT_MAX; i++) {
		shot[i].size = { 8,8 };
		shot[i].offsetSize = { shot[i].size.x / 2,shot[i].size.y / 2 };
		shot[i].moveSpeed = 10;
		shot[i].lifeMax = SHOT_LIFE_MAX;
		shot[i].life = shot[i].lifeMax;
		shot[i].moveDir = DIR_RIGHT;
		shot[i].visible = false;
		shot[i].animCnt = 0;
	}
	shotCnt = 5;
}
void ShotControl(void)
{
	for (int i = 0; i < SHOT_MAX; i++) {
		XY movedPos = shot[i].pos;
		XY movedHitCheck = movedPos;
		shot[i].animCnt++;
		if (shot[i].visible) {

			shot[i].life--;
			/*
			if (shot[i].downFlag) {
				movedPos.y += shot[i].moveSpeed;
				movedHitCheck.y = movedPos.y + shot[i].offsetSize.y;
			}
			*/
			switch (shot[i].moveDir)
			{
			case DIR_RIGHT:
				movedPos.x += shot[i].moveSpeed;
				movedHitCheck.x = movedPos.x + shot[i].offsetSize.x;
				break;
			case DIR_LEFT:
				movedPos.x -= shot[i].moveSpeed;
				movedHitCheck.x = movedPos.x - shot[i].offsetSize.x;
				break;
			}
			
			
			if (IsPass(movedHitCheck)) {
				shot[i].pos = movedPos;
			}
			else {
				shot[i].life = 0;
			}
			if (shot[i].life == 0) {
				DeleteShot(i);
			}
		}
	}
}

void Shoot(XY pPos, MOVE_DIR pDir, bool flag)
{
	for (int i = 0; i < SHOT_MAX; i++) {
		if (!shot[i].visible) {
			shot[i].life = shot[i].lifeMax;
			shot[i].pos = pPos;
			shot[i].moveDir = pDir;
			shot[i].downFlag = flag;
			shot[i].animCnt = 0;
			shot[i].visible = true;
			shotCnt = 0;
			break;
		}

	}

	
}

CHARACTER GetShot(int index)
{
	return shot[index];
}

void DeleteShot(int index)
{
	shot[index].visible = false;
	shot[index].downFlag = false;
}
void ShotDraw(void)
{
	for (int i = 0; i < SHOT_MAX; i++) {
		if (shot[i].visible) {
			DrawGraph(shot[i].pos.x - shot[i].offsetSize.x - mapPos.x, shot[i].pos.y - shot[i].offsetSize.y - mapPos.y, shotImage, true);
			if (shot[i].downFlag) {
				DrawString(780, 20, "SHOTDOWN OK", 0xffffff);
			}
		}
		DrawFormatString(200, i * 20, 0xffffff, "shotPos %d,%d", shot[i].pos.x, shot[i].pos.y);
	}
}