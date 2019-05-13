#include "Dxlib.h"				// DxLibײ���؂��g�p����@�Ǝ��ŏ�������ͯ�ް̧�ق�""�Ŏw�肷��
#include <math.h>				// �W��ͯ�ް̧�ق�<>�Ŏw�肷��
#include "keycheck.h"
#include "main.h"
#include "stage.h"
#include "player.h"

#define INIT_VELOCITY 50		// �������x
#define SECOND_PER_FRAME 0.3	// 1�ڰт̕b��

#define CORRECTION	  100		// ���C���[�̒����␳�p���l(����y���W)

#define KEEPPOSX_CORRECTION  120	// pos�̒l�̕␳�p���l

//#define PAI 3.141592
//#define JUMPSPEED 2

PLAYER_STATE player_state;

// ��ڲ԰��߰��
enum PLAYER_SPEED {
	PLAYER_SPEED_NORMAL = 8,
	PLAYER_SPEED_SEGWEY = 12
};
//int hatenaImage;
int p1Wak[2];
int yazirusiImage[2];

int playerImage[2];
int playerIcon[2];
int runImage[2][10];
int jumpImage[2];
int stopJumpImage[2];
int segweyImage[2][2];
CHARACTER player;
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
	//hatenaImage = LoadGraph("image/hatena.png");

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

}

void PlayerGameInit(void)
{
	player.type = PLAYER_RED;
	player.size = { 72,72 };
	player.offsetSize = { player.size.x / 2,player.size.y / 2 };
	player.hitPosE = { 20,36 };
	player.hitPosS = { 20,26 };
	//player.pos = { CHIP_SIZE_X * 4,CHIP_SIZE_Y * 13 - 25 };// �����ʒu
	player.pos = {CHIP_SIZE_X * 5,SCREEN_SIZE_Y+CHIP_SIZE_Y * 13 - 25 };// �����ʒu
	player.moveSpeed = PLAYER_SPEED_NORMAL;
	player.animCnt = 0;
	player.moveDir = DIR_RIGHT;
	player.runFlag = false;
	player.jumpFlag = false;
	player.jumpCnt = 0;			// �ެ��߂ł����
	player.downFlag = false;
	player.segweyFlag = false;	// ��޳��
	player.wireFlag = false;

	player.visible = false;
	player.visible2 = false;
	player.imgLocCnt = 0;

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
	player_state = PLAYER_NORMAL;

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
		if (trgKey[P1_UP]) player.visible = !player.visible;
		break;

	case GMODE_GAME:	// �ްђ�
		int Pad1;
		Pad1 = GetJoypadInputState(DX_INPUT_PAD1);
		//ItemHit(player.pos);

		player.runFlag = false;
		player.jumpFlag = true;
		player.moveSpeed = PLAYER_SPEED_NORMAL;
		player.imgLocCnt++;

		XY movedPos = player.pos;
		XY movedHitCheck = movedPos;
		XY movedHitCheck2;
		XY movedHitCheck3;
		XY wallChek = movedPos;

		// ���C���[����
		if (newKey[P2_UP])
		{
			movedPos = player.pos;
			// 5~10�}�X�ȓ��̓���Ƀu���b�N����������A���C���[���o����悤�ɂ�����
			for (int i = 5; i <= 10; )
			{
				movedHitCheck.y  = movedPos.y - player.offsetSize.y - (CHIP_SIZE_Y * i);		// ����

				movedHitCheck2   = movedHitCheck;												// 2 = ��
				movedHitCheck2.x = movedPos.x - player.hitPosS.x - CHIP_SIZE_X;

				movedHitCheck3   = movedHitCheck;												// 3 = �E
				movedHitCheck3.x = movedPos.x + player.hitPosE.x - 1 + CHIP_SIZE_X;

				// ���͈͓��Ƀu���b�N�����݂��Ȃ����ƂɂȂ�
				if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3)))		
				//if ((WireBlockPass(movedHitCheck)) && (WireBlockPass(movedHitCheck2)) && (WireBlockPass(movedHitCheck3)))		// ���͈͓��Ƀu���b�N�����݂��Ȃ����ƂɂȂ�
				{
					player.wireFlag = false;			// �͈͓��ɑ��݂��Ȃ��̂�false��������
					i++;
				}
				else
				{
					// �u���b�N�����݂���Ƃ��̏���		//�R�̒����̌v�Z
					_length = player.pos.y - mapPos.y - player.offsetSize.y;
					if (_length >= 250)
					{
						_length = 250;
					}
					else
					{
						_length = player.pos.y - mapPos.y - player.offsetSize.y;
					}
					// KeepPosX�̕␳
					if (player.pos.x - mapPos.x < KEEPPOSX_CORRECTION)
					{
						KeepPosX = KEEPPOSX_CORRECTION;
					}
					else
					{
						KeepPosX = player.pos.x - mapPos.x;
					}

					// KeepPosY���w��u���b�N�̍����ɍ��킹��΂��������ȋC������
					KeepPosY = movedHitCheck.y - CHIP_SIZE_Y / 4 - mapPos.y;
					player.wireFlag = true;
					player.visible  = false;
					player.visible2 = true;

					TimeCnt = 0;
					return;
				}
			}
		}

		// ��޳��
		if (trgKey[P2_A]) player.segweyFlag = !player.segweyFlag;
		if (player.segweyFlag) player.moveSpeed = PLAYER_SPEED_SEGWEY;

		//// �ړ�
		//if (player.visible && !player.visible2) {
		//	if (oldKey[P1_RIGHT] || (Pad1 & PAD_INPUT_RIGHT)) {
		//		player.runFlag = true;
		//		player.moveSpeed = player.moveSpeed;
		//		player.moveDir = DIR_RIGHT;
		//	}
		//	else if (oldKey[P1_LEFT] || (Pad1 & PAD_INPUT_LEFT)) {
		//		player.runFlag = true;
		//		player.moveSpeed = -player.moveSpeed;
		//		player.moveDir = DIR_LEFT;
		//	}

			// �ެ���
			if (player.jumpFlag) {
				movedPos.y -= player.velocity.y * SECOND_PER_FRAME;		// �����̍X�V
				player.velocity.y -= ACC_G * SECOND_PER_FRAME;			// ���x�̍X�V	�萔���萔�Ȃ�ŏ�����ꏏ�ɂ���

				// ���̏�
				movedHitCheck.y  = movedPos.y - player.hitPosS.y;
				movedHitCheck2	 = movedHitCheck;
				movedHitCheck2.x = movedPos.x - player.hitPosS.x;
				movedHitCheck3	 = movedHitCheck;
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
				movedHitCheck.y  = movedPos.y + player.offsetSize.y;
				movedHitCheck2	 = movedHitCheck;													// ��
				movedHitCheck2.x = movedPos.x - player.hitPosS.x;
				movedHitCheck3	 = movedHitCheck;													// �E
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
			//if ((!player.jumpFlag) && (trgKey[P1_UP]) || (Pad1 & PAD_INPUT_UP)) {
			//	player.jumpFlag = true;
			//	player.jumpCnt++;
			//	player.velocity.y = INIT_VELOCITY;
			//}

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
		
		// player��ǂ��J���� 
		// ������
		//if (player.pos.y > SCREEN_SIZE_Y - CHIP_SIZE_Y * 5)mapPos.y += ACC_G ;
		// �オ��
		//if ((player.pos.y > CHIP_SIZE_X * MAP_X - SCREEN_SIZE_X + CHIP_SIZE_X * 5)mapPos.y -= ACC_G*SECOND_PER_FRAME;

		if (player.pos.y>SCREEN_SIZE_Y)mapPos.y += ACC_G/2;	// ������
		if (player.pos.y < SCREEN_SIZE_Y+200)mapPos.y -= ACC_G ;// �オ��

		//// ���E�ɂ��킶�퓮��
		//if (player.pos.x > SCREEN_SIZE_X - CHIP_SIZE_X * 5) mapPos.x += 1;
		//if (player.pos.x < CHIP_SIZE_X * MAP_X - SCREEN_SIZE_X + CHIP_SIZE_X * 5) mapPos.x -= SECOND_PER_FRAME;
		break;
	}
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
			if ((player.jumpCnt > 0) && (!player.runFlag)) img = stopJumpImage[player.type];
			if (player.segweyFlag) img = segweyImage[player.type][(player.animCnt / 5) % 2];
			if (player.moveDir == DIR_RIGHT) {
				DrawGraph(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y, img, true);
			}
			else if (player.moveDir == DIR_LEFT) {
				DrawTurnGraph(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y, img, true);
			}
			DrawBox(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - player.offsetSize.y - mapPos.y,
					player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y, 0xff0000, false);
			DrawBox(player.pos.x - player.hitPosS.x - mapPos.x, player.pos.y - player.hitPosS.y - mapPos.y,
					player.pos.x + player.hitPosE.x - mapPos.x, player.pos.y + player.hitPosE.y - mapPos.y, 0x00ffff, false);
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
			if (player.moveDir == DIR_RIGHT)
			{
				// �������ǃL�����ɌŒ肳��Ȃ��Ђ�(����������)
				DrawLine(_pos.x, _pos.y, KeepPosX + CHIP_SIZE_X, KeepPosY /*+ CHIP_SIZE_Y*/, 0xffffffff, 2);
				// �L�����N�^��������Ƃ��ĕ`��
				DrawGraph(_pos.x - player.size.x, _pos.y, jumpImage[player.type], true);							
			}
			else
			{
				// �������ǃL�����ɌŒ肳��Ȃ��Ђ�(����������)
				DrawLine(_pos.x, _pos.y, KeepPosX - CHIP_SIZE_X, KeepPosY /*+ CHIP_SIZE_Y*/, 0xffffffff, 2);	
				DrawTurnGraph(_pos.x, _pos.y, jumpImage[player.type], true);
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

void PlayerState(void)
{
	if (player_state == PLAYER_NORMAL)
	{
		// �ړ�
		if (player.visible && !player.visible2)
		{
			if (oldKey[P1_RIGHT]) {
				player.runFlag = true;
				player.moveSpeed = player.moveSpeed;
				player.moveDir = DIR_RIGHT;
			}
			else if (oldKey[P1_LEFT]) {
				player.runFlag = true;
				player.moveSpeed = -player.moveSpeed;
				player.moveDir = DIR_LEFT;
			}
			if ((!player.jumpFlag) && (trgKey[P1_UP])) {
				player.jumpFlag = true;
				player.jumpCnt++;
				player.velocity.y = INIT_VELOCITY;
				player_state = PLAYER_JUMP_UP;
			}
		}
	}

	if (player_state == PLAYER_JUMP_UP)
	{

	}
}


//bool ItemHit(XY pos)
//{
//	bool hatenaFlag = false;
//	if (hatenaFlag == true);
//	if (player.pos.x = (SCREEN_SIZE_X / 2 - mapPos.x)&&(player.pos.y= SCREEN_SIZE_Y + 500 - mapPos.y))
//	{
//		DrawGraph(SCREEN_SIZE_X / 2 - mapPos.x, SCREEN_SIZE_Y + 500 - mapPos.y, hatenaImage, false);
//	}
//	else
//	{
//		DrawGraph(SCREEN_SIZE_X / 2 - mapPos.x, SCREEN_SIZE_Y + 500 - mapPos.y, hatenaImage, true);
//	}
//		SCREEN_SIZE_X / 2 ����	
//		&&SCREEN_SIZE_X / 2 + CHIP_SIZE_X �E��
//		&&SCREEN_SIZE_Y + 500 ����	
//		&&SCREEN_SIZE_Y + 500+CHIP_SIZE_Y�@�E��
//}



void WireDraw(void)
{
	// ���C���[�A�N�V�������͂��߂���A���S���W���Œ�

	// W��F�𓯎��ɉ������Ƃ��̃o�O��������

	if (player.wireFlag)
	{
		if (TimeCnt < 150)
		{
			//�Ђ��̎x�_���`����
			if (player.moveDir == DIR_RIGHT)
			{
				_endPoint.x = KeepPosX + CHIP_SIZE_X;
			}
			else
			{
				_endPoint.x = KeepPosX - CHIP_SIZE_X;
			}
			_endPoint.y = KeepPosY /*+ CHIP_SIZE_Y*/;

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

				// ���n�n�_�̕`��ʒu
				player.pos.x = _pos.x + mapPos.x + player.offsetSize.x;
				player.pos.y = _pos.y + mapPos.y;



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

bool GetPlayerV(void)
{
	return player.visible;
}