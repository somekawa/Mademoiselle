#pragma once
#include <math.h>				// 標準ﾍｯﾀﾞｰﾌｧｲﾙは<>で指定する

// 定数
#define PLAY_SIZE_X 2400	// プレイ画面全体のサイズX
#define PLAY_SIZE_Y 1600	// プレイ画面全体のサイズY

#define SCREEN_SIZE_X 1200
#define SCREEN_SIZE_Y 800

#define CHIP_SIZE_X 32
#define CHIP_SIZE_Y 32

#define PI 3.141592f
#define ACC_G 9.8f

#define PLAYER_MAX 4

struct Position {
	Position(float ix, float iy) { x = ix; y = iy; }
	Position()
	{
		x = 0;
		y = 0;
	}
	float x;
	float y;
	float Length()
	{
		return hypotf(x, y);
	}
	Position normalized()
	{
		return Position(x / Length(), y / Length());
	}
	Position operator+(const Position & in) 
	{
		return Position(x + in.x, y + in.y);
	}
	Position operator-(const Position & in) 
	{
		return Position(x - in.x, y - in.y);
	}
	Position operator*(float s) 
	{
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
typedef enum {
	GMODE_INIT,
	GMODE_TITLE,	// ﾀｲﾄﾙ
	GMODE_SETUMEI,	// 説明
	GMODE_CHARASERE, // ｷｬﾗｾﾚ
	GMODE_GAME,		// ゲーム中
	GMODE_RESULT,	// リザルト
	GMODE_MAX
}GAME_MODE;

// 説明
typedef enum{
	DATA_BUTTON,
	DATA_BLOCK,
	DATA_ITEM,
	DATA_ICON,
	DATA_MAX
}DATA;

// 方向
enum MOVE_DIR {
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_MAX
};

// ﾌﾟﾚｲﾔｰｶﾗｰ 選べる種類
enum CHARA_TYPE {
	CHARA_RED,			// 赤
	CHARA_BLUE,		// 青
	CHARA_GREEN,		// 緑
	CHARA_PINK,		// ピンク
	CHARA_ORANGE,	// ｵﾚﾝｼﾞ
	CHARA_MAX
};

enum PLAYER_STATE {
	PLAYER_NORMAL,		// 左右移動 
	PLAYER_DOWN,	    // ジャンプ下降
	PLAYER_JUMP_UP,		// ジャンプの上昇
	PLAYER_W_PRE,		// ﾜｲﾔｰｱｸｼｮﾝの準備(ワイヤーを伸ばす)
	PLAYER_W_ACTION,	// ﾜｲﾔｰｱｸｼｮﾝ
	PLAYER_W_JUMP,		// ﾜｲﾔｰｼﾞｬﾝﾌﾟ
	PLAYER_WALL_RIGHT,	// 壁(右)を走る
	PLAYER_WALL_LEFT,	// 壁(左)を走る
	PLAYER_HASAMU_JUMP,
	PLAYER_DEATH,
};

enum ITEM_STATE {
	ITEM_NON,			// アイテムを取得していない状態(なにもなし)
	ITEM_SEGWEY,		// セグウェイ
	ITEM_KABOSU,		// かぼす(大分県産)
	ITEM_UFO,			// UFO
	ITEM_SPECIAL,		// 必殺技1回プラス
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
	int animCnt;
	int outCnt;
	bool runFlag;
	bool jumpFlag;
	bool jumpFlag2;
	int jumpCnt;			// ｼﾞｬﾝﾌﾟできる回数
	float wallRunSpeed;		//壁にいるときのｽﾋﾟｰﾄﾞ
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
	float nowDeg;		// 今のプレイヤーの角度
	bool dropFlag;			// アイテムを取得しているかどうか
	float JumpDeg;			// 打ち出し角
	bool BlockFlag;			// ブロック内にプレイヤーがいるとき = true
	Position furiko_pos;
	PLAYER_STATE state;
	ITEM_STATE	 item_state;
	int Segwey_Cnt;
	int item[3];			// はてなボックス再出現までの時間
	int WirePreTimeCnt;					// ワイヤーを少しずつ伸ばして途中で途切れるまでの時間
	int WireTimeCnt;					// ワイヤーの表示時間
	float furikoSpeed;
	Position _length;		// 振り子の長さ
	float hpcnt;			// HPバー
	Position flydir;
	MOVE_DIR passingType;	// 通過しないといけない場所
	int passingCnt;		// 通過回数
}CHARACTER;

extern Position mapPos;
GAME_MODE GetGameMode(void);	// 今のｹﾞｰﾑﾓｰﾄﾞ