#pragma once

void PlayerSystmInit(void);
void PlayerGameInit(void);
void PlayerControl(void);
void PlayerDraw(void);

void PlayerState(void);

enum PLAYER_STATE {
	PLAYER_JUMP_UP,		// ジャンプの上昇
	PLAYER_JUMP_DOWN,	// ジャンプ下降
	PLAYER_NORMAL,		// 左右移動 
	PLAYER_Y_ACTION,	// ﾜｲﾔｰｱｸｼｮﾝ
	PLAYER_Y_JUMP,		// ﾜｲﾔｰｼﾞｬﾝﾌﾟ
};

//bool ItemHit(XY pos);

void WireDraw(void);

