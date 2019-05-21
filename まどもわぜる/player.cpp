

#include "Dxlib.h"				// DxLibײ���؂��g�p����@�Ǝ��ŏ�������ͯ�ް̧�ق�""�Ŏw�肷��
#include <math.h>				// �W��ͯ�ް̧�ق�<>�Ŏw�肷��
#include"keycheck.h"
#include"main.h"
#include"stage.h"
#include"player.h"
#include"effect.h"

#define INIT_VELOCITY 50		// �������x
#define SECOND_PER_FRAME 0.3	// 1�ڰт̕b��

#define DOWN_SPEED_DEF	5.0f	// �f�t�H���g�̗������x
#define UP_SPEED_DEF   -15.0f	// �f�t�H���g�̏㏸���x

#define CORRECTION	  100		// ���C���[�̒����␳�p���l(����y���W)

#define KEEPPOSX_CORRECTION  120	// pos�̒l�̕␳�p���l

#define ONEFRAME_WIRE_UP	  15    // 1�t���[���ŏオ�郏�C���[�̑��x

#define FURIKO_SPEED_DEF      1.6f
#define FURIKO_SPEED_MAX      3.2f
#define FURIKO_SPEED_MIN	  0.2f

//#define PAI 3.141592
//#define JUMPSPEED 2

PLAYER_STATE player_state;

// ��ڲ԰��߰��
enum PLAYER_SPEED {
	PLAYER_SPEED_NORMAL = 8,
	PLAYER_SPEED_SEGWEY = 12
};
int p1Wak[2];
int yazirusiImage[2];

int playerImage[PLAYER_MAX];
int playerIcon[PLAYER_MAX];
int runImage[PLAYER_MAX][10];
int jumpImage[PLAYER_MAX];
int stopJumpImage[PLAYER_MAX];
int downImage;
int shotImage[PLAYER_MAX];
int segweyImage[PLAYER_MAX][2];
CHARACTER player;

int WirePreTimeCnt;					// ���C���[���������L�΂��ēr���œr�؂��܂ł̎���
int WireTimeCnt;					// ���C���[�̕\������

bool _isPushJump;
bool _isJumped;


void OnMove(float& x, float& y, float vx, float vy);
void OnAdjust();



Position furiko_pos;
float _g;
float _v;
Position _length;


float KeepPosX;		// ���W�̕ۑ��p
float KeepPosY;		// ���W�̕ۑ��p

float _vx;
float _vy;

float playerX_old;
float playerY_old;
float playerX_now;
float playerY_now;

float radian;

bool skyFlag;

//float rot;

//Position Maxrad;		// �ő�̊p�x(-20)
//Position minrad;		// �ŏ��̊p�x(-160)

float MaxDeg;
float minDeg;
float nowDeg;
float furikoSpeed;

float defDeg;




typedef Position Vec2;

inline float Dot(const Vec2 & a, const Vec2 & b) {
	return a.x* b.x + a.y * b.y;
}

inline float Cross(const Vec2 & a, const Vec2 & b) {
	return a.x* b.y - b.x * a.y;
}




void PlayerSystmInit(void)
{
	p1Wak[0] = LoadGraph("image/p1CSWak.png");
	p1Wak[1] = LoadGraph("image/p1GWak.png");
	for (int i = 0; i < 2; i++) {
		yazirusiImage[i] = LoadGraph("image/yazirusi.png");
	}

	// ��
	playerImage[PLAYER_RED] = LoadGraph("image/playerR_stop.png");
	playerIcon[PLAYER_RED] = LoadGraph("image/R_icon.png");
	jumpImage[PLAYER_RED] = LoadGraph("image/playerR_jump.png");
	stopJumpImage[PLAYER_RED] = LoadGraph("image/playerR_stop_jump.png");
	LoadDivGraph("image/playerR_run.png", 10, 5, 2, 72, 72, runImage[PLAYER_RED], true);
	LoadDivGraph("image/playerR_segway.png", 2, 2, 1, 72, 72, segweyImage[PLAYER_RED], true);

	// ��
	playerImage[PLAYER_BLUE] = LoadGraph("image/playerB_stop.png");
	playerIcon[PLAYER_BLUE] = LoadGraph("image/B_icon.png");
	jumpImage[PLAYER_BLUE] = LoadGraph("image/playerB_jump.png");
	stopJumpImage[PLAYER_BLUE] = LoadGraph("image/playerB_stop_jump.png");
	LoadDivGraph("image/playerB_run.png", 10, 5, 2, 72, 72, runImage[PLAYER_BLUE], true);
	LoadDivGraph("image/playerB_segway.png", 2, 2, 1, 72, 72, segweyImage[PLAYER_BLUE], true);

	downImage = LoadGraph("image/red_down.png");
	shotImage[0] = LoadGraph("image/red_stop_shot.png");
	shotImage[1] = LoadGraph("image/red_down_shot.png");



}

void PlayerGameInit(void)
{
	player.type = PLAYER_RED;
	player.size = { 72,72 };
	player.offsetSize = { -player.size.x / 2,-player.size.y };
	player.hitPosE = { 20,0 };
	player.hitPosS = { 20,62 };
	player.pos = { CHIP_SIZE_X * 15, CHIP_SIZE_Y * 14 };
	player.moveSpeed = PLAYER_SPEED_NORMAL;
	player.animCnt = 0;
	player.moveDir = DIR_RIGHT;
	player.runFlag = false;
	player.jumpFlag = false;
	player.jumpCnt = 0;	// �ެ��߂ł����
	player.shotFlag = false;
	player.downFlag = false;
	player.segweyFlag = false;	// ��޳��

	player.wireOkFlag = false;
	player.wireFlag = false;

	player.visible = true;
	player.visible2 = false;
	player.imgLocCnt = 0;

	player.right = false;
	player.left = false;

	player.wallRunSpeed = player.moveSpeed;	// �ǂł̑��x

	//�Ђ��̎x�_�̏�����
	_v = 0;			// �U��q�̂ӂ蕝
	WireTimeCnt = 0;

	_g = 2.0f;		//�d�͂̒�`
	_length = { 0 , 0 };	//�R�̒����̌v�Z


	KeepPosX = 0;	// ���W�̕ۑ��p
	KeepPosY = 0;	// ���W�̕ۑ��p

	_vx = 0;
	_vy = 0;

	_isPushJump = false;
	_isJumped = false;

	playerX_old = 0;
	playerY_old = 0;

	radian = 0;

	skyFlag = false;

	player_state = PLAYER_NORMAL;

	WirePreTimeCnt = 0;

	//Maxrad = { 0,0 };
	//minrad = { 0,0 };

	MaxDeg = 0.0f;
	minDeg = 0.0f;
	nowDeg = 0.0f;
	furikoSpeed = 0.0f;
	defDeg = 0.0f;


	//rot = -rand() % 90;
}

void PlayerControl(void)
{
	switch (GetGameMode())
	{
	case GMODE_CHARASERE:	// ��׾ڸ�
		if (!player.visible) {
			if (trgKey[P1_RIGHT]) player.type++;
			if (trgKey[P1_LEFT]) player.type--;

			if (player.type >= PLAYER_MAX) player.type = 0;
			if (player.type <= -1)player.type = PLAYER_MAX - 1;
		}
		else {
			player.animCnt++;
		}
		//if (trgKey[P1_A]) player.visible = !player.visible;

		break;
	case GMODE_GAME:
		PlayerState();
		break;
	}

	//
		//case GMODE_GAME:	// �ްђ�
	//		
	//
	//
	//
	//		player.runFlag = false;
	//		player.jumpFlag = true;
	//		player.downFlag = false;
	//		player.moveSpeed = PLAYER_SPEED_NORMAL;
	//		player.imgLocCnt++;
	//		downPos = 0;
	//
	//		
			//XY movedHitCheck = player.pos;
			//XY movedHitCheck2;
			//XY movedHitCheck3;

			//// ���C���[����
			//if (newKey[P2_UP])
			//{

			//	

			//	// 5~10�}�X�ȓ��̓���Ƀu���b�N����������A���C���[���o����悤�ɂ�����
			//	for (int i = 5; i <= 10; )
			//	{
			//		movedHitCheck.y = player.pos.y  - (CHIP_SIZE_Y * i);		// ����
			//		if ((WireBlockPass(movedHitCheck)) && (WireBlockPass({ movedHitCheck.x - CHIP_SIZE_X / 2 ,movedHitCheck.y })) &&
			//											(WireBlockPass({ movedHitCheck.x + CHIP_SIZE_X / 2 ,movedHitCheck.y })))		// ���͈͓��Ƀu���b�N�����݂��Ȃ����ƂɂȂ�
			//		{
			//			player.wireFlag = false;			// �͈͓��ɑ��݂��Ȃ��̂�false��������
			//			i++;
			//		}
			//		else
			//		{
			//			// �u���b�N�����݂���Ƃ��̏���

			//			//�R�̒����̌v�Z
			//			_length = player.pos.y + player.offsetSize.y;

			//			// �R�̒����̕␳
			//			if (_length >= 250)
			//			{
			//				_length = 250;
			//			}

			//			// KeepPosX�̕␳
			//			if (_length < KEEPPOSX_CORRECTION)
			//			{
			//				KeepPosX = KEEPPOSX_CORRECTION;
			//			}
			//			else
			//			{
			//				KeepPosX = _length;
			//			}

			//			// KeepPosY���w��u���b�N�̍����ɍ��킹��
			//			KeepPosY = movedHitCheck.y - CHIP_SIZE_Y / 4 - mapPos.y;
			//			player.wireFlag = true;
			//			player.visible = false;
			//			player.visible2 = true;

			//			TimeCnt = 0;
			//			return;
			//		}
			//		
			//	}
			//	

			//}
	//
	//		// ��޳��
	//		if (trgKey[P2_A]) player.segweyFlag = !player.segweyFlag;
	//		if (player.segweyFlag) player.moveSpeed = PLAYER_SPEED_SEGWEY;
	//
	//		// �ړ�
	//		if (player.visible && !player.visible2) {
	//			if (oldKey[P1_RIGHT]) {
	//				player.runFlag = true;
	//				player.moveSpeed = player.moveSpeed;
	//				player.moveDir = DIR_RIGHT;
	//			}
	//			else if (oldKey[P1_LEFT]) {
	//				player.runFlag = true;
	//				player.moveSpeed = -player.moveSpeed;
	//				player.moveDir = DIR_LEFT;
	//			}
	//
	//			// �ެ���
	//			if (player.jumpFlag) {
	//
	//
	//				movedPos.y -= player.velocity.y * SECOND_PER_FRAME;		// �����̍X�V
	//				player.velocity.y -= ACC_G * SECOND_PER_FRAME;			// ���x�̍X�V	�萔���萔�Ȃ�ŏ�����ꏏ�ɂ���
	//
	//																		// ���̏�
	//				movedHitCheck.y = movedPos.y - player.hitPosS.y;
	//				movedHitCheck2 = movedHitCheck;
	//				movedHitCheck2.x = movedPos.x - player.hitPosS.x;
	//				movedHitCheck3 = movedHitCheck;
	//				movedHitCheck3.x = movedPos.x + player.hitPosE.x - 1;
	//
	//				if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3))) {
	//					player.pos.y = movedPos.y;
	//				}
	//				else {
	//					player.pos.y = MapPos(movedHitCheck, DIR_DOWN).y + player.hitPosE.y;
	//					player.velocity.y *= -1;
	//				}
	//
	//
	//				// ��������ۯ������邩
	//				//movedPos = player.pos;
	//				//movedHitCheck.y = movedPos.y + player.offsetSize.y;
	//				//movedHitCheck2 = movedHitCheck;													// ��
	//				//movedHitCheck2.x = movedPos.x - player.hitPosS.x;
	//				//movedHitCheck3 = movedHitCheck;													// �E
	//				//movedHitCheck3.x = movedPos.x + player.hitPosE.x - 1;
	//
	//				// �Ȃ��Ƃ�
	//				//if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3))) {
	//				//	player.pos.y = movedPos.y;
	//				//	if (player.jumpCnt < 2) {
	//
	//				//		player.jumpFlag = false;
	//				//	}
	//				//}
	//
	//				// ����Ƃ�
	//				else {
	//					player.pos.y = MapPos(movedHitCheck, DIR_UP).y - player.offsetSize.y;		// �������璆�S
	//					player.velocity.y = 0;
	//					player.jumpCnt = 0;
	//					player.jumpFlag = false;
	//				}
	//
	//			}
	//			if ((!player.jumpFlag) && (trgKey[P1_UP])) {
	//				player.jumpFlag = true;
	//				player.jumpCnt++;
	//				player.velocity.y = INIT_VELOCITY;
	//
	//			}
	//
		//if (player.runFlag) {
		//	movedPos = player.pos;
		//	movedHitCheck = movedPos;
		//	player.animCnt++;
		//	movedPos.x += player.moveSpeed;

		//	switch (player.moveDir)
		//	{
		//	case DIR_RIGHT:
		//		movedHitCheck.x = movedPos.x + player.hitPosE.x;
		//		break;
		//	case DIR_LEFT:
		//		movedHitCheck.x = movedPos.x - player.hitPosS.x - 1;
		//		break;
		//	}

		//	movedHitCheck2 = movedHitCheck;
		//	movedHitCheck2.y = movedPos.y - player.hitPosS.y;
		//	movedHitCheck3 = movedHitCheck;
		//	movedHitCheck3.y = movedPos.y + player.offsetSize.y - 1;
		//	if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3))) {
		//		player.pos = movedPos;
		//	}
		//	else {
		//		// �ǂɂ҂����肭������
		//		if (player.moveDir == DIR_RIGHT) {
		//			player.pos.x = MapPos(movedHitCheck, DIR_LEFT).x - player.hitPosS.x;
		//		}
		//		else if (player.moveDir == DIR_LEFT) {
		//			player.pos.x = MapPos(movedHitCheck, DIR_RIGHT).x + player.hitPosE.x;
		//		}
		//	}
		//}
	//
	//			if ((oldKey[P1_RIGHT]) && (player.pos.x > SCREEN_SIZE_X / 2)) {
	//				mapPos.x += player.moveSpeed;
	//			}
	//			if ((oldKey[P1_LEFT]) && (player.pos.x < MAP_X * CHIP_SIZE_X - SCREEN_SIZE_X / 2)) {
	//				mapPos.x += player.moveSpeed;
	//			}
	//		}
	//
	//
	//		if ((!player.jumpFlag) && (!player.runFlag) && (oldKey[P1_DOWN]) ) {
	//			player.downFlag = true;
	//			downPos = 8;
	//		}
	//
	//		// ����
	//		if ((!player.shotFlag) && (oldKey[P1_A])) {
	//			player.imgLocCnt = 0;
	//			player.shotFlag = true;
	//			if (player.moveDir == DIR_RIGHT) {
	//				Shoot({ player.pos.x, player.pos.y - 12 + downPos }, player.moveDir, player.downFlag);
	//			}
	//			else if (player.moveDir == DIR_LEFT) {
	//				Shoot({ player.pos.x,player.pos.y - 12 + downPos }, player.moveDir, player.downFlag);
	//			}
	//		}
	//		else {
	//
	//			if (player.imgLocCnt > 6) {
	//				player.shotFlag = false;
	//			}
	//		}
	//
			// player��ǂ��J����
		//if (player.pos.y > SCREEN_SIZE_Y - CHIP_SIZE_Y * 5) mapPos.y += ACC_G ;
	//if (player.pos.y < CHIP_SIZE_Y * MAP_Y - SCREEN_SIZE_Y + CHIP_SIZE_Y * 5) mapPos.y -= ACC_G * SECOND_PER_FRAME;
	//
	//
	//		//// ���E�ɂ��킶�퓮��
			/*if (player.pos.x > SCREEN_SIZE_X - CHIP_SIZE_X * 5) mapPos.x += 1;
			if (player.pos.x < CHIP_SIZE_X * MAP_X - SCREEN_SIZE_X + CHIP_SIZE_X * 5) mapPos.x -= SECOND_PER_FRAME;*/
			//	break;
			//}
		//	
		//

}

void PlayerDraw(void)
{
	switch (GetGameMode()) {
	case GMODE_CHARASERE:
		DrawGraph(0, 120, p1Wak[0], true);
		if (player.visible) {
			DrawRotaGraph(160, 300, 3, 0, runImage[player.type][(player.animCnt / 3) % 10], true);
			DrawString(120, 180, "�L��������I", 0x000000);
		}
		else {
			DrawRotaGraph(160, 300, 3, 0, playerImage[player.type], true);
			DrawTurnGraph(0, 240, yazirusiImage[0], true);
			DrawGraph(260, 240, yazirusiImage[1], true);

		}

		break;
	case GMODE_GAME:
		if (player.visible && !player.visible2)
		{
			int img = playerImage[player.type];
			if ((player.runFlag) && (!player.jumpFlag)) img = runImage[player.type][(player.animCnt / 3) % 10];
			if ((player.jumpFlag) && (player.runFlag)) img = jumpImage[player.type];
			if ((player.jumpFlag) && (!player.runFlag)) img = stopJumpImage[player.type];
			if (player.segweyFlag) img = segweyImage[player.type][(player.animCnt / 5) % 2];

			if (player_state == PLAYER_WALL_RIGHT) {
				DrawBox(player.pos.x - player.hitPosS.y - mapPos.x, player.pos.y - player.hitPosE.x - mapPos.y,
					player.pos.x - mapPos.x, player.pos.y + player.hitPosS.x - mapPos.y, 0x00ffff, false);
			}
			else if (player_state == PLAYER_WALL_LEFT) {
				DrawBox(player.pos.x - mapPos.x, player.pos.y - player.hitPosE.x - mapPos.y,
					player.pos.x + player.hitPosS.y - mapPos.x, player.pos.y + player.hitPosS.x - mapPos.y, 0x00ffff, false);
			}

			if (player.moveDir == DIR_RIGHT) {
				if (player_state == PLAYER_WALL_RIGHT) {
					DrawRotaGraph(player.pos.x - 36 - mapPos.x, player.pos.y - mapPos.y,
						1, -PI / 2, img, true, false);
				}
				else {
					DrawGraph(player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y, img, true);
				}
			}
			else if (player.moveDir == DIR_LEFT) {
				if (player_state == PLAYER_WALL_LEFT) {
					DrawRotaGraph(player.pos.x + 36 - mapPos.x, player.pos.y - mapPos.y,
						1, PI / 2, img, true, true);
				}
				else {
					DrawTurnGraph(player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y, img, true);
				}
			}

			if ((player_state != PLAYER_WALL_RIGHT) && (player_state != PLAYER_WALL_LEFT)) {
				DrawBox(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y,
					player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y - mapPos.y, 0xff0000, false);


				DrawBox(player.pos.x - player.hitPosS.x - mapPos.x, player.pos.y - player.hitPosS.y - mapPos.y,
					player.pos.x + player.hitPosE.x - mapPos.x, player.pos.y - mapPos.y, 0x00ffff, false);


				DrawLine(player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y - (player.size.y / 2) - mapPos.y,
					player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - (player.size.y / 2) - mapPos.y, 0x00ffff, true);
				DrawLine(player.pos.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y,
					player.pos.x - mapPos.x, player.pos.y - mapPos.y, 0x00ffff, true);
			}
		}

		if (!player.visible && player.visible2)
		{
			if (player.moveDir == DIR_RIGHT)
			{
				DrawGraph(player.pos.x  - mapPos.x - 56, player.pos.y - player.size.y - mapPos.y - 23, jumpImage[player.type], true);		   // �L�����N�^��������Ƃ��ĕ`��
			}
			else
			{
				DrawTurnGraph(player.pos.x  - mapPos.x - 15, player.pos.y - player.size.y  - mapPos.y - 23, jumpImage[player.type], true);	   // �L�����N�^��������Ƃ��ĕ`��
			}

		}

		if (player_state == PLAYER_W_PRE || player_state == PLAYER_W_ACTION)
		{
			if (player.wireOkFlag)
			{
				DrawLine(player.pos.x - mapPos.x, player.pos.y - player.size.y - mapPos.y, furiko_pos.x - mapPos.x, furiko_pos.y - mapPos.y, 0xffffffff, 2);		// ���C���[
			}
			else
			{
				if (WirePreTimeCnt <= furiko_pos.y)
				{
					DrawLine(player.pos.x - mapPos.x, player.pos.y - player.size.y - mapPos.y, furiko_pos.x - mapPos.x, furiko_pos.y - mapPos.y, 0xffffffff, 2);	// ���C���[
					WirePreTimeCnt++;
				}

			}
		}

		// ����
		DrawGraph(50, 40, p1Wak[1], true);
		DrawGraph(58, 74, playerIcon[player.type], true);
		DrawBox(20, 5, 86, 71, 0xffffff, true);
		DrawBox(20, 5, 86, 71, 0x000000, false);
		DrawBox(100, 80, 200, 95, 0x000000, true);
		DrawBox(101, 81, 199, 94, 0x00ff00, true);
		break;
	}

}



void WireDraw(void)
{
	// ���C���[�A�N�V�������͂��߂���A���S���W���Œ�

	// W��F�𓯎��ɉ������Ƃ��̃o�O��������



	//if (player.wireFlag)
	//{
	//	

	//	if (TimeCnt < 150)
	//	{
	//		skyFlag = true;

	//		//�Ђ��̎x�_���`����
	//		if (player.moveDir == DIR_RIGHT)
	//		{
	//			_endPoint.x = KeepPosX + CHIP_SIZE_X;
	//		}
	//		else
	//		{
	//			_endPoint.x = KeepPosX - CHIP_SIZE_X;
	//		}
	//		_endPoint.y = KeepPosY /*+ CHIP_SIZE_Y*/;

	//		Vec2 v = (_pos - _endPoint);//�U��q�̎x�_����U��q�̐��܂ł̃x�N�g��
	//		v = v.normalized();//���K������
	//		//�O�ςƓ��ς𗘗p���Ċp�x���v�Z
	//		float cost = Dot(v, Vec2(-1, 0));
	//		float sint = Cross(v, Vec2(-1, 0));
	//		float theta = atan2f(cost, sint);

	//		_v += _g * cost;

	//		

	//		if (CheckHitKey(KEY_INPUT_F))				 // F�L�[����������
	//		{
	//			XY movedPos = player.pos;
	//			XY movedHitCheck = movedPos;
	//			XY movedHitCheck2;
	//			XY movedHitCheck3;
	//			

	//			movedPos = player.pos;
	//			movedHitCheck.y = movedPos.y + player.offsetSize.y;
	//			movedHitCheck2 = movedHitCheck;													// ��
	//			movedHitCheck2.x = movedPos.x - player.hitPosS.x;
	//			movedHitCheck3 = movedHitCheck;													// �E
	//			movedHitCheck3.x = movedPos.x + player.hitPosE.x - 1;

	//			

	//			playerX_now = _pos.x + mapPos.x;		 // �؂藣���u��
	//			playerY_now = _pos.y + mapPos.y;		 // �؂藣���u��

	//			getRadian(playerX_old, playerY_old, playerX_now, playerY_now);					// �p�x(���W�A��)�����߂邽�߂̂��


	//			// �n�ʂɐڐG���Ă�
	//			//if (!(IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3)))
	//			//{		
	//			//	PlayerDraw();
	//			//	skyFlag = false;
	//			//}
	//			//else
	//			//{
	//			//	//playerX_now = _pos.x + mapPos.x;		 // �؂藣���u��
	//			//	//playerY_now = _pos.y + mapPos.y;		 // �؂藣���u��

	//			//	//getRadian(playerX_old, playerY_old, playerX_now, playerY_now);

	//			//	while (skyFlag == true)
	//			//	{
	//			//		Disassembly_C(radian);			// x��
	//			//		Disassembly_S(radian, _g);		// y��

	//			//		DrawGraph(_pos.x + mapPos.x + Disassembly_C(radian), _pos.y + mapPos.y + Disassembly_S(radian, _g), stopJumpImage[player.type], true);
	//			//	}
	//			//	
	//			//}
	//			

	//			
	//				

	//				//Disassembly_C(radian);			// x��
	//				//Disassembly_S(radian);			// y��

	//				player.visible = true;			// �A�j���[�V��������L�������\�������
	//				player.visible2 = false;		// ���C���[���̐Î~��L��������\���ɂȂ�
	//				player.wireFlag = false;		// ���C���[����\���ɂȂ�

	//				// ���n�n�_�̕`��ʒu
	//				player.pos.x = _pos.x + mapPos.x + player.offsetSize.x;
	//				//player.pos.y = _pos.y + mapPos.y;

	//				DrawGraph(Disassembly_C(radian), Disassembly_S(radian , _g), stopJumpImage[player.type], true);
	//			
	//				
	//				// �����ɓ�i�W�����v�p�����̒ǉ����K�v����

	//				

	//				PlayerDraw();
	//				
	//			
	//		}
	//		else
	//		{
	//			// �X�V�O��old�ɕۑ�
	//			playerX_old = _pos.x + mapPos.x;		// 1�t���[���O
	//			playerY_old = _pos.y + mapPos.y;		// 1�t���[���O

	//			playerX_now = 0;						// �؂藣���u�Ԃ�����܂�0
	//			playerY_now = 0;						// �؂藣���u�Ԃ�����܂�0

	//			player.visible = false;
	//			player.visible2 = true;
	//			player.wireFlag = true;
	//			_isPushJump = false;
	//		}





	//		//���Ƃ͐U��q�̊p�x�ɏ]���āA���̎��X�̉����x�����߁A
	//		//���x(_v)�ɉ��Z���悤
	//		//�����X�����AY�����ɕ�����
	//		//OnMove�̑�3��4�����ɑ��

	//		
	//		OnMove(_pos.x, _pos.y, _v * sint, _v * cost);
	//		OnAdjust();		// ���ꂪ�Ȃ��ƂЂ����L�тĂ���
	//	
	//	//DrawLine(KeepPos, player.pos.y, KeepPos+100, 0, 0xffffffff, 1);		// �����Ȃ����ǃL�����ɌŒ肳���Ђ�

	//	//DrawLine(_pos.x, _pos.y, KeepPosX + CHIP_SIZE_X, KeepPosY + CHIP_SIZE_Y, 0xffffffff, 2);			// �������ǃL�����ɌŒ肳��Ȃ��Ђ�

	//	//DrawGraph(_pos.x, _pos.y, playerImage, true);						// �L�����N�^��������Ƃ��ĕ`��

	//		TimeCnt++;
	//	}
	//	else
	//	{
	//		player.wireFlag = false;
	//		player.visible = true;
	//		player.visible2 = false;
	//		skyFlag = false;
	//	}

	//}
}



void OnMove(float& x, float& y, float vx, float vy) {
	// �ړ��̂��߂̏���
	x += vx;
	y += vy;

}

void OnAdjust() {
	//Vec2 v = (furiko_pos - _endPoint);
	//if (v.Length() > _length) {
	//	furiko_pos = _endPoint + v.normalized() * _length;
	//}
}

bool GetPlayerV(void)
{
	return player.visible;
}

float getRadian(float old_x, float old_y, float now_x, float now_y)
{
	radian = atan2f(now_y - old_y, now_x - old_x);
	return radian;
}


float Disassembly_C(float radian_cos)		// x��
{
	//radian_cos = radian;
	return cosf(radian_cos);
}

float Disassembly_S(float& radian_sin, float g)		// y��
{
	//radian_sin = radian;
	//g = _g;
	radian_sin = radian_sin - g;		// ���Z����?
	return sinf(radian_sin);
}


//float Disassembly(float radian_cos , float radian_sin)
//{
//	/*radian_cos = radian;
//	radian_sin = radian;*/
//	return (cosf(radian) , sinf(radian));
//}

void PlNormal(void)
{
	player.jumpFlag = false;
	player.runFlag = false;

	Position player_RD = { player.pos.x + player.moveSpeed + player.hitPosE.x , player.pos.y - 1 };
	Position player_LD = { player.pos.x - player.moveSpeed - player.hitPosS.x - 1 , player.pos.y - 1 };

	if (newKey[P1_RIGHT])
	{
		player.runFlag = true;
		player.moveDir = DIR_RIGHT;

		// �Ǌm�F
		if ((WallBlockPass(player_RD)) && (player.wallRunSpeed == player.moveSpeed) && (!newKey[P1_A])) {
			player_state = PLAYER_WALL_RIGHT;
		}
		else {
			if (IsPass(player_RD))// RD��chipID�擾���Ĉړ��ł���ID�����ׂ�
			{
				player.pos.x += player.moveSpeed;
				if (player.pos.x > SCREEN_SIZE_X / 2)//�J�������E�ɍs��
				{
					mapPos.x += player.moveSpeed;
				}

			}
			else
			{
				// �ׂ܂Ōv�Z����player.pos.x�ɑ��
				player.pos.x = GetWorldPos_Map(player_RD, DIR_LEFT).x - player.hitPosS.x;

			}
		}
	}
	if (newKey[P1_LEFT])
	{
		player.runFlag = true;
		player.moveDir = DIR_LEFT;

		// �Ǌm�F
		if ((WallBlockPass(player_LD)) && (player.wallRunSpeed == player.moveSpeed) && (!newKey[P1_A])) {
			if (player.wallRunSpeed == player.moveSpeed) {
				player_state = PLAYER_WALL_LEFT;
			}
		}
		else
		{
			if (IsPass(player_LD))	// LD��chipID�擾���Ĉړ��ł���ID�����ׂ�
			{
				player.pos.x -= player.moveSpeed;
				//if (player.pos.x < expData.mapWidth * CHIP_SIZE_X - SCREEN_SIZE_X / 2)//�J���������ɍs��
				//{
				//	mapPos.x -= player.moveSpeed;
				//}

			}
			else
			{
				// �ׂ܂Ōv�Z����player.pos.x�ɑ��
				player.pos.x = GetWorldPos_Map(player_LD, DIR_RIGHT).x + player.hitPosS.x;
			}
		}
	}

	if (trgKey[P2_UP])
	{
		if (player.moveDir == DIR_RIGHT)
		{
			player.right = true;
			player.left = false;
		}

		if (player.moveDir == DIR_LEFT)
		{
			player.right = false;
			player.left = true;
		}


		furiko_pos = { player.pos.x , player.pos.y - player.size.y };
		player.wireFlag = true;

		player_state = PLAYER_W_PRE;
	}



}

void PlJumpUp(void)
{
	Position player_RU = { player.pos.x + player.hitPosE.x - 1 , player.pos.y - player.moveSpeed - player.hitPosS.y };	// ����
	Position player_LU = { player.pos.x - player.hitPosS.x , player.pos.y - player.moveSpeed - player.hitPosS.y };	// �E��

	player.UpDownSpeed += player.AddUpDownSpeed;

	//�@RD �� LD �� y���W��-1���ď�̃u���b�N���𓾂�
	if (IsPass({ player_RU.x , player_RU.y - 1 }) && IsPass({ player_LU.x , player_LU.y - 1 }))
	{
		// ��ɉ����Ȃ�
		player_state = PLAYER_JUMP_UP;
		player.jumpFlag = true;
	}
	else
	{
		// ��Ƀu���b�N������
		player.UpDownSpeed = DOWN_SPEED_DEF;
		player.AddUpDownSpeed = 0.5f;
		player_state = PLAYER_DOWN;
	}

	// �㏸�̏���(������l�𒴂������ɂȂ�Ɨ����ɐ؂�ւ��悤�ɂ���)
	if (player.UpDownSpeed <= 0.0f)
	{
		if (IsPass({ player_RU.x , (player_RU.y + player.UpDownSpeed) }) && IsPass({ player_LU.x , (player_LU.y + player.UpDownSpeed) }))
		{
			player.pos.y += player.UpDownSpeed;
		}
		else
		{
			player.pos.y = GetWorldPos_Map({ player_RU.x , (player_RU.y + player.UpDownSpeed) }, DIR_DOWN).y + player.hitPosS.y;
		}

	}
	else
	{
		player_state = PLAYER_DOWN;
	}

}

void PlDown(void)
{
	Position player_RD = { player.pos.x + player.hitPosE.x - 1, player.pos.y - 1 };		// �E��
	Position player_LD = { player.pos.x - player.hitPosS.x , player.pos.y - 1 };		// ����

	//�@RD �� LD �� y���W��+1���ĉ��̃u���b�N���𓾂�
	if (IsPass({ player_RD.x , player_RD.y + 1 }) && IsPass({ player_LD.x , player_LD.y + 1 }))
	{
		if (player_state == PLAYER_NORMAL)
		{
			player.UpDownSpeed = DOWN_SPEED_DEF;
			player.AddUpDownSpeed = 0.8f;
			player_state = PLAYER_DOWN;
		}
	}
	else
	{
		if (trgKey[P1_A])
		{
			player.UpDownSpeed = UP_SPEED_DEF;
			player.AddUpDownSpeed = 0.8f;
			player_state = PLAYER_JUMP_UP;
		}
		else
		{
			if (player_state != PLAYER_W_ACTION)
			{
				player_state = PLAYER_NORMAL;
			}

		}
		return;
	}

	player.UpDownSpeed += player.AddUpDownSpeed;

	// ��������
	if (IsPass({ player_RD.x , (player_RD.y + player.UpDownSpeed) }) && IsPass({ player_LD.x , (player_LD.y + player.UpDownSpeed) }))
	{
		player.pos.y += player.UpDownSpeed;
		player.jumpFlag = true;
	}
	else
	{
		player.pos.y = GetWorldPos_Map({ player_RD.x , (player_RD.y + player.UpDownSpeed) }, DIR_UP).y;
		player.wallRunSpeed = player.moveSpeed;
		player.jumpFlag = false;
		SetEffct(player.pos);
	}


}

void PlWirePrepare(void)
{
	furiko_pos.y = furiko_pos.y - ONEFRAME_WIRE_UP;

	Position furiko_RU = { furiko_pos.x + player.size.x / 2, furiko_pos.y };
	Position furiko_LU = { furiko_pos.x - player.size.x / 2, furiko_pos.y };

	if (player.pos.y - furiko_pos.y <= 300)	// �w��͈�
	{
		if (!(WireBlockPass({ furiko_RU.x , furiko_RU.y })) || !(WireBlockPass({ furiko_LU.x , furiko_LU.y })))
		{
			// �������o��
			_length = furiko_pos - player.pos;


			// �v���C���[�̍��W��-90���Őݒ�?
			player.pos.x = cos((-90.0f * PI) / 180.0f) * _length.y + furiko_pos.x;
			player.pos.y = sin((-90.0f * PI) / 180.0f) * _length.y + furiko_pos.y;

			MaxDeg = -20.0f;
			minDeg = -160.0f;

			nowDeg = -90.0f;

			defDeg = -90.0f;

			furikoSpeed = FURIKO_SPEED_DEF;

			// �ő�
			//Maxrad.x = (-20.0f * PI / 180.0f) + furiko_pos.x;
			//Maxrad.y = (-20.0f * PI / 180.0f) + furiko_pos.y;

			//Maxrad.x = (-20 * PI / 180) + _length.x + furiko_pos.x;
			//Maxrad.y = (-20 * PI / 180) + _length.y + furiko_pos.y;

			// �ŏ�
			//minrad.x = (-160.0f * PI / 180.0f) + furiko_pos.x;
			//minrad.y = (-160.0f * PI / 180.0f) + furiko_pos.y;

			//minrad.x = (-160 * PI / 180) + _length.x + furiko_pos.x;
			//minrad.y = (-160 * PI / 180) + _length.y + furiko_pos.y;


			// �w��͈͈ȓ� & �w��u���b�N������
			// �w��u���b�N�Ƀ��C���[���q����
			//player.pos.x = furiko_RU.x - mapPos.x;
			//player.pos.y = furiko_RU.y + player.size.y - mapPos.y;
			player.visible = false;
			player.visible2 = true;
			player.wireFlag = false;
			player.wireOkFlag = true;
			player_state = PLAYER_W_ACTION;
			return;		// 1�ڂɌ������u���b�N�ł����̂�return�Ŕ�����
		}
		//else
		//{
		//	// �w��͈͊O & �w��u���b�N������
		//	//player.visible = true;
		//	//player.visible2 = false;
		//	//WirePreTimeCnt = 0;
		//	//player.wireFlag = false;
		//	//player.wireOkFlag = false;
		//	//player_state = PLAYER_NORMAL;
		//}

	}
	else
	{
		// �w�i�ȊO������
		WirePreTimeCnt = 0;
		//player.pos.x = furiko_RU.x - player.size.x / 2;
		//player.pos.y = furiko_RU.y + player.size.y  - mapPos.y;
		//player.visible = true;
		//player.visible2 = false;
		player.wireFlag = false;
		player.wireOkFlag = false;
		player_state = PLAYER_NORMAL;
	}

	//player.wireOkFlag = true;
	//player_state = PLAYER_Y_ACTION;
}


// �O�ƍ��W�̒��S�_���ς���Ă��邩�璍��!!
void PlWireAction(void)
{
	if (player.wireOkFlag)
	{
		if (WireTimeCnt < 200)
		{

			if (newKey[P2_B])
			{
				player_state = PLAYER_W_JUMP;
				return;
			}


			//Vec2 v = (furiko_pos - player.pos);//�U��q�̎x�_����U��q�̐��܂ł̃x�N�g��
			//v = v.normalized();//���K������

			////�O�ςƓ��ς𗘗p���Ċp�x���v�Z
			//float cost = Dot(v, Vec2(-1, 0));
			//float sint = Cross(v, Vec2(-1, 0));
			//float theta = atan2f(cost, sint);

			//_v += _g * cost;

			//OnMove(player.pos.x, player.pos.y, _v * sint, _v * cost);
			//OnAdjust();		// ���ꂪ�Ȃ��ƂЂ����L�тĂ���

			AddRad();

			WireTimeCnt++;
		}
		else
		{
			player.wireFlag = false;
			player.wireOkFlag = false;
			player.visible = true;
			player.visible2 = false;
			player_state = PLAYER_NORMAL;

			// ���C���[�\�����Ԃ̏�����
			WireTimeCnt = 0;

			// �U��q�X�^�[�g�ʒu�̏�����
			_v = 0;
			furiko_pos = { 0,0 };

			furikoSpeed = FURIKO_SPEED_DEF;


		}


	}
}

void AddRad(void)
{
	// �p�x�𑫂��񂾂悧������!!!!!!

	player.pos.x = cos((nowDeg * PI) / 180.0f) * _length.y + furiko_pos.x;
	player.pos.y = sin((nowDeg * PI) / 180.0f) * _length.y + furiko_pos.y;

	if (player.right == true)
	{
		if (player.moveDir == DIR_RIGHT)
		{
			// �v���C���[���E����


			if (nowDeg >= defDeg && furikoSpeed != FURIKO_SPEED_MIN)		// ���݂̍��W��deFDeg(-90)���傫�� = ���S���E���ɂ���Ƃ�
			{
				furikoSpeed = furikoSpeed - 0.15f;							// ����
			}

			nowDeg = nowDeg - furikoSpeed;		// 1���������Ă����


			if (nowDeg < defDeg && furikoSpeed != FURIKO_SPEED_MAX)			// ���݂̍��W��deFDeg(-90)��菬���� = ���S��荶���ɂ���Ƃ�
			{
				furikoSpeed = furikoSpeed + 0.15f;							// ����
			}

			if (nowDeg <= minDeg)
			{
				player.moveDir = DIR_LEFT;
			}
		}
		else
		{
			// �v���C���[��������


			if (nowDeg > defDeg && furikoSpeed != FURIKO_SPEED_MAX)			// ���݂̍��W��deFDeg(-90)���傫�� = ���S���E���ɂ���Ƃ�
			{
				furikoSpeed = furikoSpeed + 0.15f;							// ����
			}

			nowDeg = nowDeg + furikoSpeed;		// 1���������Ă����


			if (nowDeg <= defDeg && furikoSpeed != FURIKO_SPEED_MIN)		// ���݂̍��W��deFDeg(-90)���傫�� = ���S��荶���ɂ���Ƃ�
			{
				furikoSpeed = furikoSpeed - 0.15f;							// ����
			}

			if (nowDeg >= MaxDeg)
			{
				player.moveDir = DIR_RIGHT;
			}
		}
	}

	if (player.left == true)
	{
		if (player.moveDir == DIR_RIGHT)
		{
			// �v���C���[���E����
			nowDeg = nowDeg - furikoSpeed;		// 1���������Ă����


			if (nowDeg >= defDeg && furikoSpeed != FURIKO_SPEED_MIN)		// ���݂̍��W��deFDeg(-90)���傫�� = ���S���E���ɂ���Ƃ�
			{
				furikoSpeed = furikoSpeed - 0.15f;							// ����
			}



			if (nowDeg < defDeg && furikoSpeed != FURIKO_SPEED_MAX)			// ���݂̍��W��deFDeg(-90)��菬���� = ���S��荶���ɂ���Ƃ�
			{
				furikoSpeed = furikoSpeed + 0.15f;							// ����
			}

			if (nowDeg <= minDeg)
			{
				player.moveDir = DIR_LEFT;
			}
		}
		else
		{
			// �v���C���[��������
			if (nowDeg > defDeg && furikoSpeed != FURIKO_SPEED_MAX)			// ���݂̍��W��deFDeg(-90)���傫�� = ���S���E���ɂ���Ƃ�
			{
				furikoSpeed = furikoSpeed + 0.15f;							// ����
			}



			if (nowDeg <= defDeg && furikoSpeed != FURIKO_SPEED_MIN)		// ���݂̍��W��deFDeg(-90)���傫�� = ���S��荶���ɂ���Ƃ�
			{
				furikoSpeed = furikoSpeed - 0.15f;							// ����
			}

			nowDeg = nowDeg + furikoSpeed;		// 1���������Ă����


			if (nowDeg >= MaxDeg)
			{
				player.moveDir = DIR_RIGHT;
			}
		}
	}

	
}

void PlWireJump(void)
{
	player.visible = true;			// �A�j���[�V��������L�������\�������
	player.visible2 = false;		// ���C���[���̐Î~��L��������\���ɂȂ�
	player.wireFlag = false;		// ���C���[����\���ɂȂ�

	// ���n�n�_�̕`��ʒu
	/*if (player.moveDir == DIR_RIGHT)
	{
		player.pos.x = furiko_pos.x + mapPos.x - player.size.x / 2;
	}
	else
	{
		player.pos.x = furiko_pos.x - mapPos.x + player.size.x / 2;

	}*/
	//player.pos.x = furiko_pos.x + mapPos.x - player.size.x / 2;
	//player.pos.y = furiko_pos.y + mapPos.y + player.size.y;

	player_state = PLAYER_NORMAL;

}

// �ǂ𑖂�
// �E��
void PlWall_R(void)
{
	player.runFlag = false;
	player.jumpFlag = false;
	// �ʒu�C��
	player.pos.x = GetWorldPos_Map({ player.pos.x + player.moveSpeed + player.hitPosE.x , player.pos.y - 1 }, DIR_LEFT).x;

	Position player_RU = { player.pos.x - 1, player.pos.y - player.wallRunSpeed - player.hitPosE.x };

	// ��p�̕ǂ��H
	if (WallBlockPass(player.pos)) {
		// ��������Ȃ��E�r���ō~���
		if ((player.wallRunSpeed <= 0) || (newKey[P1_A])) {
			// �ʒu�C��
			player.pos.x = GetWorldPos_Map(player_RU, DIR_LEFT).x;
			if (oldKey[P1_LEFT]) {
				player.wallRunSpeed = player.moveSpeed;
				player_state = PLAYER_WALL_LEFT;
			}
			else {
				// �ʏ�ɖ߂�
				player_state = PLAYER_NORMAL;
			}
		}
		else if (player.wallRunSpeed >= 0) {
			// ���
			if (newKey[P1_RIGHT]) {
				player.runFlag = true;
				player.moveDir = DIR_RIGHT;
				if (IsPass(player_RU)) {	// �Ǌm�F
					player.pos.y -= player.wallRunSpeed;
				}
				else {
					player.pos.y = GetWorldPos_Map(player_RU, DIR_DOWN).y + player.hitPosS.x;
				}
			}
			player.wallRunSpeed -= 0.1f;	// ����
		}
	}
	else {
		// �ʏ�ɖ߂�
		player_state = PLAYER_NORMAL;
	}
}

// ����
void PlWall_L(void)
{
	player.runFlag = false;
	player.jumpFlag = false;
	// �ʒu�C��
	player.pos.x = GetWorldPos_Map({ player.pos.x - player.moveSpeed - player.hitPosS.x - 1 , player.pos.y - 1 }, DIR_RIGHT).x;

	Position player_LU = { player.pos.x, player.pos.y - player.moveSpeed - player.hitPosE.x };

	// ��p�̕ǂ��H
	if (WallBlockPass({ player.pos.x - 1,player.pos.y })) {
		// ��������Ȃ��E�r���ō~���
		if ((player.wallRunSpeed <= 0) || (newKey[P1_A])) {
			//�ʒu�C��
			player.pos.x = GetWorldPos_Map(player_LU, DIR_RIGHT).x;
			if (oldKey[P1_RIGHT]) {
				player.wallRunSpeed = player.moveSpeed;
				// �E�ǂɕύX
				player_state = PLAYER_WALL_RIGHT;
			}
			else {
				// �ʏ�ɖ߂�
				player_state = PLAYER_NORMAL;
			}
		}
		else if (player.wallRunSpeed >= 0) {
			// ���
			if (newKey[P1_LEFT]) {
				player.runFlag = true;
				player.moveDir = DIR_LEFT;
				if (IsPass(player_LU)) {	// �Ǌm�F
					player.pos.y -= player.moveSpeed;
				}
				else {
					player.pos.y = GetWorldPos_Map(player_LU, DIR_DOWN).y + player.hitPosE.x;
				}
			}
			player.wallRunSpeed -= 0.1f;	// ����
		}
	}
	else {
		// �ʏ�ɖ߂�
		player_state = PLAYER_NORMAL;
	}
}



void PlayerState(void)
{
	switch (player_state)
	{
	case PLAYER_NORMAL:			// ���E�ړ� 
	case PLAYER_DOWN:	        // �W�����v���~
		PlNormal();
		if (player_state != PLAYER_W_PRE)
		{
			PlDown();
		}
		break;
	case PLAYER_JUMP_UP:		// �W�����v�̏㏸
		PlNormal();
		if (player_state != PLAYER_W_PRE)
		{
			PlJumpUp();
		}
		break;
	case PLAYER_W_PRE:
		PlWirePrepare();		// ܲ԰����݂̏���(���C���[��L�΂��邩����)
		break;
	case PLAYER_W_ACTION:		// ܲ԰�����
		PlWireAction();
		break;
	case PLAYER_W_JUMP:			// ܲ԰�ެ���
		PlWireJump();
		break;
	case PLAYER_WALL_RIGHT:		// �E��
		PlWall_R();
		break;
	case PLAYER_WALL_LEFT:		// ����
		PlWall_L();
		break;
	default:
		break;
	}
	player.animCnt++;

}

