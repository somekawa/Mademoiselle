#pragma once

enum PLAYER_STATE {
	PLAYER_NORMAL,		// 左右移動 
	PLAYER_DOWN,	    // ジャンプ下降
	PLAYER_JUMP_UP,		// ジャンプの上昇
	PLAYER_Y_ACTION,	// ﾜｲﾔｰｱｸｼｮﾝ
	PLAYER_Y_JUMP,		// ﾜｲﾔｰｼﾞｬﾝﾌﾟ
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


// ワイヤーアクションについて
void WireDraw(void);
//ジャンプキー押されたイベント
//void OnPushJumpKey(float& vx, float& vy);
//着地イベント
//void OnGround(float& x, float& y, float& vx, float& vy);

bool GetPlayerV(void);	// ﾌﾟﾚｲﾔｰ生存確認

// 角度(ラジアン)を求めるためのやつ
float getRadian(float old_x, float old_y, float now_x, float now_y);


// ラジアンをsinとcosに分解するやつ
//float Disassembly(float radian_cos , float radian_sin);
float Disassembly_C(float radian_cos);					// x軸
float Disassembly_S(float& radian_sin , float g);		// y軸



