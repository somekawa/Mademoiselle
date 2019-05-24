

#include "Dxlib.h"				// DxLibﾗｲﾌﾞﾗﾘを使用する　独自で準備したﾍｯﾀﾞｰﾌｧｲﾙは""で指定する
#include <math.h>				// 標準ﾍｯﾀﾞｰﾌｧｲﾙは<>で指定する
#include"keycheck.h"
#include"main.h"
#include"stage.h"
#include"player.h"
#include"effect.h"

#define INIT_VELOCITY 50		// 初期速度
#define SECOND_PER_FRAME 0.3	// 1ﾌﾚｰﾑの秒数

#define DOWN_SPEED_DEF	5.0f	// デフォルトの落下速度
#define UP_SPEED_DEF   -15.0f	// デフォルトの上昇速度

#define CORRECTION	  100		// ワイヤーの長さ補正用数値(特にy座標)

#define KEEPPOSX_CORRECTION  120	// posの値の補正用数値

#define ONEFRAME_WIRE_UP	  15    // 1フレームで上がるワイヤーの速度

#define FURIKO_SPEED_DEF      10.0f	// 振り子の初速度
#define FURIKO_ADD			  0.4f	// 振り子の加減算速度

//#define PAI 3.141592
//#define JUMPSPEED 2

PLAYER_STATE player_state;
ITEM_STATE	 item_state;


// ﾌﾟﾚｲﾔｰｽﾋﾟｰﾄﾞ
enum PLAYER_SPEED {
	PLAYER_SPEED_NORMAL = 8,
	PLAYER_SPEED_SEGWEY = 12
};
int p1Wak[2];
int p2Wak[2];
int p3Wak[2];
int p4Wak[2]; int yazirusiImage[2];

int playerImage[PLAYER_MAX];
int playerIcon[PLAYER_MAX];
int runImage[PLAYER_MAX][10];
int jumpImage[PLAYER_MAX];
int stopJumpImage[PLAYER_MAX];
int downImage;
int shotImage[PLAYER_MAX];
int segweyImage[PLAYER_MAX][2];
CHARACTER player;

int WirePreTimeCnt;					// ワイヤーを少しずつ伸ばして途中で途切れるまでの時間
int WireTimeCnt;					// ワイヤーの表示時間

bool _isPushJump;
bool _isJumped;

Position furiko_pos;
float _g;
float _v;
Position _length;


float KeepPosX;		// 座標の保存用
float KeepPosY;		// 座標の保存用

float _vx;
float _vy;

float playerX_old;
float playerY_old;
float playerX_now;
float playerY_now;

float radian;

bool skyFlag;

float MaxDeg;
float minDeg;
float furikoSpeed;

float jumpSpeed;

float defDeg;

int Segwey_Cnt;


// アイテム関連 
int itemcnt;
bool itemBoxFlag = true;
int itemBoxPosX = CHIP_SIZE_X * 15 ;
int itemBoxPosY = CHIP_SIZE_Y * 16 ;
int hatenaImage;		// ？ﾎﾞｯｸｽ画像
int segweyImage_icon;

// BGM & SE
int charsel_ok;
int charsel_no;


typedef Position Vec2;

inline float Dot(const Vec2 & a, const Vec2 & b) {
	return a.x* b.x + a.y * b.y;
}

inline float Cross(const Vec2 & a, const Vec2 & b) {
	return a.x* b.y - b.x * a.y;
}




void PlayerSystmInit(void)
{
	p1Wak[0] = LoadGraph("image/p1CSWak.png");
	p1Wak[1] = LoadGraph("image/p1GWak.png");
	p2Wak[0] = LoadGraph("image/p2CSWak.png");
	p2Wak[1] = LoadGraph("image/p2GWak.png");
	p3Wak[0] = LoadGraph("image/p3CSWak.png");
	p3Wak[1] = LoadGraph("image/p3GWak.png");
	p4Wak[0] = LoadGraph("image/p4CSWak.png");
	p4Wak[1] = LoadGraph("image/p4GWak.png"); for (int i = 0; i < 2; i++) {
		yazirusiImage[i] = LoadGraph("image/yazirusi.png");
	}

	// 赤
	playerImage[PLAYER_RED] = LoadGraph("image/playerR_stop.png");
	playerIcon[PLAYER_RED] = LoadGraph("image/R_icon.png");
	jumpImage[PLAYER_RED] = LoadGraph("image/playerR_jump.png");
	stopJumpImage[PLAYER_RED] = LoadGraph("image/playerR_stop_jump.png");
	LoadDivGraph("image/playerR_run.png", 10, 5, 2, 72, 72, runImage[PLAYER_RED], true);
	LoadDivGraph("image/playerR_segway.png", 2, 2, 1, 72, 72, segweyImage[PLAYER_RED], true);

	// 青
	playerImage[PLAYER_BLUE] = LoadGraph("image/playerB_stop.png");
	playerIcon[PLAYER_BLUE] = LoadGraph("image/B_icon.png");
	jumpImage[PLAYER_BLUE] = LoadGraph("image/playerB_jump.png");
	stopJumpImage[PLAYER_BLUE] = LoadGraph("image/playerB_stop_jump.png");
	LoadDivGraph("image/playerB_run.png", 10, 5, 2, 72, 72, runImage[PLAYER_BLUE], true);
	LoadDivGraph("image/playerB_segway.png", 2, 2, 1, 72, 72, segweyImage[PLAYER_BLUE], true);

	downImage = LoadGraph("image/red_down.png");
	shotImage[0] = LoadGraph("image/red_stop_shot.png");
	shotImage[1] = LoadGraph("image/red_down_shot.png");

	hatenaImage = LoadGraph("image/hatena.png");
	segweyImage_icon = LoadGraph("image/segwey.png");

	
}

void PlayerGameInit(void)
{
	// BGM & SE
	charsel_ok = LoadSoundMem("BGM/charok_se.mp3");
	charsel_no = LoadSoundMem("BGM/charno_se.mp3");

	player.type = PLAYER_RED;
	player.size = { 72,72 };
	player.offsetSize = { -player.size.x / 2,-player.size.y };
	player.hitPosE = { 20,0 };
	player.hitPosS = { 20,62 };
	player.pos = { CHIP_SIZE_X * 12, CHIP_SIZE_Y * 14 };
	player.moveSpeed = PLAYER_SPEED_NORMAL;
	player.animCnt = 0;
	player.moveDir = DIR_RIGHT;
	player.runFlag = false;
	player.jumpFlag = false;
	player.jumpCnt = 0;	// ｼﾞｬﾝﾌﾟできる回数
	player.shotFlag = false;
	player.downFlag = false;
	player.segweyFlag = false;	// ｾｸﾞｳｪｲ

	player.wireOkFlag = false;
	player.wireFlag = false;

	player.visible = false;
	player.visible2 = false;
	player.imgLocCnt = 0;

	player.right = false;
	player.left = false;

	player.wallRunSpeed = player.moveSpeed;	// 壁での速度

	//ひもの支点の初期化
	_v = 0;			// 振り子のふり幅
	WireTimeCnt = 0;

	_g = 2.0f;		//重力の定義
	_length = { 0 , 0 };	//紐の長さの計算


	KeepPosX = 0;	// 座標の保存用
	KeepPosY = 0;	// 座標の保存用

	_vx = 0;
	_vy = 0;

	_isPushJump = false;
	_isJumped = false;

	playerX_old = 0;
	playerY_old = 0;

	radian = 0;

	skyFlag = false;

	player_state = PLAYER_NORMAL;

	WirePreTimeCnt = 0;

	//Maxrad = { 0,0 };
	//minrad = { 0,0 };

	MaxDeg = 0.0f;
	minDeg = 0.0f;
	player.nowDeg = 0.0f;
	furikoSpeed = 0.0f;
	jumpSpeed = 0.0f;
	defDeg = 0.0f;

	player.dropFlag = false;

	itemcnt = 0;

	player.JumpDeg = 0;

	player.BlockFlag = false;

	Segwey_Cnt = 0;
}

void PlayerControl(void)
{
	switch (GetGameMode())
	{
	case GMODE_CHARASERE:	// ｷｬﾗｾﾚｸﾄ
		if (!player.visible) {
			if (trgKey[P1_RIGHT]) player.type++;
			if (trgKey[P1_LEFT]) player.type--;

			if (player.type >= PLAYER_MAX) player.type = 0;
			if (player.type <= -1)player.type = PLAYER_MAX - 1;
		}
		else {

			player.animCnt++;
		}
		if (trgKey[P1_A])
		{
			if (!player.visible)
			{
				PlaySoundMem(charsel_ok, DX_PLAYTYPE_BACK, true);
				player.visible = true;
			}
			else
			{
				PlaySoundMem(charsel_no, DX_PLAYTYPE_BACK, true);
				player.visible = false;
			}

		}

		break;
	case GMODE_GAME:
		DeleteSoundMem(charsel_ok);
		DeleteSoundMem(charsel_no);
		PlayerState();
		break;
	}

	// アイテム取得
	if ((player.dropFlag == false) && (itemBoxFlag == true))
	{
		if (player.pos.x - player.hitPosS.x < itemBoxPosX + CHIP_SIZE_X		    // player左 < box右	
			&& itemBoxPosX < player.pos.x + player.hitPosS.x					// box左    < player右
			&& player.pos.y - player.size.y < itemBoxPosY + CHIP_SIZE_Y			// player上 < box下
			&& itemBoxPosY < player.pos.y)										// box上    < player下
		{
			player.dropFlag = true;												// ここをtrueにしているので、アイテムを使ってフラグをfalseにするまで次のアイテムは取得できない
			itemBoxFlag = false;												// ここに入ればhitがでる
		}
	}

	//GetItemRand();

	//
		//case GMODE_GAME:	// ｹﾞｰﾑ中
	//		
	//
	//
	//
	//		player.runFlag = false;
	//		player.jumpFlag = true;
	//		player.downFlag = false;
	//		player.moveSpeed = PLAYER_SPEED_NORMAL;
	//		player.imgLocCnt++;
	//		downPos = 0;
	//
	//		
			//XY movedHitCheck = player.pos;
			//XY movedHitCheck2;
			//XY movedHitCheck3;

			//// ワイヤー処理
			//if (newKey[P2_UP])
			//{

			//	

			//	// 5~10マス以内の頭上にブロックがあったら、ワイヤーが出せるようにしたい
			//	for (int i = 5; i <= 10; )
			//	{
			//		movedHitCheck.y = player.pos.y  - (CHIP_SIZE_Y * i);		// 高さ
			//		if ((WireBlockPass(movedHitCheck)) && (WireBlockPass({ movedHitCheck.x - CHIP_SIZE_X / 2 ,movedHitCheck.y })) &&
			//											(WireBlockPass({ movedHitCheck.x + CHIP_SIZE_X / 2 ,movedHitCheck.y })))		// 一定範囲内にブロックが存在しないことになる
			//		{
			//			player.wireFlag = false;			// 範囲内に存在しないのでfalseが正しい
			//			i++;
			//		}
			//		else
			//		{
			//			// ブロックが存在するときの処理

			//			//紐の長さの計算
			//			_length = player.pos.y + player.offsetSize.y;

			//			// 紐の長さの補正
			//			if (_length >= 250)
			//			{
			//				_length = 250;
			//			}

			//			// KeepPosXの補正
			//			if (_length < KEEPPOSX_CORRECTION)
			//			{
			//				KeepPosX = KEEPPOSX_CORRECTION;
			//			}
			//			else
			//			{
			//				KeepPosX = _length;
			//			}

			//			// KeepPosYを指定ブロックの高さに合わせる
			//			KeepPosY = movedHitCheck.y - CHIP_SIZE_Y / 4 - mapPos.y;
			//			player.wireFlag = true;
			//			player.visible = false;
			//			player.visible2 = true;

			//			TimeCnt = 0;
			//			return;
			//		}
			//		
			//	}
			//	

			//}
	//
	//		// ｾｸﾞｳｪｲ
	//		if (trgKey[P2_A]) player.segweyFlag = !player.segweyFlag;
	//		if (player.segweyFlag) player.moveSpeed = PLAYER_SPEED_SEGWEY;
	//
	//		// 移動
	//		if (player.visible && !player.visible2) {
	//			if (oldKey[P1_RIGHT]) {
	//				player.runFlag = true;
	//				player.moveSpeed = player.moveSpeed;
	//				player.moveDir = DIR_RIGHT;
	//			}
	//			else if (oldKey[P1_LEFT]) {
	//				player.runFlag = true;
	//				player.moveSpeed = -player.moveSpeed;
	//				player.moveDir = DIR_LEFT;
	//			}
	//
	//			// ｼﾞｬﾝﾌﾟ
	//			if (player.jumpFlag) {
	//
	//
	//				movedPos.y -= player.velocity.y * SECOND_PER_FRAME;		// 距離の更新
	//				player.velocity.y -= ACC_G * SECOND_PER_FRAME;			// 速度の更新	定数＊定数なら最初から一緒にする
	//
	//																		// 頭の上
	//				movedHitCheck.y = movedPos.y - player.hitPosS.y;
	//				movedHitCheck2 = movedHitCheck;
	//				movedHitCheck2.x = movedPos.x - player.hitPosS.x;
	//				movedHitCheck3 = movedHitCheck;
	//				movedHitCheck3.x = movedPos.x + player.hitPosE.x - 1;
	//
	//				if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3))) {
	//					player.pos.y = movedPos.y;
	//				}
	//				else {
	//					player.pos.y = MapPos(movedHitCheck, DIR_DOWN).y + player.hitPosE.y;
	//					player.velocity.y *= -1;
	//				}
	//
	//
	//				// 足元にﾌﾞﾛｯｸがあるか
	//				//movedPos = player.pos;
	//				//movedHitCheck.y = movedPos.y + player.offsetSize.y;
	//				//movedHitCheck2 = movedHitCheck;													// 左
	//				//movedHitCheck2.x = movedPos.x - player.hitPosS.x;
	//				//movedHitCheck3 = movedHitCheck;													// 右
	//				//movedHitCheck3.x = movedPos.x + player.hitPosE.x - 1;
	//
	//				// ないとき
	//				//if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3))) {
	//				//	player.pos.y = movedPos.y;
	//				//	if (player.jumpCnt < 2) {
	//
	//				//		player.jumpFlag = false;
	//				//	}
	//				//}
	//
	//				// あるとき
	//				else {
	//					player.pos.y = MapPos(movedHitCheck, DIR_UP).y - player.offsetSize.y;		// 足元から中心
	//					player.velocity.y = 0;
	//					player.jumpCnt = 0;
	//					player.jumpFlag = false;
	//				}
	//
	//			}
	//			if ((!player.jumpFlag) && (trgKey[P1_UP])) {
	//				player.jumpFlag = true;
	//				player.jumpCnt++;
	//				player.velocity.y = INIT_VELOCITY;
	//
	//			}
	//
		//if (player.runFlag) {
		//	movedPos = player.pos;
		//	movedHitCheck = movedPos;
		//	player.animCnt++;
		//	movedPos.x += player.moveSpeed;

		//	switch (player.moveDir)
		//	{
		//	case DIR_RIGHT:
		//		movedHitCheck.x = movedPos.x + player.hitPosE.x;
		//		break;
		//	case DIR_LEFT:
		//		movedHitCheck.x = movedPos.x - player.hitPosS.x - 1;
		//		break;
		//	}

		//	movedHitCheck2 = movedHitCheck;
		//	movedHitCheck2.y = movedPos.y - player.hitPosS.y;
		//	movedHitCheck3 = movedHitCheck;
		//	movedHitCheck3.y = movedPos.y + player.offsetSize.y - 1;
		//	if ((IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3))) {
		//		player.pos = movedPos;
		//	}
		//	else {
		//		// 壁にぴったりくっつける
		//		if (player.moveDir == DIR_RIGHT) {
		//			player.pos.x = MapPos(movedHitCheck, DIR_LEFT).x - player.hitPosS.x;
		//		}
		//		else if (player.moveDir == DIR_LEFT) {
		//			player.pos.x = MapPos(movedHitCheck, DIR_RIGHT).x + player.hitPosE.x;
		//		}
		//	}
		//}
	//
	//			if ((oldKey[P1_RIGHT]) && (player.pos.x > SCREEN_SIZE_X / 2)) {
	//				mapPos.x += player.moveSpeed;
	//			}
	//			if ((oldKey[P1_LEFT]) && (player.pos.x < MAP_X * CHIP_SIZE_X - SCREEN_SIZE_X / 2)) {
	//				mapPos.x += player.moveSpeed;
	//			}
	//		}
	//
	//
	//		if ((!player.jumpFlag) && (!player.runFlag) && (oldKey[P1_DOWN]) ) {
	//			player.downFlag = true;
	//			downPos = 8;
	//		}
	//
	//		// ｼｮｯﾄ
	//		if ((!player.shotFlag) && (oldKey[P1_A])) {
	//			player.imgLocCnt = 0;
	//			player.shotFlag = true;
	//			if (player.moveDir == DIR_RIGHT) {
	//				Shoot({ player.pos.x, player.pos.y - 12 + downPos }, player.moveDir, player.downFlag);
	//			}
	//			else if (player.moveDir == DIR_LEFT) {
	//				Shoot({ player.pos.x,player.pos.y - 12 + downPos }, player.moveDir, player.downFlag);
	//			}
	//		}
	//		else {
	//
	//			if (player.imgLocCnt > 6) {
	//				player.shotFlag = false;
	//			}
	//		}
	//
			// playerを追うカメラ
		//if (player.pos.y > SCREEN_SIZE_Y - CHIP_SIZE_Y * 5) mapPos.y += ACC_G ;
	//if (player.pos.y < CHIP_SIZE_Y * MAP_Y - SCREEN_SIZE_Y + CHIP_SIZE_Y * 5) mapPos.y -= ACC_G * SECOND_PER_FRAME;
	//
	//
	//		//// 左右にじわじわ動く
			/*if (player.pos.x > SCREEN_SIZE_X - CHIP_SIZE_X * 5) mapPos.x += 1;
			if (player.pos.x < CHIP_SIZE_X * MAP_X - SCREEN_SIZE_X + CHIP_SIZE_X * 5) mapPos.x -= SECOND_PER_FRAME;*/
			//	break;
			//}
		//	
		//
}

void PlayerDraw(void)
{
	switch (GetGameMode()) {
	case GMODE_CHARASERE:
		DrawGraph(0, 120, p1Wak[0], true);
		if (player.visible) {
			//DrawRotaGraph(160, 300, 3, 0, runImage[player.type][(player.animCnt / 3) % 10], true);
			//DrawString(120, 180, "キャラ決定！", 0x000000);
			// PL1
			DrawRotaGraph(160, 300, 3, 0,
				runImage[player.type][(player.animCnt / 3) % 10], true);
			DrawString(120, 180, "キャラ決定！", 0x000000);
			// PL2
			DrawRotaGraph(760, 300, 3, 0,
				runImage[player.type][(player.animCnt / 3) % 10], true);
			DrawString(720, 180, "キャラ決定！", 0x000000);

			// PL3
			//DrawRotaGraph(160, 600, 3, 0,
			//	runImage[player.type][(player.animCnt / 3) % 10], true);
			//DrawString(120, 420, "キャラ決定！", 0x000000);
			// PL4
			//DrawRotaGraph(760, 600, 3, 0,
			//	runImage[player.type][(player.animCnt / 3) % 10], true);
			//DrawString(720, 420, "キャラ決定！", 0x000000);		
		}
		else {
			DrawRotaGraph(160, 300, 3, 0, playerImage[player.type], true);
			DrawTurnGraph(0, 240, yazirusiImage[0], true);
			DrawGraph(260, 240, yazirusiImage[1], true);
			// PL1
			DrawRotaGraph(160, 300, 3, 0, playerImage[player.type], true);
			DrawTurnGraph(0, 240, yazirusiImage[0], true);
			DrawGraph(260, 240, yazirusiImage[1], true);
			// PL2
			DrawRotaGraph(760, 300, 3, 0, playerImage[player.type], true);
			DrawTurnGraph(600, 240, yazirusiImage[0], true);
			DrawGraph(860, 240, yazirusiImage[1], true);
			// PL3
			//DrawRotaGraph(160, 600, 3, 0, playerImage[player.type], true);
			//DrawTurnGraph(0, 540, yazirusiImage[0], true);
			//DrawGraph(260, 540, yazirusiImage[1], true);
			// PL4
			//DrawRotaGraph(760, 600, 3, 0, playerImage[player.type], true);
			//DrawTurnGraph(500, 540, yazirusiImage[0], true);
			//DrawGraph(660, 540, yazirusiImage[1], true);

		}

		break;
	case GMODE_GAME:
		if (player.visible && !player.visible2)
		{
			int img = playerImage[player.type];
			if ((player.runFlag) && (!player.jumpFlag)) img = runImage[player.type][(player.animCnt / 3) % 10];
			if ((player.jumpFlag) && (player.runFlag)) img = jumpImage[player.type];
			if ((player.jumpFlag) && (!player.runFlag)) img = stopJumpImage[player.type];
			if (player.segweyFlag) img = segweyImage[player.type][(player.animCnt / 5) % 2];

			if (player_state == PLAYER_WALL_RIGHT) {
				DrawBox(player.pos.x - player.hitPosS.y - mapPos.x, player.pos.y - player.hitPosE.x - mapPos.y,
					player.pos.x - mapPos.x, player.pos.y + player.hitPosS.x - mapPos.y, 0x00ffff, false);
			}
			else if (player_state == PLAYER_WALL_LEFT) {
				DrawBox(player.pos.x - mapPos.x, player.pos.y - player.hitPosE.x - mapPos.y,
					player.pos.x + player.hitPosS.y - mapPos.x, player.pos.y + player.hitPosS.x - mapPos.y, 0x00ffff, false);
			}

			if (player.moveDir == DIR_RIGHT) {
				if (player_state == PLAYER_WALL_RIGHT) {
					DrawRotaGraph(player.pos.x - 36 - mapPos.x, player.pos.y - mapPos.y,
						1, -PI / 2, img, true, false);
				}
				else {
					DrawGraph(player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y, img, true);
				}
			}
			else if (player.moveDir == DIR_LEFT) {
				if (player_state == PLAYER_WALL_LEFT) {
					DrawRotaGraph(player.pos.x + 36 - mapPos.x, player.pos.y - mapPos.y,
						1, PI / 2, img, true, true);
				}
				else {
					DrawTurnGraph(player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y, img, true);
				}
			}

			if ((player_state != PLAYER_WALL_RIGHT) && (player_state != PLAYER_WALL_LEFT)) {
				DrawBox(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y,
					player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y - mapPos.y, 0xff0000, false);


				DrawBox(player.pos.x - player.hitPosS.x - mapPos.x, player.pos.y - player.hitPosS.y - mapPos.y,
					player.pos.x + player.hitPosE.x - mapPos.x, player.pos.y - mapPos.y, 0x00ffff, false);


				DrawLine(player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y - (player.size.y / 2) - mapPos.y,
					player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - (player.size.y / 2) - mapPos.y, 0x00ffff, true);
				DrawLine(player.pos.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y,
					player.pos.x - mapPos.x, player.pos.y - mapPos.y, 0x00ffff, true);
			}
		}

		if (!player.visible && player.visible2)
		{
			if (player.right)
			{
				DrawGraph(player.pos.x  - mapPos.x - 56, player.pos.y - player.size.y - mapPos.y - 23, jumpImage[player.type], true);		   // キャラクタをおもりとして描画
			}
			else
			{
				DrawTurnGraph(player.pos.x  - mapPos.x - 15, player.pos.y - player.size.y  - mapPos.y - 23, jumpImage[player.type], true);	   // キャラクタをおもりとして描画
			}

		}

		if (player_state == PLAYER_W_PRE || player_state == PLAYER_W_ACTION)
		{
			if (player.wireOkFlag)
			{
				DrawLine(player.pos.x - mapPos.x, player.pos.y - player.size.y - mapPos.y, furiko_pos.x - mapPos.x, furiko_pos.y - mapPos.y, 0xffffffff, 2);		// ワイヤー
			}
			else
			{
				if (WirePreTimeCnt <= furiko_pos.y)
				{
					DrawLine(player.pos.x - mapPos.x, player.pos.y - player.size.y - mapPos.y, furiko_pos.x - mapPos.x, furiko_pos.y - mapPos.y, 0xffffffff, 2);	// ワイヤー
					WirePreTimeCnt++;
				}

			}
		}

		// ｱｲｺﾝ
		// PL1
		DrawGraph(50, 40, p1Wak[1], true);
		DrawGraph(58, 74, playerIcon[player.type], true);
		// PL2
		DrawGraph(250, 40, p2Wak[1], true);
		DrawGraph(258, 74, playerIcon[player.type], true);
		// PL3
		DrawGraph(450, 40, p3Wak[1], true);
		//DrawGraph(458, 74, playerIcon[player.type], true);
		// PL4
		DrawGraph(650, 40, p4Wak[1], true);
		//DrawGraph(658, 74, playerIcon[player.type], true);

		// 所持アイテムが描画される枠
		DrawBox(20, 5, 86, 71, 0xffffff, true);
		DrawBox(20, 5, 86, 71, 0x000000, false);

		// ｷｬﾗステータスが描画される枠
		// PL1
		DrawBox(100, 80, 200, 95, 0x000000, true);
		DrawBox(101, 81, 199, 94, 0x00ff00, true);
		// PL2
		DrawBox(300, 80, 400, 95, 0x000000, true);
		DrawBox(301, 81, 399, 94, 0x00ff00, true);
		// PL3
		DrawBox(500, 80, 600, 95, 0x000000, true);
		DrawBox(501, 81, 599, 94, 0x00ff00, true);
		// PL4
		DrawBox(700, 80, 800, 95, 0x000000, true);
		DrawBox(701, 81, 799, 94, 0x00ff00, true);

		

		//itemBoxDraw();
		if (itemBoxFlag == true)
		{
			DrawGraph(CHIP_SIZE_X * 15 - mapPos.x, CHIP_SIZE_X * 16 - mapPos.y, hatenaImage, true);// 左上1つ
			DrawGraph(SCREEN_SIZE_X - CHIP_SIZE_X * 3 - mapPos.x, SCREEN_SIZE_Y * 2 - CHIP_SIZE_X * 3 - mapPos.y, hatenaImage, true);// 下部の真ん中
			DrawGraph(SCREEN_SIZE_X * 2 - CHIP_SIZE_X * 10 - mapPos.x, SCREEN_SIZE_Y - mapPos.y, hatenaImage, true);// 左側真ん中
		}
		else
		{

			if (itemcnt == 200)
			{
				itemBoxFlag = true;
				itemcnt = 0;
			}
			else
			{
				itemcnt++;
			}
			//DrawString(SCREEN_SIZE_X / 2 - 40, SCREEN_SIZE_Y / 2 - 5, "HIT", 0xffffff);
		}




		if (player.dropFlag == true)
		{
			DrawGraph(20, 10, segweyImage_icon, true);
		}

		ItemSegwey();

		break;
	}

}

bool GetPlayerV(void)
{
	return player.visible;
}

void PlNormal(void)
{
	player.jumpFlag = false;
	player.runFlag = false;

	Position player_RD = { player.pos.x + player.moveSpeed + player.hitPosE.x , player.pos.y - 1 };
	Position player_LD = { player.pos.x - player.moveSpeed - player.hitPosS.x - 1 , player.pos.y - 1 };

	if (player.pos.y > SCREEN_SIZE_Y)mapPos.y += ACC_G / 3;	// 下がる
	if (player.pos.y < SCREEN_SIZE_Y + 200)mapPos.y -= ACC_G / 2;// 上がる
	

	if (newKey[P1_RIGHT])
	{
		player.runFlag = true;
		player.moveDir = DIR_RIGHT;

		// 壁確認
		if ((WallBlockPass(player_RD)) && (player.wallRunSpeed == player.moveSpeed) && (!newKey[P1_A])) {
			player_state = PLAYER_WALL_RIGHT;
		}
		else {
			if (IsPass(player_RD))// RDのchipID取得して移動できるIDか調べる
			{
				player.pos.x += player.moveSpeed;
				if (player.pos.x > SCREEN_SIZE_X / 2)//カメラが右に行く
				{
					mapPos.x += player.moveSpeed;
				}
			}
			else
			{
				// 隣まで計算してplayer.pos.xに代入
				player.pos.x = GetWorldPos_Map(player_RD, DIR_LEFT).x - player.hitPosS.x;

			}
		}
	}
	

	if (newKey[P1_LEFT])
	{
		player.runFlag = true;
		player.moveDir = DIR_LEFT;

		// 壁確認
		if ((WallBlockPass(player_LD)) && (player.wallRunSpeed == player.moveSpeed) && (!newKey[P1_A])) {
			if (player.wallRunSpeed == player.moveSpeed) {
				player_state = PLAYER_WALL_LEFT;
			}
		}
		else
		{
			if (IsPass(player_LD))	// LDのchipID取得して移動できるIDか調べる
			{
				player.pos.x -= player.moveSpeed;
				//if (player.pos.x < expData.mapWidth * CHIP_SIZE_X - SCREEN_SIZE_X / 2)//カメラが左に行く
				//{
				//	mapPos.x -= player.moveSpeed;
				//}
				if (player.pos.x < PLAY_SIZE_X - SCREEN_SIZE_X / 2)//カメラが左に行く
				{
					mapPos.x -= player.moveSpeed;
				}

			}
			else
			{
				// 隣まで計算してplayer.pos.xに代入
				player.pos.x = GetWorldPos_Map(player_LD, DIR_RIGHT).x + player.hitPosS.x;
			}
		}
	}

	if (trgKey[P2_UP])
	{
		if (player.moveDir == DIR_RIGHT)
		{
			player.right = true;
			player.left = false;
		}

		if (player.moveDir == DIR_LEFT)
		{
			player.right = false;
			player.left = true;
		}


		furiko_pos = { player.pos.x , player.pos.y - player.size.y };
		player.wireFlag = true;

		player_state = PLAYER_W_PRE;
	}



}

void PlJumpUp(void)
{
	Position player_RU = { player.pos.x + player.hitPosE.x - 1 , player.pos.y - player.moveSpeed - player.hitPosS.y };	// 右上
	Position player_LU = { player.pos.x - player.hitPosS.x , player.pos.y - player.moveSpeed - player.hitPosS.y };	// 左上

	player.UpDownSpeed += player.AddUpDownSpeed;

	//　RD と LD の y座標に-1して上のブロック情報を得る
	if (IsPass({ player_RU.x , player_RU.y - 1 }) && IsPass({ player_LU.x , player_LU.y - 1 }))
	{
		// 上に何もない
		player_state = PLAYER_JUMP_UP;
		player.jumpFlag = true;
	}
	else
	{
		// 上にブロックがある
		player.UpDownSpeed = DOWN_SPEED_DEF;
		player.AddUpDownSpeed = 0.5f;
		player_state = PLAYER_DOWN;
	}

	// 上昇の処理(ある一定値を超えそうになると落下に切り替わるようにする)
	if (player.UpDownSpeed <= 0.0f)
	{
		if (IsPass({ player_RU.x , (player_RU.y + player.UpDownSpeed) }) && IsPass({ player_LU.x , (player_LU.y + player.UpDownSpeed) }))
		{
			player.pos.y += player.UpDownSpeed;
		}
		else
		{
			player.pos.y = GetWorldPos_Map({ player_RU.x , (player_RU.y + player.UpDownSpeed) }, DIR_DOWN).y + player.hitPosS.y;
		}

	}
	else
	{
		player_state = PLAYER_DOWN;
	}

}

void PlDown(void)
{
	Position player_RD = { player.pos.x + player.hitPosE.x - 1, player.pos.y - 1 };		// 右下
	Position player_LD = { player.pos.x - player.hitPosS.x , player.pos.y - 1 };		// 左下

	//　RD と LD の y座標に+1して下のブロック情報を得る
	if (IsPass({ player_RD.x , player_RD.y + 1 }) && IsPass({ player_LD.x , player_LD.y + 1 }))
	{
		if (player_state == PLAYER_NORMAL)
		{
			player.UpDownSpeed = DOWN_SPEED_DEF;
			player.AddUpDownSpeed = 0.8f;
			player_state = PLAYER_DOWN;
		}
	}
	else
	{
		if (trgKey[P1_A])
		{
			player.UpDownSpeed = UP_SPEED_DEF;
			player.AddUpDownSpeed = 0.8f;
			player_state = PLAYER_JUMP_UP;
		}
		else
		{
			if (player_state != PLAYER_W_ACTION)
			{
				player_state = PLAYER_NORMAL;
			}

		}
		return;
	}

	player.UpDownSpeed += player.AddUpDownSpeed;

	// 落下処理
	if (IsPass({ player_RD.x , (player_RD.y + player.UpDownSpeed) }) && IsPass({ player_LD.x , (player_LD.y + player.UpDownSpeed) }))
	{
		player.pos.y += player.UpDownSpeed;
		player.jumpFlag = true;
	}
	else
	{
		player.pos.y = GetWorldPos_Map({ player_RD.x , (player_RD.y + player.UpDownSpeed) }, DIR_UP).y;
		player.wallRunSpeed = player.moveSpeed;
		player.jumpFlag = false;
		SetEffct(player.pos);
	}


}

void PlWirePrepare(void)
{
	furiko_pos.y = furiko_pos.y - ONEFRAME_WIRE_UP;

	Position furiko_RU = { furiko_pos.x + player.size.x / 2, furiko_pos.y };
	Position furiko_LU = { furiko_pos.x - player.size.x / 2, furiko_pos.y };

	if (player.pos.y - furiko_pos.y <= 300)	// 指定範囲
	{
		if (!(WireBlockPass({ furiko_RU.x , furiko_RU.y })) || !(WireBlockPass({ furiko_LU.x , furiko_LU.y })))
		{
			// 長さを出す
			_length = furiko_pos - player.pos;


			// プレイヤーの座標を-90°で設定?
			player.pos.x = cos((-90.0f * PI) / 180.0f) * _length.y + furiko_pos.x;
			player.pos.y = sin((-90.0f * PI) / 180.0f) * _length.y + furiko_pos.y;

			MaxDeg = -20.0f;
			minDeg = -160.0f;

			player.nowDeg = -90.0f;

			defDeg = -90.0f;

			furikoSpeed = FURIKO_SPEED_DEF;


			// 指定範囲以内 & 指定ブロックがある
			// 指定ブロックにワイヤーを繋げる
			//player.pos.x = furiko_RU.x - mapPos.x;
			//player.pos.y = furiko_RU.y + player.size.y - mapPos.y;
			player.visible = false;
			player.visible2 = true;
			player.wireFlag = false;
			player.wireOkFlag = true;
			player_state = PLAYER_W_ACTION;
			return;		// 1つ目に見つけたブロックでいいのでreturnで抜ける
		}
		

	}
	else
	{
		WirePreTimeCnt = 0;
		player.wireFlag = false;
		player.wireOkFlag = false;
		player_state = PLAYER_NORMAL;
	}

	//player.wireOkFlag = true;
	//player_state = PLAYER_Y_ACTION;
}


// 前と座標の中心点が変わっているから注意!!
void PlWireAction(void)
{
	if (player.wireOkFlag)
	{
		if (WireTimeCnt < 2000)
		{

			if (newKey[P2_B])
			{
				if (!IsPass({ player.pos.x , player.pos.y }))			// ここでそもそも壁の中でワイヤーを切り離したときの処置を書いとく
				{
					player.pos.y = GetWorldPos_Map(player.pos, DIR_DOWN).y + player.hitPosS.y + player.size.y;
				}
				jumpSpeed = FURIKO_SPEED_DEF;
				player.BlockFlag = true;
				player_state = PLAYER_W_JUMP;
				return;
			}


			AddRad();

			WireTimeCnt++;
		}
		else
		{
			player.wireFlag = false;
			player.wireOkFlag = false;
			player.visible = true;
			player.visible2 = false;
			player_state = PLAYER_NORMAL;

			// ワイヤー表示時間の初期化
			WireTimeCnt = 0;

			// 振り子スタート位置の初期化
			_v = 0;
			furiko_pos = { 0,0 };

			furikoSpeed = FURIKO_SPEED_DEF;


		}


	}
}

void AddRad(void)
{
	// 角度を足すんだよぉぉぉぉ!!!!!!

	player.pos.x = cos((player.nowDeg * PI) / 180.0f) * _length.y + furiko_pos.x;
	player.pos.y = sin((player.nowDeg * PI) / 180.0f) * _length.y + furiko_pos.y;

	if (player.right == true)
	{
		if (player.moveDir == DIR_RIGHT)
		{
			// プレイヤーが右向き

			furikoSpeed = furikoSpeed - FURIKO_ADD;							// 加速
			player.nowDeg = player.nowDeg - furikoSpeed;		// 1°ずつ引いてくやつ

			if (player.nowDeg >= defDeg)
			{
				player.moveDir = DIR_LEFT;
			}
		}
		else
		{
			// プレイヤーが左向き
			furikoSpeed = furikoSpeed + FURIKO_ADD;							// 減速
			player.nowDeg = player.nowDeg - furikoSpeed;		// 1°ずつ足してくやつ

			if (player.nowDeg <= defDeg)
			{
				player.moveDir = DIR_RIGHT;
			}
		}
	}

	if (player.left == true)
	{
		if (player.moveDir == DIR_RIGHT)
		{
			// プレイヤーが右向き
			furikoSpeed = furikoSpeed + FURIKO_ADD;							// 減速
			player.nowDeg = player.nowDeg - furikoSpeed;		// 1°ずつ引いてくやつ

			if (player.nowDeg <= defDeg)
			{
				player.moveDir = DIR_LEFT;
			}
		}
		else
		{
			// プレイヤーが左向き
			furikoSpeed = furikoSpeed - FURIKO_ADD;							// 加速
			player.nowDeg = player.nowDeg - furikoSpeed;		// 1°ずつ足してくやつ
			if (player.nowDeg >= defDeg)
			{
				player.moveDir = DIR_RIGHT;
			}
		}
	}

	
}

void PlWireJump(void)
{


	Position player_RU = { player.pos.x + player.hitPosE.x - 1 , player.pos.y - player.moveSpeed - player.hitPosS.y };	// 右上
	Position player_LU = { player.pos.x - player.hitPosS.x , player.pos.y - player.moveSpeed - player.hitPosS.y };		// 左上

	Position player_RD = { player.pos.x + player.hitPosE.x - 1, player.pos.y - 1 };										// 右下
	Position player_LD = { player.pos.x - player.hitPosS.x , player.pos.y - 1 };										// 左下

	player.visible = true;			// アニメーションするキャラが表示される
	player.visible2 = false;		// ワイヤー中の静止画キャラが非表示になる
	player.wireFlag = false;		// ワイヤーが非表示になる

	//player.JumpDeg = 180 - (player.nowDeg + 90);		// 打ち出し角

	if (player.right == true)
	{
		player.moveDir = DIR_RIGHT;

		player.JumpDeg = 180 - (player.nowDeg + 90);		// 打ち出し角

		if (player.BlockFlag == true)
		{
			player.pos.x += PLAYER_SPEED_NORMAL;

			if (player.pos.y >= player.JumpDeg - player.pos.y)
			{
				player.pos.y -= jumpSpeed;

			}
			if (player.pos.y > SCREEN_SIZE_Y)mapPos.y += ACC_G / 3;	// 下がる
			if (player.pos.y < SCREEN_SIZE_Y + 200)mapPos.y -= ACC_G / 2;// 上がる

			if (player.pos.x > SCREEN_SIZE_X / 2)//カメラが右に行く
			{
				mapPos.x += player.moveSpeed;
			}

			jumpSpeed = jumpSpeed - FURIKO_ADD;
		}

	}
	if (player.left == true)
	{
		player.moveDir = DIR_LEFT;

		player.JumpDeg = -(180 - (player.nowDeg + 90));		// 打ち出し角
		if (player.BlockFlag == true)
		{
			player.pos.x -= PLAYER_SPEED_NORMAL;

			if (player.pos.y >= player.JumpDeg - player.pos.y)
			{
				player.pos.y -= jumpSpeed;

			}
			if (player.pos.y > SCREEN_SIZE_Y)mapPos.y += ACC_G / 3;	// 下がる
			if (player.pos.y < SCREEN_SIZE_Y + 200)mapPos.y -= ACC_G / 2;// 上がる

			if (player.pos.x < PLAY_SIZE_X - SCREEN_SIZE_X / 2)//カメラが左に行く
			{
				mapPos.x -= player.moveSpeed;
			}

			jumpSpeed = jumpSpeed - FURIKO_ADD;
		}

	}

	//if (player.BlockFlag == true)
	//{
	//	player.pos.x += PLAYER_SPEED_NORMAL;

	//	if (player.pos.y >= player.JumpDeg - player.pos.y)
	//	{
	//		player.pos.y -= jumpSpeed;

	//	}
	//	if (player.pos.y > SCREEN_SIZE_Y)mapPos.y += ACC_G / 3;	// 下がる
	//	if (player.pos.y < SCREEN_SIZE_Y + 200)mapPos.y -= ACC_G / 2;// 上がる

	//	if (player.pos.x > SCREEN_SIZE_X / 2)//カメラが右に行く
	//	{
	//		mapPos.x += player.moveSpeed;
	//	}

	//	jumpSpeed = jumpSpeed - FURIKO_ADD;
	//}


	if (!IsPass({ player_RU.x , player_RU.y }) || !IsPass({ player_LU.x , player_LU.y }) || !IsPass({ player_RD.x , player_RD.y }) || !IsPass({ player_LD.x , player_LD.y }))
	{
		player.BlockFlag = false;
	}

	if (player.BlockFlag == false)
	{
		if (IsPass({ player_RU.x , player_RU.y }) || IsPass({ player_LU.x , player_LU.y }) || IsPass({ player_RD.x , player_RD.y }) || IsPass({ player_LD.x , player_LD.y }))
		{
			player_state = PLAYER_DOWN;
		}
	}

}

// 壁を走る
// 右壁
void PlWall_R(void)
{
	player.runFlag = false;
	player.jumpFlag = false;
	// 位置修正
	player.pos.x = GetWorldPos_Map({ player.pos.x + player.moveSpeed + player.hitPosE.x , player.pos.y - 1 }, DIR_LEFT).x;

	Position player_RU = { player.pos.x - 1, player.pos.y - player.wallRunSpeed - player.hitPosE.x };

	if (player.pos.y < SCREEN_SIZE_Y + 200)mapPos.y -= ACC_G / 2;// 上がる


	// 専用の壁か？
	if (WallBlockPass(player.pos)) {
		// もう走れない・途中で降りる
		if ((player.wallRunSpeed <= 0) || (newKey[P1_A])) {
			// 位置修正
			player.pos.x = GetWorldPos_Map(player_RU, DIR_LEFT).x;
			if (oldKey[P1_LEFT]) {
				player.wallRunSpeed = player.moveSpeed;
				player_state = PLAYER_WALL_LEFT;
			}
			else {
				// 通常に戻す
				player_state = PLAYER_NORMAL;
			}
		}
		else if (player.wallRunSpeed >= 0) {
			// 上へ
			if (newKey[P1_RIGHT]) {
				player.runFlag = true;
				player.moveDir = DIR_RIGHT;
				if (IsPass(player_RU)) {	// 壁確認
					player.pos.y -= player.wallRunSpeed;
				}
				else {
					player.pos.y = GetWorldPos_Map(player_RU, DIR_DOWN).y + player.hitPosS.x;
				}
			}
			player.wallRunSpeed -= 0.1f;	// 減速
		}
	}
	else {
		// 通常に戻す
		player_state = PLAYER_NORMAL;
	}
}

// 左壁
void PlWall_L(void)
{
	player.runFlag = false;
	player.jumpFlag = false;
	// 位置修正
	player.pos.x = GetWorldPos_Map({ player.pos.x - player.moveSpeed - player.hitPosS.x - 1 , player.pos.y - 1 }, DIR_RIGHT).x;

	Position player_LU = { player.pos.x, player.pos.y - player.moveSpeed - player.hitPosE.x };

	if (player.pos.y < SCREEN_SIZE_Y + 200)mapPos.y -= ACC_G / 2;// 上がる


	// 専用の壁か？
	if (WallBlockPass({ player.pos.x - 1,player.pos.y })) {
		// もう走れない・途中で降りる
		if ((player.wallRunSpeed <= 0) || (newKey[P1_A])) {
			//位置修正
			player.pos.x = GetWorldPos_Map(player_LU, DIR_RIGHT).x;
			if (oldKey[P1_RIGHT]) {
				player.wallRunSpeed = player.moveSpeed;
				// 右壁に変更
				player_state = PLAYER_WALL_RIGHT;
			}
			else {
				// 通常に戻す
				player_state = PLAYER_NORMAL;
			}
		}
		else if (player.wallRunSpeed >= 0) {
			// 上へ
			if (newKey[P1_LEFT]) {
				player.runFlag = true;
				player.moveDir = DIR_LEFT;
				if (IsPass(player_LU)) {	// 壁確認
					player.pos.y -= player.moveSpeed;
				}
				else {
					player.pos.y = GetWorldPos_Map(player_LU, DIR_DOWN).y + player.hitPosE.x;
				}
			}
			player.wallRunSpeed -= 0.1f;	// 減速
		}
	}
	else {
		// 通常に戻す
		player_state = PLAYER_NORMAL;
	}
}



void PlayerState(void)
{
	switch (player_state)
	{
	case PLAYER_NORMAL:			// 左右移動 
	case PLAYER_DOWN:	        // ジャンプ下降
		PlNormal();
		if (player_state != PLAYER_W_PRE)
		{
			PlDown();
		}
		break;
	case PLAYER_JUMP_UP:		// ジャンプの上昇
		PlNormal();
		if (player_state != PLAYER_W_PRE)
		{
			PlJumpUp();
		}
		break;
	case PLAYER_W_PRE:
		PlWirePrepare();		// ﾜｲﾔｰｱｸｼｮﾝの準備(ワイヤーを伸ばせるか判定)
		break;
	case PLAYER_W_ACTION:		// ﾜｲﾔｰｱｸｼｮﾝ
		PlWireAction();
		break;
	case PLAYER_W_JUMP:			// ﾜｲﾔｰｼﾞｬﾝﾌﾟ
		PlWireJump();
		break;
	case PLAYER_WALL_RIGHT:		// 右壁
		PlWall_R();
		break;
	case PLAYER_WALL_LEFT:		// 左壁
		PlWall_L();
		break;
	default:
		break;
	}
	player.animCnt++;

}

void GetItemRand(void)
{
	//if (player.dropFlag)
	//{
	//	//取得アイテムをランダムで決めて、item_stateをどれかに切り替える
	//	//どれかといわれても今はセグウェイしかない
	//	//DrawGraph(20, 10, segweyImage_icon, true);
	//	if (trgKey[P2_A])
	//	{
	//		ItemSegwey();
	//		item_state = ITEM_SEGWEY;
	//	}
	//	
	//}
}

void ItemSegwey(void)
{
	if (trgKey[P2_A])
	{
		player.segweyFlag = true;
	}

	if (player.segweyFlag == true)
	{
		player.moveSpeed = PLAYER_SPEED_SEGWEY;

		if (Segwey_Cnt == 80)
		{
			player.segweyFlag = false;
			player.dropFlag = false;
			Segwey_Cnt = 0;
			player.moveSpeed = PLAYER_SPEED_NORMAL;
		}
		else
		{
			Segwey_Cnt++;
		}
	}
	
}

void ItemKabosu(void)
{
}

void ItemUFO(void)
{
}

void ItemSP(void)
{
}

void ItemState(void)
{
	switch (item_state)
	{
	case ITEM_NON:
		PlNormal();
		break;
	case ITEM_SEGWEY:
		ItemSegwey();
		break;
	case ITEM_KABOSU:
		ItemKabosu();
		break;
	case ITEM_UFO:
		ItemUFO();
		break;
	case ITEM_SPECIAL:
		ItemSP();
		break;
	default:
		break;
	}
}