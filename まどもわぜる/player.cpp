
#include "Dxlib.h"				// DxLibײ���؂��g�p����@�Ǝ��ŏ�������ͯ�ް̧�ق�""�Ŏw�肷��
#include <math.h>				// �W��ͯ�ް̧�ق�<>�Ŏw�肷��
#include"keycheck.h"
#include"main.h"
#include"stage.h"
#include"player.h"
#include"shot.h"

#define INIT_VELOCITY 50		// �������x
#define SECOND_PER_FRAME 0.3	// 1�ڰт̕b��

#define CORRECTION	  100		// ���C���[�̒����␳�p���l(����y���W)

#define KEEPPOSX_CORRECTION  120	// pos�̒l�̕␳�p���l

//#define PAI 3.141592
//#define JUMPSPEED 2

// ��ڲ԰��߰��
enum PLAYER_SPEED {
	PLAYER_SPEED_NORMAL = 8,
	PLAYER_SPEED_SEGWEY = 12
};

int playerImage;
int runImage[10];
int jumpImage;
int stopJumpImage;
int downImage;
int shotImage[2];
int segweyImage[2];
CHARACTER player;
int downPos;
int TimeCnt;					// ���C���[�̕\������

bool _isPushJump;
bool _isJumped;

void OnMove(float& x, float& y, float vx, float vy);
void OnAdjust();

struct Position {
	Position(float ix, float iy) { x = ix; y = iy; }
	Position() {
		x = 0;
		y = 0;
	}
	float x;
	float y;
	float Length() {
		return hypotf(x, y);
	}
	Position normalized() {
		return Position(x / Length(), y / Length());
	}
	Position operator+(const Position & in) {
		return Position(x + in.x, y + in.y);
	}

	Position operator-(const Position & in) {
		return Position(x - in.x, y - in.y);
	}

	Position operator*(float s) {
		return Position(x * s, y * s);
	}



};

Position _pos;
Position _endPoint;
float _g;
float _v;
float _length;

float KeepPosX;		// ���W�̕ۑ��p
float KeepPosY;		// ���W�̕ۑ��p

float _vx;
float _vy;

//float rot;

typedef Position Vec2;

inline float Dot(const Vec2 & a, const Vec2 & b) {
	return a.x* b.x + a.y * b.y;
}

inline float Cross(const Vec2 & a, const Vec2 & b) {
	return a.x* b.y - b.x * a.y;
}


void PlayerSystmInit(void)
{
	playerImage = LoadGraph("image/playerR_stop.png");
	jumpImage = LoadGraph("image/playerR_jump.png");
	stopJumpImage = LoadGraph("image/playerR_stop_jump.png");
	downImage = LoadGraph("image/red_down.png");
	shotImage[0] = LoadGraph("image/red_stop_shot.png");
	shotImage[1] = LoadGraph("image/red_down_shot.png");
	LoadDivGraph("image/playerR_run.png", 10, 5, 2, 72, 72, runImage, true);
	LoadDivGraph("image/playerR_segway.png", 2, 2, 1, 72, 72, segweyImage, true);

	//�Ђ��̎x�_�̏�����
	_endPoint.x = 0;
	_endPoint.y = 0;
	_v = 0;			// �U��q�̂ӂ蕝

	_g = 2.0f;		//�d�͂̒�`
	_length = 0;	//�R�̒����̌v�Z

	KeepPosX = 0;	// ���W�̕ۑ��p
	KeepPosY = 0;	// ���W�̕ۑ��p

	_vx = 0;
	_vy = 0;

	_isPushJump = false;
	_isJumped = false;

	//rot = -rand() % 90;
}

void PlayerGameInit(void)
{
	player.size = { 72,72 };
	player.offsetSize = { player.size.x / 2,player.size.y / 2 };
	player.hitPosE = { 20,36 };
	player.hitPosS = { 20,26 };
	player.pos = { CHIP_SIZE_X * 4,CHIP_SIZE_Y * 13 - 25 };
	player.moveSpeed = PLAYER_SPEED_NORMAL;
	player.animCnt = 0;
	player.moveDir = DIR_RIGHT;
	player.runFlag = false;
	player.jumpFlag = false;
	player.jumpCnt = 0;	// �ެ��߂ł����
	player.shotFlag = false;
	player.downFlag = false;
	player.segweyFlag = false;	// ��޳��

	player.wireFlag = false;

	player.visible = true;
	player.visible2 = false;
	player.imgLocCnt = 0;
	downPos = 0;
}

void PlayerControl(void)
{
	int Pad1;
	Pad1 = GetJoypadInputState(DX_INPUT_PAD1);



	player.runFlag = false;
	player.jumpFlag = true;
	player.downFlag = false;
	player.moveSpeed = PLAYER_SPEED_NORMAL;
	player.imgLocCnt++;
	downPos = 0;

	XY movedPos = player.pos;
	XY movedHitCheck = movedPos;
	XY movedHitCheck2;
	XY movedHitCheck3;

	// ���C���[����
	if (newKey[P2_UP])
	{
		_length = player.pos.y - mapPos.y - player.offsetSize.y;	//�R�̒����̌v�Z
		// KeepPosX�̕␳
		if (player.pos.x - mapPos.x < KEEPPOSX_CORRECTION)
		{
			KeepPosX = KEEPPOSX_CORRECTION;
		}
		else
		{
			KeepPosX = player.pos.x - mapPos.x;
		}
		KeepPosY = 0;
		player.wireFlag = true;
		player.visible = false;
		player.visible2 = true;

		TimeCnt = 0;

	}

	// ��޳��
	if (trgKey[P2_A]) player.segweyFlag = !player.segweyFlag;
	if (player.segweyFlag) player.moveSpeed = PLAYER_SPEED_SEGWEY;

	// �ړ�
	if (player.visible && !player.visible2) {
		if (oldKey[P1_RIGHT] || (Pad1 & PAD_INPUT_RIGHT)) {
			player.runFlag = true;
			player.moveSpeed = player.moveSpeed;
			player.moveDir = DIR_RIGHT;
		}
		else if (oldKey[P1_LEFT] || (Pad1 & PAD_INPUT_LEFT)) {
			player.runFlag = true;
			player.moveSpeed = -player.moveSpeed;
			player.moveDir = DIR_LEFT;
		}

		// �ެ���
		if (player.jumpFlag) {
			movedPos.y -= player.velocity.y * SECOND_PER_FRAME;		// �����̍X�V
			player.velocity.y -= ACC_G * SECOND_PER_FRAME;			// ���x�̍X�V	�萔���萔�Ȃ�ŏ�����ꏏ�ɂ���

																	// ���̏�
			movedHitCheck.y = movedPos.y - player.hitPosS.y;
			movedHitCheck2 = movedHitCheck;
			movedHitCheck2.x = movedPos.x - player.hitPosS.x;
			movedHitCheck3 = movedHitCheck;
			movedHitCheck3.x = movedPos.x + player.hitPosE.x - 1;

			if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3))) {
				player.pos.y = movedPos.y;
			}
			else {
				player.pos.y = MapPos(movedHitCheck, DIR_DOWN).y + player.hitPosE.y;
				player.velocity.y *= -1;
			}


			// ��������ۯ������邩
			movedPos = player.pos;
			movedHitCheck.y = movedPos.y + player.offsetSize.y;
			movedHitCheck2 = movedHitCheck;
			movedHitCheck2.x = movedPos.x - player.hitPosS.x;
			movedHitCheck3 = movedHitCheck;
			movedHitCheck3.x = movedPos.x + player.hitPosE.x - 1;
			if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3))) {
				player.pos.y = movedPos.y;
				if (player.jumpCnt < 2) {

					player.jumpFlag = false;
				}
			}
			else {
				player.pos.y = MapPos(movedHitCheck, DIR_UP).y - player.offsetSize.y;		// �������璆�S
				player.velocity.y = 0;
				player.jumpCnt = 0;
				player.jumpFlag = false;
			}


		}
		if ((!player.jumpFlag) && (trgKey[P1_UP]) || (Pad1 & PAD_INPUT_UP)) {
			player.jumpFlag = true;
			player.jumpCnt++;
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
				movedHitCheck.x = movedPos.x + player.hitPosE.x;
				break;
			case DIR_LEFT:
				movedHitCheck.x = movedPos.x - player.hitPosS.x - 1;
				break;
			}

			movedHitCheck2 = movedHitCheck;
			movedHitCheck2.y = movedPos.y - player.hitPosS.y;
			movedHitCheck3 = movedHitCheck;
			movedHitCheck3.y = movedPos.y + player.offsetSize.y - 1;
			if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3))) {
				player.pos = movedPos;
			}
			else {
				// �ǂɂ҂����肭������
				if (player.moveDir == DIR_RIGHT) {
					player.pos.x = MapPos(movedHitCheck, DIR_LEFT).x - player.hitPosS.x;
				}
				else if (player.moveDir == DIR_LEFT) {
					player.pos.x = MapPos(movedHitCheck, DIR_RIGHT).x + player.hitPosE.x;
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


	if ((!player.jumpFlag) && (!player.runFlag) && (oldKey[P1_DOWN]) || (Pad1 & PAD_INPUT_DOWN)) {
		player.downFlag = true;
		downPos = 8;
	}

	// ����
	if ((!player.shotFlag) && (oldKey[P1_A])) {
		player.imgLocCnt = 0;
		player.shotFlag = true;
		if (player.moveDir == DIR_RIGHT) {
			Shoot({ player.pos.x, player.pos.y - 12 + downPos }, player.moveDir, player.downFlag);
		}
		else if (player.moveDir == DIR_LEFT) {
			Shoot({ player.pos.x,player.pos.y - 12 + downPos }, player.moveDir, player.downFlag);
		}
	}
	else {

		if (player.imgLocCnt > 6) {
			player.shotFlag = false;
		}
	}

	// player��ǂ��J����
	if (player.pos.y > SCREEN_SIZE_Y - CHIP_SIZE_Y * 5) mapPos.y += ACC_G / 2;
	if (player.pos.y < CHIP_SIZE_Y * MAP_Y - SCREEN_SIZE_Y + CHIP_SIZE_Y * 5) mapPos.y -= ACC_G * SECOND_PER_FRAME;


	//// ���E�ɂ��킶�퓮��
	//if (player.pos.x > SCREEN_SIZE_X - CHIP_SIZE_X * 5) mapPos.x += 1;
	//if (player.pos.x < CHIP_SIZE_X * MAP_X - SCREEN_SIZE_X + CHIP_SIZE_X * 5) mapPos.x -= SECOND_PER_FRAME;

}

void PlayerDraw(void)
{
	if (player.visible && !player.visible2)
	{
		int img = playerImage;
		if ((player.runFlag) && (!player.jumpFlag)) img = runImage[(player.animCnt / 3) % 10];
		if ((player.jumpFlag) && (player.runFlag)) img = jumpImage;
		if ((player.jumpFlag) && (!player.runFlag)) img = stopJumpImage;
		if (player.downFlag) img = downImage;
		if ((!player.runFlag) && (oldKey[P1_A])) img = shotImage[0];
		if ((!player.runFlag) && (oldKey[P1_A]) && (player.downFlag)) img = shotImage[1];
		if (player.segweyFlag) img = segweyImage[(player.animCnt / 5) % 2];
		if (player.moveDir == DIR_RIGHT) {

			DrawGraph(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y, img, true);
		}
		else if (player.moveDir == DIR_LEFT) {
			DrawTurnGraph(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y, img, true);
		}
		if (player.downFlag) {
			DrawString(780, 0, "PLAYERDOWN OK", 0xffffff);
		}
		DrawBox(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y,
			player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y, 0xff0000, false);
		DrawBox(player.pos.x - player.hitPosS.x - mapPos.x, player.pos.y - player.hitPosS.y - mapPos.y + downPos,
			player.pos.x + player.hitPosE.x - mapPos.x, player.pos.y + player.hitPosE.y - mapPos.y + downPos, 0x00ffff, false);
		DrawLine(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - mapPos.y,
			player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y - mapPos.y, 0x00ffff, true);
		DrawLine(player.pos.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y,
			player.pos.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y, 0x00ffff, true);
	}

	if (!player.visible && player.visible2)
	{
		// ���C���[�����܂�ɂ��Z�������Ƃ��̑Ώ�
		if (_pos.y < CORRECTION)
		{
			_pos.y = CORRECTION;
		}
		DrawLine(_pos.x, _pos.y, KeepPosX + CHIP_SIZE_X, KeepPosY + CHIP_SIZE_Y, 0xffffffff, 2);			// �������ǃL�����ɌŒ肳��Ȃ��Ђ�

		DrawGraph(_pos.x, _pos.y, playerImage, true);														// �L�����N�^��������Ƃ��ĕ`��
	}

}

void WireDraw(void)
{
	// ���C���[�A�N�V�������͂��߂���A���S���W���Œ�

	// W��F�𓯎��ɉ������Ƃ��̃o�O��������

	if (player.wireFlag)
	{
		if (TimeCnt < 150)
		{
			//�Ђ��̎x�_���`����
			_endPoint.x = KeepPosX + CHIP_SIZE_X;
			_endPoint.y = KeepPosY + CHIP_SIZE_Y;

			Vec2 v = (_pos - _endPoint);//�U��q�̎x�_����U��q�̐��܂ł̃x�N�g��
			v = v.normalized();//���K������
			//�O�ςƓ��ς𗘗p���Ċp�x���v�Z
			float cost = Dot(v, Vec2(-1, 0));
			float sint = Cross(v, Vec2(-1, 0));
			float theta = atan2f(cost, sint);

			_v += _g * cost;


			if (CheckHitKey(KEY_INPUT_F))		// F�L�[����������
			{
				//float playerX = 0;

				player.visible = true;			// �A�j���[�V��������L�������\�������
				player.visible2 = false;		// ���C���[���̐Î~��L��������\���ɂȂ�
				player.wireFlag = false;		// ���C���[����\���ɂȂ�

				//playerX = cos(rot*PAI / 180)*JUMPSPEED;
				//player.pos.x += playerX;
				player.pos.x = _pos.x + mapPos.x + player.size.x/2;
				player.pos.y = _pos.y + mapPos.y;

				
			



















				//--------------------------------------------------------------

				// �E��
				//if (player.pos.x >=  SCREEN_SIZE_X)
				//{
				//	player.pos.x = _pos.x + SCREEN_SIZE_X;
				//	player.pos.y = _pos.y;
				//}
				////else
				////{
				////	// ����
				////	player.pos.x = _pos.x;
				////}
				////--------------------------------------------------------------

				////
				//if ((player.pos.y = _pos.y + SCREEN_SIZE_Y)&& (player.pos.x >= SCREEN_SIZE_X))
				//{
				//	//_pos.y = _pos.y + SCREEN_SIZE_Y;
				//	player.pos.y = _pos.y + SCREEN_SIZE_Y;
				//	player.pos.x = _pos.x + SCREEN_SIZE_X;
				//}
				////else
				//{
				//	// ����
				//	player.pos.y = _pos.y;
				//}


				// �����ɓ�i�W�����v�p�����̒ǉ����K�v����

				//if (!_isPushJump)
				//{
				//	OnPushJumpKey(_vx, _vy);
				//}
				//_isPushJump = true;
				//if (_pos.y >= CHIP_SIZE_Y * 12) {//�n�ʂɓ���������c
				//	OnGround(_pos.x, _pos.y, _vx, _vy);
				//}
				PlayerDraw();
			}
			else
			{
				player.visible = false;
				player.visible2 = true;
				player.wireFlag = true;
				_isPushJump = false;
			}





			//���Ƃ͐U��q�̊p�x�ɏ]���āA���̎��X�̉����x�����߁A
			//���x(_v)�ɉ��Z���悤
			//�����X�����AY�����ɕ�����
			//OnMove�̑�3��4�����ɑ�����Ă�������

			/*if (_isJumped) {
				OnMove(_pos.x, _pos.y, _vx, _vy);
				_vy += _g;
			}
			else {*/
			OnMove(_pos.x, _pos.y, _v * sint, _v * cost);
			OnAdjust();		// ���ꂪ�Ȃ��ƂЂ����L�тĂ���
		//}

		//DrawLine(KeepPos, player.pos.y, KeepPos+100, 0, 0xffffffff, 1);		// �����Ȃ����ǃL�����ɌŒ肳���Ђ�

		//DrawLine(_pos.x, _pos.y, KeepPosX + CHIP_SIZE_X, KeepPosY + CHIP_SIZE_Y, 0xffffffff, 2);			// �������ǃL�����ɌŒ肳��Ȃ��Ђ�

		//DrawGraph(_pos.x, _pos.y, playerImage, true);						// �L�����N�^��������Ƃ��ĕ`��

			TimeCnt++;
		}
		else
		{
			player.wireFlag = false;
			player.visible = true;
			player.visible2 = false;
		}

	}
}

//void OnPushJumpKey(float& vx, float& vy)
//{
//	Vec2 v = (_pos - _endPoint);
//	v = v.normalized();
//	_isJumped = true;
//	float theta = atan2f(Cross(v, Vec2(-1, 0)), Dot(v, Vec2(-1, 0)));
//	float cost = Dot(v, Vec2(-1, 0));
//	float sint = Cross(v, Vec2(-1, 0));
//	vx = _v * sint;
//	vy = _v * cost;
//	
//
//}
//
//void OnGround(float& x, float& y, float& vx, float& vy)
//{
//	
//}

void OnMove(float& x, float& y, float vx, float vy) {
	//�@�����Ɉړ��̂��߂̏�����`���Ă��������B
	x += vx;
	y += vy;
}

void OnAdjust() {
	Vec2 v = (_pos - _endPoint);
	if (v.Length() > _length) {
		_pos = _endPoint + v.normalized() * _length;
	}
}