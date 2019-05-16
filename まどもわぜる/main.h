#pragma once
#include <math.h>				// �W��ͯ�ް̧�ق�<>�Ŏw�肷��


// �萔
#define SCREEN_SIZE_X 1200
#define SCREEN_SIZE_Y 800

#define PI 3.141592

#define PI 3.141592
#define ACC_G 9.8


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

// ��`
typedef struct {
	float x;
	float y;
}XY_F;

typedef struct {
	int x;
	int y;
}XY;

// �ް�Ӱ��
typedef enum
{
	GMODE_INIT,
	GMODE_TITLE,
	GMODE_CHARASERE,
	GMODE_GAME,
	GMODE_OVER,
	GMODE_MAX
}GAME_MODE;

// ����
enum MOVE_DIR {
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_MAX
};

// ��ڲ԰�װ
enum PLAYER_TYPE {
	PLAYER_RED,
	PLAYER_BLUE,
	PLAYER_MAX
};

typedef struct {
	int type;	//��ڲ԰�װ
	bool visible;
	bool visible2;
	int point;
	MOVE_DIR moveDir;
	Position pos;
	Position hitPosS;		//�����蔻��p�̍���
	Position hitPosE;		//�����蔻��p�̉E��
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
	int jumpCnt;	// �ެ��߂ł����
	bool shotFlag;
	bool downFlag;
	bool wireFlag;
	bool segweyFlag;	// ��޳��
	int imgLocCnt;
	Position velocity;
	int cnt;
	float UpDownSpeed;		// �������x
	float AddUpDownSpeed;	// �㉺�̉��Z��
	DWORD linkCnt;
}CHARACTER;

extern Position mapPos;
GAME_MODE GetGameMode(void);	// ���̹ް�Ӱ��
