#include "Dxlib.h"	//DxLibﾗｲﾌﾞﾗﾘを使用する　独自で準備したﾍｯﾀﾞｰﾌｧｲﾙは""で指定する
#include <math.h>	// 標準ﾍｯﾀﾞｰﾌｧｲﾙは<>で指定する
#include"keycheck.h"
#include"main.h"
#include"stage.h"
#include"player.h"
#include"shot.h"

#define INIT_VELOCITY 50	// 初期速度
#define SECOND_PER_FRAME 0.2	// 1ﾌﾚｰﾑの秒数
#define SPEED 10


int playerImage;
int runImage[4];
int downImage;
int shotImage[2];
CHARACTER player;
int downPos;

void PlayerSystmInit(void)
{
	playerImage = LoadGraph("image/red_stop.png");
	downImage = LoadGraph("image/red_down.png");
	shotImage[0] = LoadGraph("image/red_stop_shot.png");
	shotImage[1] = LoadGraph("image/red_down_shot.png");
	LoadDivGraph("image/red_run.png", 4, 4, 1, 50, 50, runImage, true);
}

void PlayerGameInit(void)
{
	player.size = { 50,50 };
	player.offsetSize = { player.size.x / 2,player.size.y / 2 };
	player.hitPos = 16;
	player.pos = { CHIP_SIZE_X * 4,CHIP_SIZE_Y * 13 - 25};
	player.moveSpeed = 4;
	player.animCnt = 0;
	player.moveDir = DIR_RIGHT;
	player.runFlag = false;
	player.jumpFlag = false;
	player.shotFlag = false;
	player.downFlag = false;
	player.visible = true;
	player.imgLocCnt = 0;
	downPos = 0;
}

void PlayerControl(void)
{
	player.runFlag = false;
	player.jumpFlag = true;
	player.downFlag = false;
	player.imgLocCnt++;
	downPos = 0;

	XY movedPos = player.pos;
	XY movedHitCheck = movedPos;
	XY movedHitCheck2;
	XY movedHitCheck3;

	// 移動
	if (player.visible) {
		if (oldKey[P1_RIGHT]) {
			player.runFlag = true;
			player.moveSpeed = SPEED;
			player.moveDir = DIR_RIGHT;
		}
		else if (oldKey[P1_LEFT]) {
			player.runFlag = true;
			player.moveSpeed = -SPEED;
			player.moveDir = DIR_LEFT;
		}

		// ｼﾞｬﾝﾌﾟ
		if (player.jumpFlag) {
			movedPos.y -= player.velocity.y * SECOND_PER_FRAME;	// 距離の更新
			player.velocity.y -= ACC_G * SECOND_PER_FRAME;			// 速度の更新	定数＊定数なら最初から一緒にする

																	// 頭の上
			movedHitCheck.y = movedPos.y - player.hitPos;
			movedHitCheck2 = movedHitCheck;
			movedHitCheck2.x = movedPos.x - player.hitPos;
			movedHitCheck3 = movedHitCheck;
			movedHitCheck3.x = movedPos.x + player.hitPos - 1;

			if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3))) {
				player.pos.y = movedPos.y;
			}
			else {
				player.pos.y = MapPos(movedHitCheck, DIR_DOWN).y + player.hitPos;
				player.velocity.y *= -1;
			}


			// 足元にﾌﾞﾛｯｸがあるか
			movedPos = player.pos;
			movedHitCheck.y = movedPos.y + player.offsetSize.y;
			movedHitCheck2 = movedHitCheck;
			movedHitCheck2.x = movedPos.x - player.hitPos;
			movedHitCheck3 = movedHitCheck;
			movedHitCheck3.x = movedPos.x + player.hitPos - 1;
			if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3))) {
				player.pos.y = movedPos.y;
			}
			else {
				player.pos.y = MapPos(movedHitCheck, DIR_UP).y - player.offsetSize.y;		// 足元から中心
				player.velocity.y = 0;
				player.jumpFlag = false;
			}


		}
		if ((!player.jumpFlag) && (trgKey[P1_UP])) {
			player.jumpFlag = true;
			player.velocity.y = INIT_VELOCITY;
		}

		if (player.runFlag) {
			movedPos = player.pos;
			movedHitCheck = movedPos;
			player.animCnt++;
			movedPos.x += player.moveSpeed;

			switch (player.moveDir)
			{
			case DIR_RIGHT:
				movedHitCheck.x = movedPos.x + player.hitPos;
				break;
			case DIR_LEFT:
				movedHitCheck.x = movedPos.x - player.hitPos - 1;
				break;
			}

			movedHitCheck2 = movedHitCheck;
			movedHitCheck2.y = movedPos.y - player.hitPos;
			movedHitCheck3 = movedHitCheck;
			movedHitCheck3.y = movedPos.y + player.offsetSize.y - 1;
			if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3))) {
				player.pos = movedPos;
			}
			else {
				// 壁にぴったりくっつける
				if (player.moveDir == DIR_RIGHT) {
					player.pos.x = MapPos(movedHitCheck, DIR_LEFT).x - player.hitPos;
				}
				else if (player.moveDir == DIR_LEFT) {
					player.pos.x = MapPos(movedHitCheck, DIR_RIGHT).x + player.hitPos;
				}
			}
		}

		if ((oldKey[P1_RIGHT]) && (player.pos.x > SCREEN_SIZE_X / 2)) {
			mapPos.x += player.moveSpeed;
		}
		if ((oldKey[P1_LEFT]) && (player.pos.x < MAP_X * CHIP_SIZE_X - SCREEN_SIZE_X / 2)) {
			mapPos.x += player.moveSpeed;
		}
	}

	if ((!player.jumpFlag)&&(!player.runFlag)&&(oldKey[P1_DOWN])) {
		player.downFlag = true;
		downPos = 8;
	}

	// ｼｮｯﾄ
	if ((!player.shotFlag) && (oldKey[P1_A])) {
		player.imgLocCnt = 0;
		player.shotFlag = true;
		if (player.moveDir == DIR_RIGHT) {
			Shoot({ player.pos.x, player.pos.y - 12  + downPos}, player.moveDir,player.downFlag);
		}
		else if (player.moveDir == DIR_LEFT) {
			Shoot({ player.pos.x,player.pos.y - 12 + downPos}, player.moveDir,player.downFlag);
		}
	}
	else {

		if (player.imgLocCnt > 6) {
			player.shotFlag = false;
		}
	}
}

void PlayerDraw(void)
{
	int img = playerImage;
	if ((player.runFlag)&&(!player.jumpFlag)) img = runImage[(player.animCnt / 6) % 4];
	if (player.downFlag) img = downImage;
	if ((!player.runFlag) && (oldKey[P1_A])) img = shotImage[0];
	if ((!player.runFlag) && (oldKey[P1_A])&&(player.downFlag)) img = shotImage[1];
	if(player.moveDir==DIR_RIGHT){
		DrawGraph(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y, img, true);
	}
	else if(player.moveDir==DIR_LEFT){
		DrawTurnGraph(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y, img, true);
	}
	if (player.downFlag) {
		DrawString(780, 0, "PLAYERDOWN OK", 0xffffff);
	}
	DrawBox(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y,
		player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y, 0xff0000, false);
	DrawBox(player.pos.x - player.hitPos - mapPos.x, player.pos.y - player.hitPos - mapPos.y + downPos,
		player.pos.x + player.hitPos  - mapPos.x, player.pos.y + player.hitPos - mapPos.y + downPos, 0x00ffff, false);
	DrawLine(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - mapPos.y,
		player.pos.x + player.offsetSize.x - mapPos.x,player.pos.y - mapPos.y, 0x00ffff, true);
	DrawLine(player.pos.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y,
		player.pos.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y, 0x00ffff, true);
}