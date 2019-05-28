#include"DxLib.h"
#include"keycheck.h"
#include "main.h"

PAD pad[PLAYER_MAX];

void KeyCheckInit(void)
{
	for (int i = 0; i < PAD_TBL_MAX; i++)
	{
		for (int j = 0; j < PLAYER_MAX; j++)
		{
			pad[j].newKey[i] = false;
			pad[j].trgKey[i] = false;
			pad[j].upKey[i] = false;
			pad[j].oldKey[i] = false;
		}
	}
}

void KeyCheck(int padNo)
{
	int Pad= GetJoypadInputState(padList[padNo]);

	// -----newKey �֐��ւ̏��������@
	for (int i = 0; i < PAD_TBL_MAX; i++) 
	{
		pad[padNo].oldKey[i] = pad[padNo].newKey[i]; // �O�ɗ��Ȃ����old�̎擾���ł��Ȃ�

		// �ڂ��Ă���false 
		pad[padNo].newKey[i] = false;	// �S�������Ă��Ȃ����ɂ���
		pad[padNo].trgKey[i] = false;
		pad[padNo].upKey[i] = false;
	}

	if (CheckHitKey(KEY_INPUT_RIGHT) || Pad&(PAD_INPUT_RIGHT))
	{
		pad[padNo].newKey[PAD_TBL_RIGHT] = true;
	}
	if (CheckHitKey(KEY_INPUT_LEFT) || Pad&(PAD_INPUT_LEFT))
	{
		pad[padNo].newKey[PAD_TBL_LEFT] = true;
	}
	if (CheckHitKey(KEY_INPUT_UP) || Pad & (PAD_INPUT_1))// �R���g���[���[A�{�^��
	{
		pad[padNo].newKey[PAD_TBL_JUMP] = true;
	}
	if (CheckHitKey(KEY_INPUT_W) || Pad&(PAD_INPUT_3)) // �R���g���[���[X�{�^��
	{
		pad[padNo].newKey[PAD_TBL_WIRE] = true;
	}

	// ITEM_R
	if (CheckHitKey(KEY_INPUT_RCONTROL) || Pad & (PAD_INPUT_6)) // 
	{
		pad[padNo].newKey[PAD_TBL_ITEM_R] = true;
	}
	// ITEM_L
	if (CheckHitKey(KEY_INPUT_LCONTROL) || Pad & (PAD_INPUT_5)) // 
	{
		pad[padNo].newKey[PAD_TBL_ITEM_L] = true;
	}
	//// SKILL
	//if (CheckHitKey(KEY_INPUT_S) || Pad & (PAD_INPUT_3)) // 
	//{
	//	pad[padNo].newKey[PAD_TBL_SKILL] = true;
	//}

	if (CheckHitKey(KEY_INPUT_SPACE) || Pad&(PAD_INPUT_8))	// �R���g���[���[START�{�^��
	{
		pad[padNo].newKey[PAD_TBL_START] = true;
	}
	if (CheckHitKey(KEY_INPUT_P) || Pad&(PAD_INPUT_7))
	{
		pad[padNo].newKey[PAD_TBL_PAUSE] = true;
	}


	//�@�R���g���[���[����
	/*	PAD_INPUT_1	//A�{�^��
	//	PAD_INPUT_2	//B�{�^��
	//	PAD_INPUT_3	//X�{�^��
	//	PAD_INPUT_4	//Y�{�^��
	//	PAD_INPUT_5	//LB�{�^��
	//	PAD_INPUT_6	//RB�{�^��
	//	PAD_INPUT_7	//Back�{�^��
	//	PAD_INPUT_8	//�X�^�[�g�{�^��*/


	//if (CheckHitKey(KEY_INPUT_UP) || (Pad1 & PAD_INPUT_UP)) newKey[P1_UP] = true;
	//if (CheckHitKey(KEY_INPUT_RIGHT) || (Pad1 & PAD_INPUT_RIGHT)) newKey[P1_RIGHT] = true;
	//if (CheckHitKey(KEY_INPUT_DOWN) || (Pad1 & PAD_INPUT_DOWN)) newKey[P1_DOWN] = true;
	//if (CheckHitKey(KEY_INPUT_LEFT) || (Pad1 & PAD_INPUT_LEFT)) newKey[P1_LEFT] = true;
	//if (CheckHitKey(KEY_INPUT_UP) || (Pad1 & PAD_INPUT_1)) newKey[P1_A] = true;
	//if ((CheckHitKey(KEY_INPUT_RCONTROL || (Pad1 & PAD_INPUT_2)))) newKey[P1_B] = true;
	//if (CheckHitKey(KEY_INPUT_P)) newKey[P1_PAUSE] = true;
	//if (CheckHitKey(KEY_INPUT_W) || (Pad1 & PAD_INPUT_3)) newKey[P2_UP] = true;
	//if (CheckHitKey(KEY_INPUT_D)) newKey[P2_RIGHT] = true;
	//if (CheckHitKey(KEY_INPUT_S)) newKey[P2_DOWN] = true;
	//if (CheckHitKey(KEY_INPUT_A)) newKey[P2_LEFT] = true;
	//if (CheckHitKey(KEY_INPUT_LCONTROL)) newKey[P2_A] = true;
	//if ((CheckHitKey(KEY_INPUT_F)) || (Pad1 & PAD_INPUT_4)) newKey[P2_B] = true;
	//if (CheckHitKey(KEY_INPUT_P)) newKey[P2_PAUSE] = true;
	//if (CheckHitKey(KEY_INPUT_SPACE) || (Pad1 & PAD_INPUT_8)) newKey[PAD_TBL_START] = true;

	// -----trgKey & upKey & oldKey
	for (int i = 0; i < PAD_TBL_MAX; i++) 
	{
			pad[padNo].trgKey[i] = pad[padNo].newKey[i] & ~pad[padNo].oldKey[i];	// trgKey
			pad[padNo].upKey[i] = ~pad[padNo].newKey[i] & pad[padNo].oldKey[i];	// upKey


			/*
			trgKey[i] = false; // ��U�ر
			if((oldKey[i] == false) && (newKey[i] == true)){
			trgKey[i] = true;
			}

			upKey[i] = false; // ��U�ر
			if((oldKey[i] == true) && (newKey[i] == false)){
			upKey[i] = true;
			}
			*/
	}
}
