#pragma once
#include "main.h"

//enum KEY_MODE {
//	P1_UP,
//	P1_RIGHT,
//	P1_DOWN,
//	P1_LEFT,
//	P1_A,
//	P1_B,
//	P1_PAUSE,
//	P2_UP,
//	P2_RIGHT,
//	P2_DOWN,
//	P2_LEFT,
//	P2_A,
//	P2_B,
//	P2_PAUSE,
//	START,
//	KEY_MAX
//};

enum PAD_TBL {
	PAD_TBL_RIGHT,		// �E�ړ�
	PAD_TBL_LEFT,		// ���ړ�
	PAD_TBL_JUMP,		// �W�����v
	PAD_TBL_WIRE,		// ���C���[���o���A�؂�
	PAD_TBL_ITEM,	// �A�C�e���g�p
	//PAD_TBL_SKILL,	// �K�E�Z�H
	PAD_TBL_START,		// �X�^�[�g
	PAD_TBL_PAUSE,
	PAD_TBL_MAX
};

const int padList[PLAYER_MAX] = {
	{DX_INPUT_PAD1},
	{DX_INPUT_PAD2},
	//{DX_INPUT_PAD3},
	//{DX_INPUT_PAD4},
};

struct PAD{
	bool newKey[PAD_TBL_MAX];
	bool trgKey[PAD_TBL_MAX];
	bool upKey[PAD_TBL_MAX];
	bool oldKey[PAD_TBL_MAX];
};
// ��۰��ٕϐ�
extern PAD pad[PLAYER_MAX];


// �������ߐ錾
void KeyCheckInit(void);
void KeyCheck(int padNo);

