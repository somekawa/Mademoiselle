#include "Dxlib.h"	//DxLibײ���؂��g�p����@�Ǝ��ŏ�������ͯ�ް̧�ق�""�Ŏw�肷��
#include"keycheck.h"
#include"main.h"
#include"player.h"
#include"stage.h"



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

XY mapPos;
int maiImage;
int cnt;

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

bool FadeInScreen(int fadeStep);
bool FadeOutScreen(int fadeStep);




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
	maiImage = LoadGraph("image/bollI.png");
	PlayerSystmInit();
	StageSystmInit();


	return 1;

}

void GameInit(void)
{
	fadeIn = true;
	fadeOut = false;
	pauseFlag = 0;
	cnt = 0;
	PlayerGameInit();
	StageGameInit();
	gameMode = GMODE_TITLE;

}

void GameTitle(void)
{
	if (cnt > 2500) {
		cnt = 2500;
	}
	else {
		cnt++;
	}

	GameTitleDraw();
}

void GameTitleDraw(void)
{
	DrawLine(0, 0, 0, SCREEN_SIZE_Y, 0xffffff, cnt / 4);
	DrawLine(0, 0, SCREEN_SIZE_X, 0, 0xffffff, cnt / 6);
	DrawLine(SCREEN_SIZE_X, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0xffffff, cnt / 4);
	DrawLine(0, SCREEN_SIZE_Y, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0xffffff, cnt / 6);
	DrawString(0, 0, "GameTitle", 0xffffff);
}

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
	/*if (cnt > 2500) {
		cnt = 2500;
	}
	else {
		cnt++;
	}*/

	if (trgKey[P1_PAUSE]) {
		pauseFlag = !pauseFlag;
	}
	if (pauseFlag) {
		SetDrawBright(128, 128, 128);
	}
	else {
		gameCnt++;
		PlayerControl();
		StageControl();
		HitCheck();
	}
	GameMainDraw();
	if (pauseFlag) {
		SetDrawBright(255, 255, 255);
		DrawString(SCREEN_SIZE_X / 2 - 40, SCREEN_SIZE_Y / 2 - 5, "P A U S E", 0xffffff);
	}
}

void GameMainDraw(void)
{
	StageDraw();
	PlayerDraw();
	WireDraw();
	DrawGraph(CHIP_SIZE_X * 5 - mapPos.x, CHIP_SIZE_Y * 23 - mapPos.y, maiImage, true);
	DrawFormatString(0, 0, 0xffffff, "GameMain : %d", gameCnt);
	//DrawLine(0, 0, 0, SCREEN_SIZE_Y, 0xffffff, cnt / 4);
	//DrawLine(0, 0, SCREEN_SIZE_X, 0, 0xffffff, cnt / 6);
	//DrawLine(SCREEN_SIZE_X, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0xffffff, cnt / 4);
	//DrawLine(0, SCREEN_SIZE_Y, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0xffffff, cnt / 6);
	//DrawLine(SCREEN_SIZE_X / 2, 0, SCREEN_SIZE_X / 2, SCREEN_SIZE_Y, 0xffffff, true);
	//DrawLine(0, SCREEN_SIZE_Y / 2, SCREEN_SIZE_X, SCREEN_SIZE_Y / 2, 0xffffff, true);
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