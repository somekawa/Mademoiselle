#pragma once

enum PLAYER_STATE {
	PLAYER_NORMAL,		// 左右移動 
	PLAYER_DOWN,	    // ジャンプ下降
	PLAYER_JUMP_UP,		// ジャンプの上昇
	PLAYER_W_PRE,		// ﾜｲﾔｰｱｸｼｮﾝの準備(ワイヤーを伸ばす)
	PLAYER_W_ACTION,	// ﾜｲﾔｰｱｸｼｮﾝ
	PLAYER_W_JUMP,		// ﾜｲﾔｰｼﾞｬﾝﾌﾟ
	PLAYER_WALL_RIGHT,	// 壁(右)を走る
	PLAYER_WALL_LEFT,	// 壁(左)を走る
};

enum ITEM_STATE {
	ITEM_NON,			// アイテムを取得していない状態(なにもなし)
	ITEM_SEGWEY,		// セグウェイ
	ITEM_KABOSU,		// かぼす(大分県産)
	ITEM_UFO,			// UFO
	ITEM_SPECIAL,		// 必殺技1回プラス
};

void PlNormal(void);
void PlDown(void);
void PlJumpUp(void);
void PlWirePrepare(void);
void PlWireAction(void);
void PlWireJump(void);
void PlWall_R(void);	// 壁を走る(右)
void PlWall_L(void);	// 壁を走る(左)


void GetItemRand(void);	// player.dropFlagがtrueになったときにアイテムをランダムで決定させる
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


void AddRad(void);		// 角度の加減算


// ワイヤーアクションについて
void WireDraw(void);

bool GetPlayerV(void);	// ﾌﾟﾚｲﾔｰ生存確認

// 角度(ラジアン)を求めるためのやつ
float getRadian(float old_x, float old_y, float now_x, float now_y);


// ラジアンをsinとcosに分解するやつ
//float Disassembly(float radian_cos , float radian_sin);
float Disassembly_C(float radian_cos);					// x軸
float Disassembly_S(float& radian_sin , float g);		// y軸



