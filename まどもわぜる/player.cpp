
#include "Dxlib.h"	//DxLibײ���؂��g�p����@�Ǝ��ŏ�������ͯ�ް̧�ق�""�Ŏw�肷��
#include <math.h>	// �W��ͯ�ް̧�ق�<>�Ŏw�肷��
#include"keycheck.h"
#include"main.h"
#include"stage.h"
#include"player.h"
#include"shot.h"

#define INIT_VELOCITY 50	// �������x
#define SECOND_PER_FRAME 0.3	// 1�ڰт̕b��
#define SPEED 8


int playerImage;
int runImage[10];
int jumpImage;
int stopJumpImage;
int downImage;
int shotImage[2];
CHARACTER player;
int downPos;

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

float KeepPos;		// ���W�̕ۑ��p


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

	//�Ђ��̎x�_�̏�����
	_endPoint.x = 0;
	_endPoint.y = 0;
	_v = 0;


	_g = 1.2f;		//�d�͂̒�`
	_length = 200;	//�R�̒����̌v�Z

	KeepPos = 0;	// ���W�̕ۑ��p

	
}

void PlayerGameInit(void)
{
	player.size = { 72,72 };
	player.offsetSize = { player.size.x / 2,player.size.y / 2 };
	player.hitPosE = { 20,36 };
	player.hitPosS = { 20,26 };
	player.pos = { CHIP_SIZE_X * 4,CHIP_SIZE_Y * 13 - 25 };
	player.moveSpeed = 4;
	player.animCnt = 0;
	player.moveDir = DIR_RIGHT;
	player.runFlag = false;
	player.jumpFlag = false;
	player.shotFlag = false;
	player.downFlag = false;

	player.wireFlag = false;

	player.visible = true;
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
	player.imgLocCnt++;
	downPos = 0;

	XY movedPos = player.pos;
	XY movedHitCheck = movedPos;
	XY movedHitCheck2;
	XY movedHitCheck3;

	// �ړ�
	if (player.visible) {
		if (oldKey[P1_RIGHT] || (Pad1 & PAD_INPUT_RIGHT)) {
			player.runFlag = true;
			player.moveSpeed = SPEED;
			player.moveDir = DIR_RIGHT;
		}
		else if (oldKey[P1_LEFT] || (Pad1 & PAD_INPUT_LEFT)) {
			player.runFlag = true;
			player.moveSpeed = -SPEED;
			player.moveDir = DIR_LEFT;
		}

		// �ެ���
		if (player.jumpFlag) {

			if (newKey[P2_UP] )
			{
				KeepPos = player.pos.x - mapPos.x;
				player.wireFlag = true;
			}

			movedPos.y -= player.velocity.y * SECOND_PER_FRAME;	// �����̍X�V
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
			}
			else {
				player.pos.y = MapPos(movedHitCheck, DIR_UP).y - player.offsetSize.y;		// �������璆�S
				player.velocity.y = 0;
				player.jumpFlag = false;
			}


		}
		if ((!player.jumpFlag) && (trgKey[P1_UP]) || (Pad1 & PAD_INPUT_UP)) {
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
}

void PlayerDraw(void)
{
	int img = playerImage;
	if ((player.runFlag) && (!player.jumpFlag)) img = runImage[(player.animCnt / 3) % 10];
	if ((player.jumpFlag) && (player.runFlag)) img = jumpImage;
	if ((player.jumpFlag) && (!player.runFlag)) img = stopJumpImage;
	if (player.downFlag) img = downImage;
	if ((!player.runFlag) && (oldKey[P1_A])) img = shotImage[0];
	if ((!player.runFlag) && (oldKey[P1_A]) && (player.downFlag)) img = shotImage[1];
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

void WireDraw(void)
{
	// ����͍��W�Ǘ��̎d����player.pos��_pos�ňႤ������Ȃ�����

	// ���C���[�A�N�V�������͂��߂���A���S���W���Œ�

	if (player.wireFlag == true)
	{

		//�Ђ��̎x�_���`����
		_endPoint.x = KeepPos + 100;
		_endPoint.y = 0;
		//_v = 0;					// ����������ɏ����ƃX���[���[�V�����ɂȂ�

		Vec2 v = (_pos - _endPoint);//�U��q�̎x�_����U��q�̐��܂ł̃x�N�g��
		v = v.normalized();//���K������
		//�O�ςƓ��ς𗘗p���Ċp�x���v�Z
		float cost = Dot(v, Vec2(-1, 0));
		float sint = Cross(v, Vec2(-1, 0));
		float theta = atan2f(cost, sint);

		// �����x�����x�����ꂼ��̃x�N�g����


		_v += _g * cost;
		
		//���Ƃ͐U��q�̊p�x�ɏ]���āA���̎��X�̉����x�����߁A
		//���x(_v)�ɉ��Z���悤
		//�����X�����AY�����ɕ�����
		//OnMove�̑�3��4�����ɑ�����Ă�������

		OnMove(_pos.x, _pos.y, _v * sint, _v * cost);//��3�����A��4������������Ɛݒ肵�悤

		//�␳����
		OnAdjust();			// �����̕␳�������Ȃ��ƁA�Ђ����L�тĂ������璍��!!

		// �Ђ��Ƃ�����̕`�悪2������͉̂A�e�����邽�߁B
		//DrawLine(player.pos.x - player.offsetSize.x - mapPos.x + 1, player.pos.y - player.offsetSize.y - mapPos.y, 320, 0, 0x808080, 1);//�Ђ��`��
		//DrawLine(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y, 320, 0, 0xffffffff, 2);//�Ђ��`��
		//DrawCircle(_pos.x + 1, _pos.y + 1, 20, 0x000000);//������`��
		//DrawCircle(_pos.x, _pos.y, 20, 0x008000);//������`��
	    //DrawGraph(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y, playerImage, true);				// �L�����N�^��������Ƃ��ĕ`��


		DrawLine(KeepPos, player.pos.y, KeepPos+100, 0, 0xffffffff, 1);		// �����Ȃ����ǃL�����ɌŒ肳���Ђ�

		

		DrawLine(_pos.x, _pos.y, KeepPos + 100, 0, 0xffffffff, 2);			// �������ǃL�����ɌŒ肳��Ȃ��Ђ�

		player.wireFlag = false;

	}
}

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