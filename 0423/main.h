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

// 方向
enum MOVE_DIR {
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_MAX
};

typedef struct {
	int blockType;
	bool visible;
	int point;
	MOVE_DIR moveDir;
	XY pos;
	int hitPos;	//　中心からﾌﾞﾛｯｸまで
	XY offsetSize;
	XY size;
	int moveSpeed;
	int life;
	int lifeMax;
	int animCnt;
	int outCnt;
	bool runFlag;
	bool jumpFlag;
	bool shotFlag;
	bool downFlag;
	int imgLocCnt;
	XY velocity;
	int cnt;
	DWORD linkCnt;
}CHARACTER;

extern XY mapPos;
