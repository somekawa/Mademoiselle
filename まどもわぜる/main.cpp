#include "Dxlib.h"	//DxLibײ���؂��g�p����@�Ǝ��ŏ�������ͯ�ް̧�ق�""�Ŏw�肷��
#include"keycheck.h"
#include"main.h"
#include"player.h"
#include"stage.h"
#include"effect.h"



// ----------�ϐ���`----------


GAME_MODE  gameMode;
int pauseFlag;
int gameCnt;
int fadeCnt;
bool fadeIn;
bool fadeOut;

// ����
int selectImage1;
int selectImage2;
int titleImage;

// ��׸���ڸ�
int charaSeleTitle;
int wakImage;

// ����
int setumei[DATA_MAX];
int dataType;

int nowKey;

Position mapPos;
int setumeiImage;
int cnt;

int SystmInit(void);
void GameInit(void);
void GameTitle(void);
void GameTitleDraw(void);
void GameSetumei(void);
void GameSetumeiDraw(void);
void GameCharasere(void);
void GameCharasereDraw(void);
void GameMain(void);
void GameMainDraw(void);
void GameOver(void);
void GameOverDraw(void);
void HitCheck(void);

bool FadeInScreen(int fadeStep);
bool FadeOutScreen(int fadeStep);

// BGM & SE
int titleBGM;
int sousaBGM;
int charselBGM;
int gameBGM;




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
					gameMode = GMODE_SETUMEI;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else if (trgKey[START]) fadeOut = true;

			GameTitle();
			break;
		// ����
		case GMODE_SETUMEI:
			if (fadeIn) {
				if (!FadeInScreen(5))fadeIn = false;
			}
			else if (fadeOut) {
				if (!FadeOutScreen(5)) {
					if(nowKey == START) gameMode = GMODE_CHARASERE;
					if (nowKey == P1_LEFT) gameMode = GMODE_TITLE;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else {
				if ((trgKey[START]) && (dataType == DATA_MAX - 1)) {
					nowKey = START;
					fadeOut = true;
				}
				else if ((trgKey[P1_LEFT])&&(dataType==0)) {
					nowKey = P1_LEFT;
					fadeOut = true;
				}
			}
			GameSetumei();
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
			else if ((trgKey[START]) && (GetPlayerV())) fadeOut = true;

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
	setumeiImage = LoadGraph("image/data_button_2.png");
	PlayerSystmInit();
	StageSystmInit();
	EffectSystmInit();

	selectImage1 = LoadGraph("image/1player.png");
	selectImage2 = LoadGraph("image/4player.png");
	titleImage = LoadGraph("image/title2.png");

	setumei[DATA_BUTTON] = LoadGraph("image/data_button.png");
	setumei[DATA_BLOCK] = LoadGraph("image/data_block.png");
	setumei[DATA_ICON] = LoadGraph("image/data_icon.png");

	// ��׾ڸ�
	charaSeleTitle = LoadGraph("image/CharacterSelect.png");
	wakImage = LoadGraph("image/wak0.png");
	return 1;

}

void GameInit(void)
{
	titleBGM = LoadSoundMem("BGM/bgm_maoudamashii_8bit11.ogg");		// ��ʐ؂�ւ����ɖ����x�����Ă���BGM���폜���邽��GameInit�ɓ���Ă���
	sousaBGM = LoadSoundMem("BGM/loop001.mp3");
	charselBGM = LoadSoundMem("BGM/loop007.mp3");
	gameBGM = LoadSoundMem("BGM/map_bgm.mp3");

	fadeIn = true;
	fadeOut = false;
	pauseFlag = 0;
	cnt = 0;
	PlayerGameInit();
	StageGameInit();
	EffectGameInit();
	gameMode = GMODE_TITLE;
	dataType = DATA_BUTTON;

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
	DrawLine(0, 0, cnt * 3, cnt * 3, 0xffffff, 3);
	DrawString(0, 0, "GameTitle", 0xffffff);
	// �ł�����
	DrawGraph(0, 0, titleImage, false);
	DrawGraph(100, 0, selectImage1, true);
	DrawGraph(0, 100, selectImage2, true);
	
	PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP, false);


}

// ����
void GameSetumei(void)
{
	/*if (fadeIn) {
		if (!FadeInScreen(5))fadeIn = false;
	}
	else if (fadeOut) {
		if (!FadeOutScreen(5)) {
			if(nowKey == P1_RIGHT) dataType++;
			if (nowKey == P1_LEFT) dataType--;
			fadeOut = false;
			fadeIn = true;
		}
	}
	else  {
		if ((trgKey[P1_RIGHT])&&(dataType < DATA_MAX - 1)) {
			fadeOut = true;
			nowKey = P1_RIGHT;
		}
		else if((trgKey[P1_LEFT])&&(dataType > 0)) {
			fadeOut = true;
			nowKey = P1_LEFT;
		}
	}*/

	if (trgKey[P1_RIGHT]) dataType++;
	if (trgKey[P1_LEFT]) dataType--;

	if (dataType >= DATA_MAX - 1) dataType = DATA_MAX - 1;
	if (dataType <= 0) dataType = 0;

	GameSetumeiDraw();
}

void GameSetumeiDraw(void)
{
	PlaySoundMem(sousaBGM, DX_PLAYTYPE_LOOP, false);
	DeleteSoundMem(titleBGM);
	DrawGraph(0, 0, setumei[dataType], true);
	DrawFormatString(0, 0, 0xff0000, "%d / 4", dataType + 1);
	if (dataType < DATA_MAX - 1) DrawString(1000, 0, "�E : ����", 0xff0000, true);
	DrawString(1000, 20, "�� : �߂�", 0xff0000, true);
	if(dataType==DATA_MAX - 1) DrawString(1000, 0, "�X�y�[�X�L�[ : �����I��", 0xff0000, true);
}

void GameCharasere(void)
{
	DeleteSoundMem(sousaBGM);

	PlayerControl();
	GameCharasereDraw();
}

void GameCharasereDraw(void)
{
	ChangeVolumeSoundMem(255 * 60 / 100, charselBGM);
	PlaySoundMem(charselBGM, DX_PLAYTYPE_LOOP, false);

	DrawBox(150, 20, SCREEN_SIZE_X - 150, 120, 0xffffff, false);
	DrawLine(0, SCREEN_SIZE_Y / 2 + 60, SCREEN_SIZE_X, SCREEN_SIZE_Y / 2 + 60, 0xffffff, true);
	DrawLine(SCREEN_SIZE_X / 2, 120, SCREEN_SIZE_X / 2, SCREEN_SIZE_Y, 0xffffff, true);
	DrawGraph(150, 20, charaSeleTitle, true);
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			DrawGraph((SCREEN_SIZE_X / 2 + 1)*x, 120 + (341 * y), wakImage, true);
		}
	}
	DrawBox(40, 120 + 40, 40 + 260, 160 + 260, 0xffffff, true);
	PlayerDraw();
	DrawString(0, 0, "Charasere", 0xffffff);
}

void GameMain(void)
{
	DeleteSoundMem(charselBGM);

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
		EffectControl();
		HitCheck();
	}
	GameMainDraw();

}

void GameMainDraw(void)
{
	ChangeVolumeSoundMem(255 * 70 / 100, gameBGM);
	PlaySoundMem(gameBGM, DX_PLAYTYPE_LOOP, false);

	StageDraw();
	BgControl();
	PlayerDraw();
	DrawFormatString(0, 0, 0xffffff, "GameMain : %d", gameCnt);
	//DrawLine(0, 0, 0, SCREEN_SIZE_Y, 0xffffff, cnt / 4);
	//DrawLine(0, 0, SCREEN_SIZE_X, 0, 0xffffff, cnt / 6);
	//DrawLine(SCREEN_SIZE_X, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0xffffff, cnt / 4);
	//DrawLine(0, SCREEN_SIZE_Y, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0xffffff, cnt / 6);
	//DrawLine(SCREEN_SIZE_X / 2, 0, SCREEN_SIZE_X / 2, SCREEN_SIZE_Y, 0xffffff, true);
	//DrawLine(0, SCREEN_SIZE_Y / 2, SCREEN_SIZE_X, SCREEN_SIZE_Y / 2, 0xffffff, true);
	EffectDraw();

	if (pauseFlag) {
		SetDrawBright(255, 255, 255);
		DrawString(SCREEN_SIZE_X / 2 - 40, 100, "P A U S E", 0xffffff);
		DrawRotaGraph(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2, 0.6 , 0, setumeiImage, true,false);
	}
}

void GameOver(void)
{
	DeleteSoundMem(gameBGM);

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

// ���̹ް�Ӱ��
GAME_MODE GetGameMode(void)
{
	return gameMode;
}