#include"DxLib.h"
#include"keycheck.h"



bool newKey[KEY_MAX];
bool trgKey[KEY_MAX];
bool upKey[KEY_MAX];
bool oldKey[KEY_MAX];

void KeyCheckInit()
{
	for (int i = 0; i < KEY_MAX; i++) {
		newKey[i] = false;
		trgKey[i] = false;
		upKey[i] = false;
		oldKey[i] = false;
	}
}

void KeyCheck()
{

	int Pad1;
	Pad1 = GetJoypadInputState(DX_INPUT_PAD1);

	// -----newKey
	for (int i = 0; i < KEY_MAX; i++) {
		newKey[i] = false;	// ‘S•”‰Ÿ‚µ‚Ä‚¢‚È‚¢Ž–‚É‚·‚é
		trgKey[i] = false;
		upKey[i] = false;
	}

	if (CheckHitKey(KEY_INPUT_UP) || (Pad1 & PAD_INPUT_UP)) newKey[P1_UP] = true;
	if (CheckHitKey(KEY_INPUT_RIGHT) || (Pad1 & PAD_INPUT_RIGHT)) newKey[P1_RIGHT] = true;
	if (CheckHitKey(KEY_INPUT_DOWN) || (Pad1 & PAD_INPUT_DOWN)) newKey[P1_DOWN] = true;
	if (CheckHitKey(KEY_INPUT_LEFT) || (Pad1 & PAD_INPUT_LEFT)) newKey[P1_LEFT] = true;
	if (CheckHitKey(KEY_INPUT_UP) || (Pad1 & PAD_INPUT_1)) newKey[P1_A] = true;
	if ((CheckHitKey(KEY_INPUT_RCONTROL || (Pad1 & PAD_INPUT_2)))) newKey[P1_B] = true;
	if (CheckHitKey(KEY_INPUT_P)) newKey[P1_PAUSE] = true;

	if (CheckHitKey(KEY_INPUT_W) || (Pad1 & PAD_INPUT_3)) newKey[P2_UP] = true;
	if (CheckHitKey(KEY_INPUT_D)) newKey[P2_RIGHT] = true;
	if (CheckHitKey(KEY_INPUT_S)) newKey[P2_DOWN] = true;
	if (CheckHitKey(KEY_INPUT_A)) newKey[P2_LEFT] = true;
	if (CheckHitKey(KEY_INPUT_LCONTROL) || (Pad1 & PAD_INPUT_5) || (Pad1 & PAD_INPUT_6)) newKey[P2_A] = true;
	if ((CheckHitKey(KEY_INPUT_F))||(Pad1 & PAD_INPUT_4)) newKey[P2_B] = true;
	if (CheckHitKey(KEY_INPUT_P)) newKey[P2_PAUSE] = true;

	if(CheckHitKey(KEY_INPUT_SPACE) || (Pad1 & PAD_INPUT_8)) newKey[START] = true;

	// -----trgKey & upKey & oldKey
	for (int i = 0; i < KEY_MAX; i++) {
		trgKey[i] = newKey[i] &~oldKey[i];	// trgKey
		upKey[i] = ~newKey[i] & oldKey[i];	// upKey

		oldKey[i]=newKey[i];

		/*
		trgKey[i] = false; // ˆê’U¸Ø±
		if((oldKey[i] == false) && (newKey[i] == true)){
		trgKey[i] = true;
		}

		upKey[i] = false; // ˆê’U¸Ø±
		if((oldKey[i] == true) && (newKey[i] == false)){
		upKey[i] = true;
		}
		*/
	}
}
