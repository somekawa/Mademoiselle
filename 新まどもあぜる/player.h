#pragma once

//enum PLAYER_STATE {
//	PLAYER_NORMAL,		// 左右移動 
//	PLAYER_DOWN,	    // ジャンプ下降
//	PLAYER_JUMP_UP,		// ジャンプの上昇
//	PLAYER_W_PRE,		// ﾜｲﾔｰｱｸｼｮﾝの準備(ワイヤーを伸ばす)
//	PLAYER_W_ACTION,	// ﾜｲﾔｰｱｸｼｮﾝ
//	PLAYER_W_JUMP,		// ﾜｲﾔｰｼﾞｬﾝﾌﾟ
//	PLAYER_WALL_RIGHT,	// 壁(右)を走る
//	PLAYER_WALL_LEFT,	// 壁(左)を走る
//};

//enum ITEM_STATE {
//	ITEM_NON,			// アイテムを取得していない状態(なにもなし)
//	ITEM_SEGWEY,		// セグウェイ
//	ITEM_KABOSU,		// かぼす(大分県産)
//	ITEM_UFO,			// UFO
//	ITEM_SPECIAL,		// 必殺技1回プラス
//};

void PlayerSystmInit(void);
void PlayerGameInit(void);
void PlayerControl(int padNo);
void PlayerDraw(int padNo);

void AddRad(int padNo);

void PlNormal(int padNo);
void PlJumpUp(int padNo);
void PlDown(int padNo);
void PlWirePrepare(int padNo);
void PlWireAction(int padNo);
void PlWireJump(int padNo);
void PlWall_Check(int padNo); // 壁走りできる壁であるかどうか
void PlWall_R(int padNo);	// 壁を走る(右)
void PlWall_L(int padNo);	// 壁を走る(左)
void PlHasamuJump(int padNo);
void Pl_Death(int padNo); // HP0のときに


void UIDraw(int padNo);
void UIDrawSel(int padNo);

// アイテム関連

void GetItemRand(void);	// player.dropFlagがtrueになったときにアイテムをランダムで決定させる
void ItemSegwey(int padNo);
void ItemKabosu(void);
void ItemUFO(void);
void ItemSP(void);

void PlayerState(int padNo);
void ItemState(int padNo);

void Passing(int padNo);
void CameraControl(int padNo);
void ScrollMap(Position pos, int speed, MOVE_DIR dir);
int PlayerTop(int padNo, int passCnt, Position pos, MOVE_DIR dir);

bool GetPlayerV(void);	// ﾌﾟﾚｲﾔｰ生存確認
Position GetPlayerPos(int padNo);
int GetPassingCnt(int padNo);