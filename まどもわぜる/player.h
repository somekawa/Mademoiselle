#pragma once

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

void PlNormal(void);
void PlDown(void);
void PlJumpUp(void);
void PlWirePrepare(void);
void PlWireAction(void);
void PlWireJump(void);
void PlWall_R(void);	// �ǂ𑖂�(�E)
void PlWall_L(void);	// �ǂ𑖂�(��)


void GetItemRand(void);	// player.dropFlag��true�ɂȂ����Ƃ��ɃA�C�e���������_���Ō��肳����
void ItemSegwey(void);
void ItemKabosu(void);
void ItemUFO(void);
void ItemSP(void);

void PlayerSystmInit(void);
void PlayerGameInit(void);
void PlayerControl(void);
void PlayerDraw(void);

void PlayerState(void);
void ItemState(void);


void AddRad(void);		// �p�x�̉����Z


// ���C���[�A�N�V�����ɂ���
void WireDraw(void);

bool GetPlayerV(void);	// ��ڲ԰�����m�F

// �p�x(���W�A��)�����߂邽�߂̂��
float getRadian(float old_x, float old_y, float now_x, float now_y);


// ���W�A����sin��cos�ɕ���������
//float Disassembly(float radian_cos , float radian_sin);
float Disassembly_C(float radian_cos);					// x��
float Disassembly_S(float& radian_sin , float g);		// y��



