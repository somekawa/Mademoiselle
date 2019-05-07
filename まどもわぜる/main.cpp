#include <math.h>
#include <vector>
#include "Dxlib.h"	//DxLibײ���؂��g�p����@�Ǝ��ŏ�������ͯ�ް̧�ق�""�Ŏw�肷��
#include"keycheck.h"
#include"main.h"
#include"player.h"
#include"shot.h"
#include"stage.h"

#define PI 3.141592

// ----------�ϐ���`----------
typedef enum
{
	GMODE_INIT,
	GMODE_TITLE,
	GMODE_CHARASERE,
	GMODE_GAME,
	GMODE_OVER,
	GMODE_MAX
}GAME_MODE;




GAME_MODE  gameMode;
int pauseFlag;
int gameCnt;
int fadeCnt;
bool fadeIn;
bool fadeOut;

int selectImage1;
int selectImage2;
int titleImage;

XY mapPos;

int SystmInit(void);
void GameInit(void);
void GameTitle(void);
void GameTitleDraw(void);
void GameCharasere(void);
void GameCharasereDraw(void);
void GameMain(void);
void GameMainDraw(void);
void GameOver(void);
void GameOverDraw(void);
void HitCheck(void);
//void OnMove(float& x, float& y, float vx, float vy);
//void OnAdjust();


bool FadeInScreen(int fadeStep);
bool FadeOutScreen(int fadeStep);

//struct Position {
//	Position(float ix, float iy) { x = ix; y = iy; }
//	Position() {
//		x = 0;
//		y = 0;
//	}
//	float x;
//	float y;
//	float Length() {
//		return hypotf(x, y);
//	}
//	Position normalized() {
//		return Position(x / Length(), y / Length());
//	}
//	Position operator+(const Position& in) {
//		return Position(x + in.x, y + in.y);
//	}
//
//	Position operator-(const Position& in) {
//		return Position(x - in.x, y - in.y);
//	}
//
//	Position operator*(float s) {
//		return Position(x*s, y*s);
//	}
//
//};
//
//Position _pos;
//Position _endPoint;
//float _g;
//float _v;
//float _length;
//
//
//typedef Position Vec2;
//
//inline float Dot(const Vec2& a, const Vec2& b) {
//	return a.x*b.x + a.y*b.y;
//}
//
//inline float Cross(const Vec2& a, const Vec2& b) {
//	return a.x*b.y - b.x*a.y;
//}


// ==========WinMain�֐�
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SystmInit();

	//----------�ް�ٰ��
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen();	// ��ʏ���

		KeyCheck();

		switch (gameMode)
		{
		case GMODE_INIT:	// ������
			GameInit();
			gameMode = GMODE_TITLE;
			break;
		case GMODE_TITLE:	// ����
			if (fadeIn) {
				if (!FadeInScreen(5))fadeIn = false;
			}
			else if (fadeOut) {
				if (!FadeOutScreen(5)) {
					//gameMode = GMODE_GAME;
					gameMode = GMODE_CHARASERE;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else if (trgKey[START]) fadeOut = true;

			GameTitle();
			break;
			// ��׾ڸ�
		case GMODE_CHARASERE:
			if (fadeIn) {
				if (!FadeInScreen(5))fadeIn = false;
			}
			else if (fadeOut) {
				if (!FadeOutScreen(5)) {

					gameMode = GMODE_GAME;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else if (trgKey[START]) fadeOut = true;

			GameCharasere();
			break;
		case GMODE_GAME:	// �ްђ�
			if (fadeIn) {
				if (!FadeInScreen(5))fadeIn = false;
			}
			else if (trgKey[START]) gameMode = GMODE_OVER;
			GameMain();
			break;
		case GMODE_OVER:	// �ްѵ��ް
			if (fadeIn) {
				if (!FadeInScreen(5))fadeIn = false;
			}
			else if (fadeOut) {
				if (!FadeOutScreen(5)) {
					gameMode = GMODE_INIT;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else if (trgKey[START]) fadeOut = true;
			GameOver();
			break;
		default:
			break;
		}

		// Ҳݏ���
		ScreenFlip();	//����ʂ�\��ʂɏu�Ժ�߰
	}
	DxLib_End();	//DXײ���؂̏I������
	return 0;		//������۸��т̏I��
}

int SystmInit(void)
{
	// ----------���я���
	SetWindowText("�Q�[���^�C�g��");	//	�ްі�
	// ���я���
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) return false;
	SetDrawScreen(DX_SCREEN_BACK);

	KeyCheckInit();

	gameCnt = 0;
	fadeCnt = 0;
	//----------�O���t�B�b�N�̓o�^----------
	PlayerSystmInit();
	ShotSystmInit();
	StageSystmInit();

	selectImage1 = LoadGraph("image/1player.png");
	selectImage2 = LoadGraph("image/4player.png");
	titleImage = LoadGraph("image/title2.png");
	

	////�Ђ��̎x�_���`����
	//_endPoint.x = 320;
	//_endPoint.y = 0;
	//_v = 0;
	//

	//_g = 1.f;//�d�͂̒�`
	//_length = 320;//�R�̒����̌v�Z

	return 1;

}

void GameInit(void)
{
	fadeIn = true;
	fadeOut = false;
	pauseFlag = 0;
	gameMode = GMODE_TITLE;
	PlayerGameInit();
	ShotGameInit();
	StageGameInit();

}

void GameTitle(void)
{


	GameTitleDraw();
}

void GameTitleDraw(void)
{
	// �ł�����
	DrawGraph(0, 0, titleImage, false);
	DrawGraph(100,0,selectImage1, true);
	DrawGraph(0, 100, selectImage2, true);


	//Vec2 v = (_pos - _endPoint);//�U��q�̎x�_����U��q�̐��܂ł̃x�N�g��
	//v = v.normalized();//���K������
	////�O�ςƓ��ς𗘗p���Ċp�x���v�Z
	//float cost = Dot(v, Vec2(-1, 0));
	//float sint = Cross(v, Vec2(-1, 0));
	//float theta = atan2f(cost, sint);

	//// �����x�����x�����ꂼ��̃x�N�g����


	//_v += _g * cost;
	////�q���g�͂����܂ŁB
	////���Ƃ͐U��q�̊p�x�ɏ]���āA���̎��X�̉����x�����߁A
	////���x(_v)�ɉ��Z���悤
	////�����X�����AY�����ɕ�����
	////OnMove�̑�3��4�����ɑ�����Ă�������

	//OnMove(_pos.x, _pos.y, _v * sint, _v * cost);//��3�����A��4������������Ɛݒ肵�悤

	////�␳����
	//OnAdjust();			// �����̕␳�������Ȃ��ƁA�Ђ����L�тĂ������璍��!!

	//DrawLine(_pos.x + 1, _pos.y, 320, 0, 0x808080, 1);//�Ђ��`��
	//DrawLine(_pos.x, _pos.y, 320, 0, 0xffffffff, 2);//�Ђ��`��
	//DrawCircle(_pos.x + 1, _pos.y + 1, 20, 0x000000);//������`��
	//DrawCircle(_pos.x, _pos.y, 20, 0x008000);//������`��




}

//void OnMove(float& x, float& y, float vx, float vy) {
//	//�@�����Ɉړ��̂��߂̏�����`���Ă��������B
//	x += vx;
//	y += vy;
//}
//
//void OnAdjust() {
//	Vec2 v = (_pos - _endPoint);
//	if (v.Length() > _length) {
//		_pos = _endPoint + v.normalized()*_length;
//	}
//}

void GameCharasere(void)
{
	GameCharasereDraw();
}

void GameCharasereDraw(void)
{
	DrawString(0, 0, "Charasere", 0xffffff);
}

void GameMain(void)
{

	if (trgKey[P1_PAUSE]) {
		pauseFlag = !pauseFlag;
	}
	if (pauseFlag) {
		SetDrawBright(128, 128, 128);
	}
	else {
		gameCnt++;
		PlayerControl();
		ShotControl();
		StageControl();

		HitCheck();
	}

	GameMainDraw();

	if (pauseFlag) {
		SetDrawBright(255, 255, 255);
		DrawString(360, 292, "P A U S E", 0xffffff);
	}

}

void GameMainDraw(void)
{
	StageDraw();
	PlayerDraw();
	ShotDraw();
	WireDraw();

	DrawFormatString(0, 0, 0xffffff, "GameMain : %d", gameCnt);

	

}

void GameOver(void)
{

	GameOverDraw();
}

void GameOverDraw(void)
{
	DrawString(0, 0, "GameOver", 0xffffff);
}

// ̪��޲݁F���邭�Ȃ�
bool FadeInScreen(int fadeStep)
{
	fadeCnt += fadeStep;
	if (fadeCnt <= 255)
	{
		SetDrawBright(fadeCnt, fadeCnt, fadeCnt);
		return true;
	}
	else
	{
		SetDrawBright(255, 255, 255);
		fadeCnt = 0;
		return false;
	}
}

// ̪��ޱ�āF�Â��Ȃ�
bool FadeOutScreen(int fadeStep)
{
	fadeCnt += fadeStep;
	if (fadeCnt <= 255)
	{
		SetDrawBright(255 - fadeCnt, 255 - fadeCnt, 255 - fadeCnt);
		return true;
	}
	else
	{
		SetDrawBright(0, 0, 0);
		fadeCnt = 0;
		return false;
	}
}

void HitCheck(void)
{

}