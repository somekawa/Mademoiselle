#pragma once
#include <math.h>				// �W��ͯ�ް̧�ق�<>�Ŏw�肷��

// �萔
#define PLAY_SIZE_X 2400	// �v���C��ʑS�̂̃T�C�YX
#define PLAY_SIZE_Y 1600	// �v���C��ʑS�̂̃T�C�YY

#define SCREEN_SIZE_X 1200
#define SCREEN_SIZE_Y 800

#define CHIP_SIZE_X 32
#define CHIP_SIZE_Y 32

#define PI 3.141592f
#define ACC_G 9.8f

#define PLAYER_MAX 2

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
typedef enum{
	GMODE_INIT,
	GMODE_TITLE,
	GMODE_SETUMEI,	// ����
	GMODE_CHARASERE,
	GMODE_GAME,
	GMODE_OVER,
	GMODE_MAX
}GAME_MODE;

// ����
typedef enum{
	DATA_BUTTON,
	DATA_BLOCK,
	DATA_ITEM,
	DATA_ICON,
	DATA_MAX
};

// ����
enum MOVE_DIR {
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_MAX
};

// ��ڲ԰�װ �I�ׂ���
enum CHARA_TYPE {
	CHARA_RED,			// ��
	CHARA_BLUE,		// ��
	CHARA_GREEN,		// ��
	CHARA_PINK,		// �s���N
	CHARA_ORANGE,	// ��ݼ�
	CHARA_MAX
};

enum PLAYER_STATE {
	PLAYER_NORMAL,		// ���E�ړ� 
	PLAYER_DOWN,	    // �W�����v���~
	PLAYER_JUMP_UP,		// �W�����v�̏㏸
	PLAYER_W_PRE,		// ܲ԰����݂̏���(���C���[��L�΂�)
	PLAYER_W_ACTION,	// ܲ԰�����
	PLAYER_W_JUMP,		// ܲ԰�ެ���
	PLAYER_WALL_RIGHT,	// ��(�E)�𑖂�
	PLAYER_WALL_LEFT,	// ��(��)�𑖂�
};

enum ITEM_STATE {
	ITEM_NON,			// �A�C�e�����擾���Ă��Ȃ����(�Ȃɂ��Ȃ�)
	ITEM_SEGWEY,		// �Z�O�E�F�C
	ITEM_KABOSU,		// ���ڂ�(�啪���Y)
	ITEM_UFO,			// UFO
	ITEM_SPECIAL,		// �K�E�Z1��v���X
};

typedef struct {
	int type;				//��ڲ԰�װ
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
	int jumpCnt;			// �ެ��߂ł����
	float wallRunSpeed;		//�ǂɂ���Ƃ��̽�߰��
	bool wireFlag;			// ���C���[������
	bool wireOkFlag;		// ���C���[���g���Ă����ǂɂԂ������Ƃ�
	bool segweyFlag;		// ��޳��
	int imgLocCnt;
	Position velocity;
	int cnt;
	float UpDownSpeed;		// �������x
	float AddUpDownSpeed;	// �㉺�̉��Z��
	DWORD linkCnt;
	bool right;				// �E����
	bool left;				// ������
	float nowDeg;		// ���̃v���C���[�̊p�x
	bool dropFlag;			// �A�C�e�����擾���Ă��邩�ǂ���
	float JumpDeg;			// �ł��o���p
	bool BlockFlag;			// �u���b�N���Ƀv���C���[������Ƃ� = true
	Position furiko_pos;
	PLAYER_STATE state;
	ITEM_STATE	 item_state;
	int Segwey_Cnt;
}CHARACTER;

extern Position mapPos;
GAME_MODE GetGameMode(void);	// ���̹ް�Ӱ��