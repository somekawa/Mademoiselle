#pragma once
#include <math.h>				// 標準ﾍｯﾀﾞｰﾌｧｲﾙは<>で指定する


// 定数
#define SCREEN_SIZE_X 1200
#define SCREEN_SIZE_Y 800

#define PLAY_SIZE_X 2400	// プレイ画面全体のサイズX
#define PLAY_SIZE_Y 1600	// プレイ画面全体のサイズY


#define PI 3.141592f
#define ACC_G 9.8f


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

// 定義
typedef struct {
	float x;
	float y;
}XY_F;

typedef struct {
	int x;
	int y;
}XY;

// ｹﾞｰﾑﾓｰﾄﾞ
typedef enum
{
	GMODE_INIT,
	GMODE_TITLE,
	GMODE_CHARASERE,
	GMODE_GAME,
	GMODE_OVER,
	GMODE_MAX
}GAME_MODE;

// 方向
enum MOVE_DIR {
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_MAX
};

// ﾌﾟﾚｲﾔｰｶﾗｰ
enum PLAYER_TYPE {
	PLAYER_RED,
	PLAYER_BLUE,
	PLAYER_MAX
};

typedef struct {
	int type;				//ﾌﾟﾚｲﾔｰｶﾗｰ
	bool visible;
	bool visible2;
	int point;
	MOVE_DIR moveDir;
	Position pos;
	Position hitPosS;		//当たり判定用の左上
	Position hitPosE;		//当たり判定用の右下
	Position offsetSize;
	Position size;
	float moveSpeed;
	int life;
	int lifeMax;
	int animCnt;
	int outCnt;
	bool runFlag;
	bool jumpFlag;
	bool jumpFlag2;
	int jumpCnt;			// ｼﾞｬﾝﾌﾟできる回数
	float wallRunSpeed;		//壁にいるときのｽﾋﾟｰﾄﾞ
	bool shotFlag;
	bool downFlag;
	bool wireFlag;			// ワイヤーをだす
	bool wireOkFlag;		// ワイヤーを使っていい壁にぶつかったとき
	bool segweyFlag;		// ｾｸﾞｳｪｲ
	int imgLocCnt;
	Position velocity;
	int cnt;
	float UpDownSpeed;		// 落下速度
	float AddUpDownSpeed;	// 上下の加算量
	DWORD linkCnt;
	bool right;				// 右向き
	bool left;				// 左向き
	float nowDeg;			// 今のプレイヤーの角度
	bool dropFlag;			// アイテムを取得しているかどうか
	float JumpDeg;			// 打ち出し角
	bool BlockFlag;			// ブロック内にプレイヤーがいるとき = true
}CHARACTER;

extern Position mapPos;
GAME_MODE GetGameMode(void);	// 今のｹﾞｰﾑﾓｰﾄﾞ
