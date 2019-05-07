#include <math.h>
#include <vector>
#include "Dxlib.h"	//DxLibﾗｲﾌﾞﾗﾘを使用する　独自で準備したﾍｯﾀﾞｰﾌｧｲﾙは""で指定する
#include"keycheck.h"
#include"main.h"
#include"player.h"
#include"shot.h"
#include"stage.h"

#define PI 3.141592

// ----------変数定義----------
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
					//gameMode = GMODE_GAME;
					gameMode = GMODE_CHARASERE;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else if (trgKey[START]) fadeOut = true;

			GameTitle();
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
			else if (trgKey[START]) fadeOut = true;

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
	PlayerSystmInit();
	ShotSystmInit();
	StageSystmInit();

	selectImage1 = LoadGraph("image/1player.png");
	selectImage2 = LoadGraph("image/4player.png");
	titleImage = LoadGraph("image/title2.png");
	

	////ひもの支点を定義する
	//_endPoint.x = 320;
	//_endPoint.y = 0;
	//_v = 0;
	//

	//_g = 1.f;//重力の定義
	//_length = 320;//紐の長さの計算

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
	// でかすぎ
	DrawGraph(0, 0, titleImage, false);
	DrawGraph(100,0,selectImage1, true);
	DrawGraph(0, 100, selectImage2, true);


	//Vec2 v = (_pos - _endPoint);//振り子の支点から振り子の錘までのベクトル
	//v = v.normalized();//正規化する
	////外積と内積を利用して角度を計算
	//float cost = Dot(v, Vec2(-1, 0));
	//float sint = Cross(v, Vec2(-1, 0));
	//float theta = atan2f(cost, sint);

	//// 加速度→速度→それぞれのベクトルへ


	//_v += _g * cost;
	////ヒントはここまで。
	////あとは振り子の角度に従って、その時々の加速度を求め、
	////速度(_v)に加算しよう
	////それをX成分、Y成分に分けて
	////OnMoveの第3第4引数に代入してください

	//OnMove(_pos.x, _pos.y, _v * sint, _v * cost);//第3引数、第4引数をきちんと設定しよう

	////補正処理
	//OnAdjust();			// ここの補正処理がないと、ひもが伸びていくから注意!!

	//DrawLine(_pos.x + 1, _pos.y, 320, 0, 0x808080, 1);//ひも描画
	//DrawLine(_pos.x, _pos.y, 320, 0, 0xffffffff, 2);//ひも描画
	//DrawCircle(_pos.x + 1, _pos.y + 1, 20, 0x000000);//おもり描画
	//DrawCircle(_pos.x, _pos.y, 20, 0x008000);//おもり描画




}

//void OnMove(float& x, float& y, float vx, float vy) {
//	//①ここに移動のための処理を描いてください。
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