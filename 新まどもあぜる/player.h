#pragma once

void PlayerSystmInit(void);
void PlayerGameInit(void);
void PlayerControl(void);
void PlayerDraw(void);

//void PlayerState(void);

enum PLAYER_STATE {	
	PLAYER_NORMAL,		// ���E�ړ� 

	PLAYER_JUMP_UP,		// �W�����v�̏㏸
	PLAYER_JUMP_DOWN,	// �W�����v���~
	PLAYER_W_ACTION,	// ܲ԰�����
	PLAYER_W_JUMP,		// ܲ԰�ެ���
};

//bool ItemHit(XY pos);

void WireDraw(void);

