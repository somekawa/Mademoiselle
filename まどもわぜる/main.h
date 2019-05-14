#pragma once

// 定数
#define SCREEN_SIZE_X 1200
#define SCREEN_SIZE_Y 800

#define PI 3.141592

#define PI 3.141592
#define ACC_G 9.8




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
	int type;	//ﾌﾟﾚｲﾔｰｶﾗｰ
	bool visible;
	bool visible2;
	int point;
	MOVE_DIR moveDir;
	XY pos;
	XY hitPosS;		//当たり判定用の左上
	XY hitPosE;		//当たり判定用の右下
	XY offsetSize;
	XY size;
	int moveSpeed;
	int life;
	int lifeMax;
	int animCnt;
	int outCnt;
	bool runFlag;
	bool jumpFlag;
	bool jumpFlag2;
	int jumpCnt;	// ｼﾞｬﾝﾌﾟできる回数
	bool shotFlag;
	bool downFlag;
	bool wireFlag;
	bool segweyFlag;	// ｾｸﾞｳｪｲ
	int imgLocCnt;
	XY velocity;
	int cnt;
	float UpDownSpeed;		// 落下速度
	float AddUpDownSpeed;	// 上下の加算量
	DWORD linkCnt;
}CHARACTER;

extern XY mapPos;
GAME_MODE GetGameMode(void);	// 今のｹﾞｰﾑﾓｰﾄﾞ
