#pragma once

void PlayerSystmInit(void);
void PlayerGameInit(void);
void PlayerControl(void);
void PlayerDraw(void);

void PlayerState(void);

enum PLAYER_STATE {
	PLAYER_JUMP_UP,		// �W�����v�̏㏸
	PLAYER_JUMP_DOWN,	// �W�����v���~
	PLAYER_NORMAL,		// ���E�ړ� 
	PLAYER_Y_ACTION,	// ܲ԰�����
	PLAYER_Y_JUMP,		// ܲ԰�ެ���
};

//bool ItemHit(XY pos);

void WireDraw(void);

