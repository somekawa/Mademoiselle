#pragma once
#include "main.h"

//enum KEY_MODE {
//	P1_UP,
//	P1_RIGHT,
//	P1_DOWN,
//	P1_LEFT,
//	P1_A,
//	P1_B,
//	P1_PAUSE,
//	P2_UP,
//	P2_RIGHT,
//	P2_DOWN,
//	P2_LEFT,
//	P2_A,
//	P2_B,
//	P2_PAUSE,
//	START,
//	KEY_MAX
//};

enum PAD_TBL {
	PAD_TBL_RIGHT,		// 右移動
	PAD_TBL_LEFT,		// 左移動
	PAD_TBL_JUMP,		// ジャンプ
	PAD_TBL_WIRE,		// ワイヤーを出す、切る
	PAD_TBL_ITEM,	// アイテム使用
	//PAD_TBL_SKILL,	// 必殺技？
	PAD_TBL_START,		// スタート
	PAD_TBL_PAUSE,
	PAD_TBL_MAX
};

const int padList[PLAYER_MAX] = {
	{DX_INPUT_PAD1},
	{DX_INPUT_PAD2},
	//{DX_INPUT_PAD3},
	//{DX_INPUT_PAD4},
};

struct PAD{
	bool newKey[PAD_TBL_MAX];
	bool trgKey[PAD_TBL_MAX];
	bool upKey[PAD_TBL_MAX];
	bool oldKey[PAD_TBL_MAX];
};
// ｸﾞﾛｰﾊﾞﾙ変数
extern PAD pad[PLAYER_MAX];


// ﾌﾟﾛﾄﾀｲﾌﾟ宣言
void KeyCheckInit(void);
void KeyCheck(int padNo);

