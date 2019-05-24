#include "Dxlib.h"	//DxLibﾗｲﾌﾞﾗﾘを使用する　独自で準備したﾍｯﾀﾞｰﾌｧｲﾙは""で指定する
#include"keycheck.h"
#include"main.h"
#include"player.h"
#include"stage.h"
#include"effect.h"



// ----------変数定義----------


GAME_MODE  gameMode;
int pauseFlag;
int gameCnt;
int fadeCnt;
bool fadeIn;
bool fadeOut;

// ﾀｲﾄﾙ
int selectImage1;
int selectImage2;
int titleImage;

// ｷｬﾗｸﾀｰｾﾚｸﾄ
int charaSeleTitle;
int wakImage;

// 説明
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




// ==========WinMain関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SystmInit();

	//----------ｹﾞｰﾑﾙｰﾌﾟ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen();	// 画面消去

		KeyCheck();

		switch (gameMode)
		{
		case GMODE_INIT:	// 初期化
			GameInit();
			gameMode = GMODE_TITLE;
			break;
		case GMODE_TITLE:	// ﾀｲﾄﾙ
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
		// 説明
		case GMODE_SETUMEI:
			if (fadeIn) {
				if (!FadeInScreen(5))fadeIn = false;
			}
			else if (fadeOut) {
				if (!FadeOutScreen(5)) {
					if(nowKey == P1_A) gameMode = GMODE_CHARASERE;
					if (nowKey == P1_B) gameMode = GMODE_TITLE;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else {
				if ((trgKey[P1_A]) && (dataType >= DATA_MAX - 1)) {
					nowKey = P1_A;
					fadeOut = true;
				}
				else if ((trgKey[P1_B])&&(dataType==0)) {
					nowKey = P1_B;
					fadeOut = true;
				}
			}
			GameSetumei();
			break;
			// ｷｬﾗｾﾚｸﾄ
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
		case GMODE_GAME:	// ｹﾞｰﾑ中
			if (fadeIn) {
				if (!FadeInScreen(5))fadeIn = false;
			}
			else if (trgKey[START]) gameMode = GMODE_OVER;
			GameMain();
			break;
		case GMODE_OVER:	// ｹﾞｰﾑｵｰﾊﾞｰ
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

		// ﾒｲﾝ処理
		ScreenFlip();	//裏画面を表画面に瞬間ｺﾋﾟｰ
	}
	DxLib_End();	//DXﾗｲﾌﾞﾗﾘの終了処理
	return 0;		//このﾌﾟﾛｸﾞﾗﾑの終了
}

int SystmInit(void)
{
	// ----------ｼｽﾃﾑ処理
	SetWindowText("ゲームタイトル");	//	ｹﾞｰﾑ名
	// ｼｽﾃﾑ処理
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) return false;
	SetDrawScreen(DX_SCREEN_BACK);

	KeyCheckInit();

	gameCnt = 0;
	fadeCnt = 0;
	//----------グラフィックの登録----------
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

	// ｷｬﾗｾﾚｸﾄ
	charaSeleTitle = LoadGraph("image/CharacterSelect.png");
	wakImage = LoadGraph("image/wak0.png");
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
	// でかすぎ
	DrawGraph(0, 0, titleImage, false);
	DrawGraph(100, 0, selectImage1, true);
	DrawGraph(0, 100, selectImage2, true);
	


}

// 説明
void GameSetumei(void)
{
	if (fadeIn) {
		if (!FadeInScreen(5))fadeIn = false;
	}
	else if (fadeOut) {
		if (!FadeOutScreen(5)) {
			if(nowKey == P1_A) dataType++;
			if (nowKey == P1_B) dataType--;
			fadeOut = false;
			fadeIn = true;
		}
	}
	else  {
		if ((trgKey[P1_A])&&(dataType < DATA_MAX - 1)) {
			fadeOut = true;
			nowKey = P1_A;
		}
		else if((trgKey[P1_B])&&(dataType > 0)) {
			fadeOut = true;
			nowKey = P1_B;
		}
	}

	GameSetumeiDraw();
}

void GameSetumeiDraw(void)
{
	DrawGraph(0, 0, setumei[dataType], true);
	DrawFormatString(0, 0, 0xff0000, "%d / 4", dataType + 1);
}

void GameCharasere(void)
{
	PlayerControl();
	GameCharasereDraw();
}

void GameCharasereDraw(void)
{
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
	StageDraw();
	BgControl();
	PlayerDraw();
	WireDraw();
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

	GameOverDraw();
}

void GameOverDraw(void)
{
	DrawString(0, 0, "GameOver", 0xffffff);
}

// ﾌｪｰﾄﾞｲﾝ：明るくなる
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

// ﾌｪｰﾄﾞｱｳﾄ：暗くなる
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

// 今のｹﾞｰﾑﾓｰﾄﾞ
GAME_MODE GetGameMode(void)
{
	return gameMode;
}