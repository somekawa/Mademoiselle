#include "Dxlib.h"	//DxLibﾗｲﾌﾞﾗﾘを使用する　独自で準備したﾍｯﾀﾞｰﾌｧｲﾙは""で指定する
#include"keycheck.h"
#include"main.h"
#include"player.h"
#include"stage.h"
#include"effect.h"
#include"camera.h"
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
int data[DATA_MAX];
int dataType;

int nowKey;

Position mapPos;
int setumeiImage;
int cnt;
int flameCnt;

int readyImage;
int goImage;

// リザルト
int resultImage;
int continueImage;

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
void GameResult(void);
void GameResultDraw(void);
void HitCheck(void);

bool FadeInScreen(int fadeStep);
bool FadeOutScreen(int fadeStep);

// BGM & SE
int titleBGM;
int sousaBGM;
int charselBGM;
int gameBGM;
int title_se;
int start_se;
int setumei_se;
int CountDown_se;

// ==========WinMain関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SystmInit();

	//----------ｹﾞｰﾑﾙｰﾌﾟ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen();	// 画面消去

		for (int j = 0; j < PLAYER_MAX; j++)
		{
			KeyCheck(j);
		}

		switch (gameMode)
		{
		case GMODE_INIT:	// 初期化
			GameInit();
			gameMode = GMODE_TITLE;
			break;

		case GMODE_TITLE:	// ﾀｲﾄﾙ
			if (fadeIn)
			{
				if (!FadeInScreen(5))
				{
					fadeIn = false;
				}
			}
			else if (fadeOut)
			{
				if (!FadeOutScreen(5))
				{
					gameMode = GMODE_SETUMEI;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else if (pad[0].trgKey[PAD_TBL_START])
			{
				PlaySoundMem(title_se, DX_PLAYTYPE_BACK, false);
				fadeOut = true;
			}
			//else
			//{
			//}
			// []内の数字で使えるコントローラーが変わる　０はPL1　１はPL2
			GameTitle();
			break;

			// 説明
		case GMODE_SETUMEI:
			if (fadeIn)
			{
				if (!FadeInScreen(5))fadeIn = false;
			}
			else if (fadeOut)
			{
				if (!FadeOutScreen(5))
				{
					if (nowKey == PAD_TBL_JUMP)
					{
						gameMode = GMODE_CHARASERE;
					}
					if (nowKey == PAD_TBL_BACK)
					{
						gameMode = GMODE_TITLE;
					}
					fadeOut = false;
					fadeIn = true;
				}
			}
			else
			{
				if ((pad[0].trgKey[PAD_TBL_JUMP]) && (dataType == DATA_MAX - 1))
				{
					nowKey = PAD_TBL_JUMP;
					fadeOut = true;
				}
				else if ((pad[0].trgKey[PAD_TBL_BACK]) && (dataType == 0))
				{
					nowKey = PAD_TBL_BACK;
					fadeOut = true;
				}
			}
			GameSetumei();
			break;

			// ｷｬﾗｾﾚｸﾄ
		case GMODE_CHARASERE:
			if (fadeIn)
			{
				if (!FadeInScreen(5))fadeIn = false;
			}
			else if (fadeOut)
			{
				if (!FadeOutScreen(5))
				{
					gameMode = GMODE_GAME;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else if ((pad[0].trgKey[PAD_TBL_START]) && (GetPlayerV()))
			{
				PlaySoundMem(start_se, DX_PLAYTYPE_BACK, true);
				fadeOut = true;
			}
			GameCharasere();
			break;

		case GMODE_GAME:	// ｹﾞｰﾑ中
			if (fadeIn)
			{
				if (!FadeInScreen(5))
				{
					PlaySoundMem(CountDown_se, DX_PLAYTYPE_BACK, false);

					fadeIn = false;
				}
			}
			else if (pad[0].trgKey[PAD_TBL_START])
			{
				gameMode = GMODE_RESULT;
			}
			GameMain();
			break;

		case GMODE_RESULT:	// ｹﾞｰﾑｵｰﾊﾞｰ
			if (fadeIn)
			{
				if (!FadeInScreen(5))
				{
					fadeIn = false;
				}
			}
			else if (fadeOut)
			{
				if (!FadeOutScreen(5))
				{
					gameMode = GMODE_INIT;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else if (pad[0].trgKey[PAD_TBL_START])
			{
				fadeOut = true;
			}
			GameResult();
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

	selectImage1 = LoadGraph("image/title_1.png");
	selectImage2 = LoadGraph("image/title_2.png");
	titleImage = LoadGraph("image/title2.png");

	readyImage = LoadGraph("image/ready.png");
	goImage = LoadGraph("image/go.png");

	data[DATA_BUTTON] = LoadGraph("image/data_button.png");
	data[DATA_BLOCK] = LoadGraph("image/data_block.png");
	data[DATA_ITEM] = LoadGraph("image/data_item.png");
	data[DATA_ICON] = LoadGraph("image/data_icon.png");

	// ｷｬﾗｾﾚｸﾄ
	charaSeleTitle = LoadGraph("image/CharacterSelect.png");
	wakImage = LoadGraph("image/wak0.png");

	title_se = LoadSoundMem("BGM/title_se.mp3");
	start_se = LoadSoundMem("BGM/gamestart_se.mp3");
	setumei_se = LoadSoundMem("BGM/setumei_se.mp3");
	CountDown_se = LoadSoundMem("BGM/Countdown.mp3");

	// ﾘｻﾞﾙﾄ
	resultImage = LoadGraph("image/nback3.png");
	continueImage = LoadGraph("image/continue.png");

	return 1;

}

void GameInit(void)
{
	titleBGM = LoadSoundMem("BGM/bgm_maoudamashii_8bit11.ogg");		// 画面切り替え時に毎回一度流していたBGMを削除するためGameInitに入れている
	sousaBGM = LoadSoundMem("BGM/loop001.ogg");
	charselBGM = LoadSoundMem("BGM/loop007.ogg");
	gameBGM = LoadSoundMem("BGM/map_bgm.mp3");

	fadeIn = true;
	fadeOut = false;
	pauseFlag = 0;
	cnt = 0;
	flameCnt = 0;
	PlayerGameInit();
	StageGameInit();
	EffectGameInit();
	CameraGameInit();
	gameMode = GMODE_TITLE;
	dataType = DATA_BUTTON;
}

void GameTitle(void)
{
	if (cnt > 2500)
	{
		cnt = 2500;
	}
	else
	{
		cnt++;
	}
	GameTitleDraw();
}

void GameTitleDraw(void)
{
	StopSoundMem(sousaBGM);

	DrawLine(0, 0, 0, SCREEN_SIZE_Y, 0xffffff, cnt / 4);
	DrawLine(0, 0, SCREEN_SIZE_X, 0, 0xffffff, cnt / 6);
	DrawLine(SCREEN_SIZE_X, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0xffffff, cnt / 4);
	DrawLine(0, SCREEN_SIZE_Y, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0xffffff, cnt / 6);
	DrawLine(0, 0, cnt * 3, cnt * 3, 0xffffff, 3);
	DrawString(0, 0, "GameTitle", 0xffffff);
	// でかすぎ
	DrawGraph(0, 0, titleImage, false);
	DrawGraph(SCREEN_SIZE_X / 5, 0, selectImage1, true);
	DrawGraph(100, 150, selectImage2, true);

	ChangeVolumeSoundMem(255 * 80 / 100, charselBGM);
	PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP, false);
}

// 説明
void GameSetumei(void)
{
	if (pad[0].trgKey[PAD_TBL_JUMP])
	{
		PlaySoundMem(setumei_se, DX_PLAYTYPE_BACK, true);
		dataType++;
	}
	if (pad[0].trgKey[PAD_TBL_BACK])
	{
		PlaySoundMem(setumei_se, DX_PLAYTYPE_BACK, true);
		dataType--;
	}
	if (dataType >= DATA_MAX - 1)
	{
		dataType = DATA_MAX - 1;
	}
	if (dataType <= 0)
	{
		dataType = 0;
	}
	GameSetumeiDraw();
}

void GameSetumeiDraw(void)
{
	PlaySoundMem(sousaBGM, DX_PLAYTYPE_LOOP, false);
	StopSoundMem(titleBGM);
	DrawGraph(0, 0, data[dataType], true);
	DrawFormatString(0, 0, 0xff0000, "%d / 4", dataType + 1);
	/*if (dataType < DATA_MAX - 1)
	{
		DrawString(1000, 0, "右 : 次へ", 0xff0000, true);
	}
	if (dataType > 0)
	{
		DrawString(1000, 20, "左 : 戻る", 0xff0000, true);
	}
	if (dataType == DATA_MAX - 1)
	{
		DrawString(1000, 0, "スペースキー : 説明終了", 0xff0000, true);
	}
	if (dataType == 0)
	{
		DrawString(1000, 20, "左 : タイトルに戻る", 0xff0000, true);
	}*/
}

void GameCharasere(void)
{
	DeleteSoundMem(titleBGM);
	DeleteSoundMem(sousaBGM);
	for (int j = 0; j < PLAYER_MAX; j++)
	{
		PlayerControl(j);
	}
	GameCharasereDraw();
}

void GameCharasereDraw(void)
{
	ChangeVolumeSoundMem(255 * 60 / 100, charselBGM);
	PlaySoundMem(charselBGM, DX_PLAYTYPE_LOOP, false);

	DrawGraph(0, 0, resultImage, true);

	//DrawBox(150, 20, SCREEN_SIZE_X - 150, 120, 0xffffff, false);
	//DrawLine(0, SCREEN_SIZE_Y / 2 + 60, SCREEN_SIZE_X, SCREEN_SIZE_Y / 2 + 60, 0xffffff, true);
	//DrawLine(SCREEN_SIZE_X / 2, 120, SCREEN_SIZE_X / 2, SCREEN_SIZE_Y, 0xffffff, true);
	DrawGraph(150, 20, charaSeleTitle, true);
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			DrawGraph((SCREEN_SIZE_X / 2 + 1)*x, 120 + (341 * y), wakImage, true);
		}
	}
	//DrawBox(40, 120 + 40, 40 + 260, 160 + 260, 0xffffff, true);
	for (int j = 0; j < PLAYER_MAX; j++)
	{
		PlayerDraw(j);
	}
	DrawString(0, 0, "Charasere", 0xffffff);
}

void GameMain(void)
{
	DeleteSoundMem(charselBGM);

	GameMainDraw();

	if (flameCnt > 270)
	{
		ChangeVolumeSoundMem(255 * 70 / 100, gameBGM);
		PlaySoundMem(gameBGM, DX_PLAYTYPE_LOOP, false);
		if (pad[0].trgKey[PAD_TBL_PAUSE])
		{
			pauseFlag = !pauseFlag;
		}
		if (pauseFlag)
		{
			SetDrawBright(128, 128, 128);
		}
		else
		{
			gameCnt++;
			for (int j = 0; j < PLAYER_MAX; j++)
			{
				PlayerControl(j);
			}
			StageControl();
			EffectControl();
			HitCheck();
		}

	}


	/*if (cnt > 2500) {
		cnt = 2500;
	}
	else {
		cnt++;
	}*/


}

void GameMainDraw(void)
{

	StageDraw();
	BgControl();
	for (int j = 0; j < PLAYER_MAX; j++)
	{
		PlayerDraw(j);
	}
	//DrawGraph(CHIP_SIZE_X * 5 - mapPos.x, CHIP_SIZE_Y * 23 - mapPos.y, maiImage, true);
	DrawFormatString(0, 0, 0xffffff, "GameMain : %d", gameCnt);
	//DrawLine(0, 0, 0, SCREEN_SIZE_Y, 0xffffff, cnt / 4);
	//DrawLine(0, 0, SCREEN_SIZE_X, 0, 0xffffff, cnt / 6);
	//DrawLine(SCREEN_SIZE_X, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0xffffff, cnt / 4);
	//DrawLine(0, SCREEN_SIZE_Y, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0xffffff, cnt / 6);
	//DrawLine(SCREEN_SIZE_X / 2, 0, SCREEN_SIZE_X / 2, SCREEN_SIZE_Y, 0xffffff, true);
	//DrawLine(0, SCREEN_SIZE_Y / 2, SCREEN_SIZE_X, SCREEN_SIZE_Y / 2, 0xffffff, true);
	EffectDraw();

	if (pauseFlag)
	{
		SetDrawBright(255, 255, 255);
		DrawString(SCREEN_SIZE_X / 2 - 40, 100, "P A U S E", 0xffffff);
		DrawRotaGraph(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2, 0.6, 0, setumeiImage, true, false);
	}

	if (flameCnt <= 230)
	{
		// READY...
		DrawGraph(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2, readyImage, true);
		flameCnt++;
	}
	else if (flameCnt <= 270)
	{
		// GO !!
		DrawGraph(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2, goImage, true);
		flameCnt++;
	}
}

void GameResult(void)
{
	DeleteSoundMem(gameBGM);

	GameResultDraw();
}

void GameResultDraw(void)
{
	DrawGraph(0, 0, resultImage, true);
	DrawGraph(SCREEN_SIZE_X / 3, SCREEN_SIZE_Y / 2, continueImage, true);
	DrawString(0, 0, "GameResult", 0xffffff);
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