#pragma once

enum PLAYER_STATE {
	PLAYER_NORMAL,		// ���E�ړ� 
	PLAYER_DOWN,	    // �W�����v���~
	PLAYER_JUMP_UP,		// �W�����v�̏㏸
	PLAYER_Y_ACTION,	// ܲ԰�����
	PLAYER_Y_JUMP,		// ܲ԰�ެ���
};

void PlJumpUp(void);
void PlDown(void);
void PlNormal(void);
void PlWireAction(void);
void PlWireJump(void);

void PlayerSystmInit(void);
void PlayerGameInit(void);
void PlayerControl(void);
void PlayerDraw(void);

void PlayerState(void);


// ���C���[�A�N�V�����ɂ���
void WireDraw(void);
//�W�����v�L�[�����ꂽ�C�x���g
//void OnPushJumpKey(float& vx, float& vy);
//���n�C�x���g
//void OnGround(float& x, float& y, float& vx, float& vy);

bool GetPlayerV(void);	// ��ڲ԰�����m�F

// �p�x(���W�A��)�����߂邽�߂̂��
float getRadian(float old_x, float old_y, float now_x, float now_y);


// ���W�A����sin��cos�ɕ���������
//float Disassembly(float radian_cos , float radian_sin);
float Disassembly_C(float radian_cos);					// x��
float Disassembly_S(float& radian_sin , float g);		// y��



