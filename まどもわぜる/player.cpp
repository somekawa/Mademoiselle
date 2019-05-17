

#include "Dxlib.h"				// DxLibﾗｲﾌﾞﾗﾘを使用する　独自で準備したﾍｯﾀﾞｰﾌｧｲﾙは""で指定する
#include <math.h>				// 標準ﾍｯﾀﾞｰﾌｧｲﾙは<>で指定する
#include"keycheck.h"
#include"main.h"
#include"stage.h"
#include"player.h"
#include"shot.h"

#define INIT_VELOCITY 50		// 初期速度
#define SECOND_PER_FRAME 0.3	// 1ﾌﾚｰﾑの秒数

#define DOWN_SPEED_DEF	5.0f	// デフォルトの落下速度
#define UP_SPEED_DEF   -15.0f	// デフォルトの上昇速度

#define CORRECTION	  100		// ワイヤーの長さ補正用数値(特にy座標)

#define KEEPPOSX_CORRECTION  120	// posの値の補正用数値

#define ONEFRAME_WIRE_UP	   5    // 1フレームで上がるワイヤーの速度

//#define PAI 3.141592
//#define JUMPSPEED 2

PLAYER_STATE player_state;

// ﾌﾟﾚｲﾔｰｽﾋﾟｰﾄﾞ
enum PLAYER_SPEED {
	PLAYER_SPEED_NORMAL = 8,
	PLAYER_SPEED_SEGWEY = 12
};

int playerImage[2];
int runImage[2][10];
int jumpImage[2];
int stopJumpImage[2];
int downImage;
int shotImage[2];
int segweyImage[2][2];
CHARACTER player;
int downPos;

int WirePreTimeCnt;					// ワイヤーを少しずつ伸ばして途中で途切れるまでの時間
int WireTimeCnt;					// ワイヤーの表示時間

bool _isPushJump;
bool _isJumped;


void OnMove(float& x, float& y, float vx, float vy);
void OnAdjust();



Position furiko_pos;
Position _endPoint;
float _g;
float _v;
float _length;

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

//float rot;



typedef Position Vec2;

inline float Dot(const Vec2 & a, const Vec2 & b) {
	return a.x* b.x + a.y * b.y;
}

inline float Cross(const Vec2 & a, const Vec2 & b) {
	return a.x* b.y - b.x * a.y;
}




void PlayerSystmInit(void)
{
	// 赤
	playerImage[PLAYER_RED] = LoadGraph("image/playerR_stop.png");
	jumpImage[PLAYER_RED] = LoadGraph("image/playerR_jump.png");
	stopJumpImage[PLAYER_RED] = LoadGraph("image/playerR_stop_jump.png");
	LoadDivGraph("image/playerR_run.png", 10, 5, 2, 72, 72, runImage[PLAYER_RED], true);
	LoadDivGraph("image/playerR_segway.png", 2, 2, 1, 72, 72, segweyImage[PLAYER_RED], true);

	// 青
	playerImage[PLAYER_BLUE] = LoadGraph("image/playerB_stop.png");
	jumpImage[PLAYER_BLUE] = LoadGraph("image/playerB_jump.png");
	stopJumpImage[PLAYER_BLUE] = LoadGraph("image/playerB_stop_jump.png");
	LoadDivGraph("image/playerB_run.png", 10, 5, 2, 72, 72, runImage[PLAYER_BLUE], true);
	LoadDivGraph("image/playerB_segway.png", 2, 2, 1, 72, 72, segweyImage[PLAYER_BLUE], true);

	downImage = LoadGraph("image/red_down.png");
	shotImage[0] = LoadGraph("image/red_stop_shot.png");
	shotImage[1] = LoadGraph("image/red_down_shot.png");



}

void PlayerGameInit(void)
{
	player.type = PLAYER_RED;
	player.size = { 72,72 };
	player.offsetSize = { -player.size.x / 2,-player.size.y };
	player.hitPosE = { 20,0 };
	player.hitPosS = { 20,62 };
	player.pos = { CHIP_SIZE_X * 15, CHIP_SIZE_Y * 14 };
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

	player.visible = true;
	player.visible2 = false;
	player.imgLocCnt = 0;
	downPos = 0;

	//ひもの支点の初期化
	_endPoint.x = 0;
	_endPoint.y = 0;
	_v = 0;			// 振り子のふり幅
	WireTimeCnt = 0;

	_g = 2.0f;		//重力の定義
	_length = 0;	//紐の長さの計算

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


	//rot = -rand() % 90;
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
			//player.animCnt++;
		}
		//if (trgKey[P1_UP]) player.visible = !player.visible;

		break;
	}

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
	if (player.pos.y < CHIP_SIZE_Y * MAP_Y - SCREEN_SIZE_Y + CHIP_SIZE_Y * 5) mapPos.y -= ACC_G * SECOND_PER_FRAME;
	//
	//
	//		//// 左右にじわじわ動く
			/*if (player.pos.x > SCREEN_SIZE_X - CHIP_SIZE_X * 5) mapPos.x += 1;
			if (player.pos.x < CHIP_SIZE_X * MAP_X - SCREEN_SIZE_X + CHIP_SIZE_X * 5) mapPos.x -= SECOND_PER_FRAME;*/
			//	break;
			//}
		//	
		//
	PlayerState();
}

void PlayerDraw(void)
{
	switch (GetGameMode()) {
	case GMODE_CHARASERE:
		if (player.visible) {
			DrawRotaGraph(160, 300, 3, 0, runImage[player.type][(player.animCnt / 3) % 10], true);
			DrawString(120, 180, "キャラ決定！", 0x000000);
		}
		else {
			DrawRotaGraph(160, 300, 3, 0, playerImage[player.type], true);

		}

		break;
	case GMODE_GAME:
		if (player.visible && !player.visible2)
		{
			int img = playerImage[player.type];
			if ((player.runFlag) && (!player.jumpFlag)) img = runImage[player.type][(player.animCnt / 3) % 10];
			if ((player.jumpFlag) && (player.runFlag)) img = jumpImage[player.type];
			if ((player.jumpCnt > 0) && (!player.runFlag)) img = stopJumpImage[player.type];
			if (player.downFlag) img = downImage;
			if ((!player.runFlag) && (oldKey[P1_A])) img = shotImage[0];
			if ((!player.runFlag) && (oldKey[P1_A]) && (player.downFlag)) img = shotImage[1];
			if (player.segweyFlag) img = segweyImage[player.type][(player.animCnt / 5) % 2];
			if (player.moveDir == DIR_RIGHT) {

				DrawGraph(player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y, img, true);
			}
			else if (player.moveDir == DIR_LEFT) {
				DrawTurnGraph(player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y, img, true);
			}
			if (player.downFlag) {
				DrawString(780, 0, "PLAYERDOWN OK", 0xffffff);
			}
			DrawBox(player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y,
				player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y - mapPos.y, 0xff0000, false);


			DrawBox(player.pos.x - player.hitPosS.x - mapPos.x, player.pos.y - player.hitPosS.y - mapPos.y + downPos,
				player.pos.x + player.hitPosE.x - mapPos.x, player.pos.y - mapPos.y + downPos, 0x00ffff, false);


			DrawLine(player.pos.x + player.offsetSize.x - mapPos.x, player.pos.y - (player.size.y / 2) - mapPos.y,
				player.pos.x - player.offsetSize.x - mapPos.x, player.pos.y - (player.size.y / 2) - mapPos.y, 0x00ffff, true);
			DrawLine(player.pos.x - mapPos.x, player.pos.y + player.offsetSize.y - mapPos.y,
				player.pos.x - mapPos.x, player.pos.y - mapPos.y, 0x00ffff, true);
		}

		if (!player.visible && player.visible2)
		{
			// ワイヤーがあまりにも短すぎたときの対処
			//if (furiko_pos.y < CORRECTION)
			//{
			//	furiko_pos.y = CORRECTION;
			//}

			if (player.moveDir == DIR_RIGHT)
			{
				DrawGraph(player.pos.x - player.size.x - mapPos.x , player.pos.y - player.size.y  - mapPos.y , jumpImage[player.type], true);		   // キャラクタをおもりとして描画
			}
			else
			{
				DrawTurnGraph(player.pos.x - player.size.x / 3 - mapPos.x, player.pos.y - player.size.y / 4 - mapPos.y, jumpImage[player.type], true); // キャラクタをおもりとして描画
			}

		}

		if (player_state == PLAYER_W_PRE || player_state == PLAYER_W_ACTION)
		{
			// 指定ブロックがとりあえず自分より上にある状態なら
			if (player.wireOkFlag == true)
			{
				DrawLine(player.pos.x - mapPos.x, player.pos.y - player.size.y - mapPos.y, furiko_pos.x - mapPos.x, furiko_pos.y - mapPos.y, 0xffffffff, 2);		// 動くけどキャラに固定されないひも(だったもの)
			}
			else
			{
				if (WirePreTimeCnt <= 30)
				{
					DrawLine(player.pos.x - mapPos.x, player.pos.y - player.size.y - mapPos.y, furiko_pos.x - mapPos.x, furiko_pos.y - mapPos.y, 0xffffffff, 2);		// 動くけどキャラに固定されないひも(だったもの)
					WirePreTimeCnt++;
				}
				
			}

			// 指定ブロックが上にないとき(何か障害物にあたるまでは伸ばしきりたい)

			
		}


		break;
	}

}



void WireDraw(void)
{
	// ワイヤーアクションをはじめたら、中心座標を固定

	// WとFを同時に押したときのバグがえぐい



	//if (player.wireFlag)
	//{
	//	

	//	if (TimeCnt < 150)
	//	{
	//		skyFlag = true;

	//		//ひもの支点を定義する
	//		if (player.moveDir == DIR_RIGHT)
	//		{
	//			_endPoint.x = KeepPosX + CHIP_SIZE_X;
	//		}
	//		else
	//		{
	//			_endPoint.x = KeepPosX - CHIP_SIZE_X;
	//		}
	//		_endPoint.y = KeepPosY /*+ CHIP_SIZE_Y*/;

	//		Vec2 v = (_pos - _endPoint);//振り子の支点から振り子の錘までのベクトル
	//		v = v.normalized();//正規化する
	//		//外積と内積を利用して角度を計算
	//		float cost = Dot(v, Vec2(-1, 0));
	//		float sint = Cross(v, Vec2(-1, 0));
	//		float theta = atan2f(cost, sint);

	//		_v += _g * cost;

	//		

	//		if (CheckHitKey(KEY_INPUT_F))				 // Fキーを押したら
	//		{
	//			XY movedPos = player.pos;
	//			XY movedHitCheck = movedPos;
	//			XY movedHitCheck2;
	//			XY movedHitCheck3;
	//			

	//			movedPos = player.pos;
	//			movedHitCheck.y = movedPos.y + player.offsetSize.y;
	//			movedHitCheck2 = movedHitCheck;													// 左
	//			movedHitCheck2.x = movedPos.x - player.hitPosS.x;
	//			movedHitCheck3 = movedHitCheck;													// 右
	//			movedHitCheck3.x = movedPos.x + player.hitPosE.x - 1;

	//			

	//			playerX_now = _pos.x + mapPos.x;		 // 切り離す瞬間
	//			playerY_now = _pos.y + mapPos.y;		 // 切り離す瞬間

	//			getRadian(playerX_old, playerY_old, playerX_now, playerY_now);					// 角度(ラジアン)を求めるためのやつ


	//			// 地面に接触してる
	//			//if (!(IsPass(movedHitCheck)) && (IsPass(movedHitCheck2)) && (IsPass(movedHitCheck3)))
	//			//{		
	//			//	PlayerDraw();
	//			//	skyFlag = false;
	//			//}
	//			//else
	//			//{
	//			//	//playerX_now = _pos.x + mapPos.x;		 // 切り離す瞬間
	//			//	//playerY_now = _pos.y + mapPos.y;		 // 切り離す瞬間

	//			//	//getRadian(playerX_old, playerY_old, playerX_now, playerY_now);

	//			//	while (skyFlag == true)
	//			//	{
	//			//		Disassembly_C(radian);			// x軸
	//			//		Disassembly_S(radian, _g);		// y軸

	//			//		DrawGraph(_pos.x + mapPos.x + Disassembly_C(radian), _pos.y + mapPos.y + Disassembly_S(radian, _g), stopJumpImage[player.type], true);
	//			//	}
	//			//	
	//			//}
	//			

	//			
	//				

	//				//Disassembly_C(radian);			// x軸
	//				//Disassembly_S(radian);			// y軸

	//				player.visible = true;			// アニメーションするキャラが表示される
	//				player.visible2 = false;		// ワイヤー中の静止画キャラが非表示になる
	//				player.wireFlag = false;		// ワイヤーが非表示になる

	//				// 着地地点の描画位置
	//				player.pos.x = _pos.x + mapPos.x + player.offsetSize.x;
	//				//player.pos.y = _pos.y + mapPos.y;

	//				DrawGraph(Disassembly_C(radian), Disassembly_S(radian , _g), stopJumpImage[player.type], true);
	//			
	//				
	//				// ここに二段ジャンプ用処理の追加が必要かも

	//				

	//				PlayerDraw();
	//				
	//			
	//		}
	//		else
	//		{
	//			// 更新前にoldに保存
	//			playerX_old = _pos.x + mapPos.x;		// 1フレーム前
	//			playerY_old = _pos.y + mapPos.y;		// 1フレーム前

	//			playerX_now = 0;						// 切り離す瞬間だからまだ0
	//			playerY_now = 0;						// 切り離す瞬間だからまだ0

	//			player.visible = false;
	//			player.visible2 = true;
	//			player.wireFlag = true;
	//			_isPushJump = false;
	//		}





	//		//あとは振り子の角度に従って、その時々の加速度を求め、
	//		//速度(_v)に加算しよう
	//		//それをX成分、Y成分に分けて
	//		//OnMoveの第3第4引数に代入

	//		
	//		OnMove(_pos.x, _pos.y, _v * sint, _v * cost);
	//		OnAdjust();		// これがないとひもが伸びていく
	//	
	//	//DrawLine(KeepPos, player.pos.y, KeepPos+100, 0, 0xffffffff, 1);		// 動かないけどキャラに固定されるひも

	//	//DrawLine(_pos.x, _pos.y, KeepPosX + CHIP_SIZE_X, KeepPosY + CHIP_SIZE_Y, 0xffffffff, 2);			// 動くけどキャラに固定されないひも

	//	//DrawGraph(_pos.x, _pos.y, playerImage, true);						// キャラクタをおもりとして描画

	//		TimeCnt++;
	//	}
	//	else
	//	{
	//		player.wireFlag = false;
	//		player.visible = true;
	//		player.visible2 = false;
	//		skyFlag = false;
	//	}

	//}
}



void OnMove(float& x, float& y, float vx, float vy) {
	// 移動のための処理
	x += vx;
	y += vy;

}

void OnAdjust() {
	Vec2 v = (furiko_pos - _endPoint);
	if (v.Length() > _length) {
		furiko_pos = _endPoint + v.normalized() * _length;
	}
}

bool GetPlayerV(void)
{
	return player.visible;
}

float getRadian(float old_x, float old_y, float now_x, float now_y)
{
	radian = atan2f(now_y - old_y, now_x - old_x);
	return radian;
}


float Disassembly_C(float radian_cos)		// x軸
{
	//radian_cos = radian;
	return cosf(radian_cos);
}

float Disassembly_S(float& radian_sin, float g)		// y軸
{
	//radian_sin = radian;
	//g = _g;
	radian_sin = radian_sin - g;		// 減算処理?
	return sinf(radian_sin);
}


//float Disassembly(float radian_cos , float radian_sin)
//{
//	/*radian_cos = radian;
//	radian_sin = radian;*/
//	return (cosf(radian) , sinf(radian));
//}

void PlNormal(void)
{
	player.runFlag = false;

	Position player_RD = { player.pos.x + player.moveSpeed + player.hitPosE.x , player.pos.y - 1 };
	Position player_LD = { player.pos.x - player.moveSpeed - player.hitPosS.x , player.pos.y - 1 };

	if (newKey[P1_RIGHT])
	{
		player.runFlag = true;
		player.moveDir = DIR_RIGHT;
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
	if (newKey[P1_LEFT])
	{
		player.runFlag = true;
		player.moveDir = DIR_LEFT;
		if (IsPass(player_LD))	// LDのchipID取得して移動できるIDか調べる
		{
			player.pos.x -= player.moveSpeed;
			if (player.pos.x < MAP_X * CHIP_SIZE_X - SCREEN_SIZE_X / 2)//カメラが左に行く
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

	if (trgKey[P2_UP])
	{
		//紐の長さの計算

		_length = player.pos.y;

		// 紐の長さの補正
		if (_length >= 250)		// 長すぎるとき
		{
			_length = 250;
		}
		else if (_length < 120)		// 短すぎるとき
		{
			_length = 120;
		}
		
		furiko_pos = { player.pos.x , player.pos.y - player.size.y };
		_endPoint = player.pos;
		
		OnAdjust();
		player.wireFlag = true;

		player_state = PLAYER_W_PRE;
	}



}

void PlJumpUp(void)
{
	Position player_RU = { player.pos.x + player.moveSpeed + player.hitPosE.x , player.pos.y - player.moveSpeed - player.hitPosS.y };	// 左上
	Position player_LU = { player.pos.x - player.moveSpeed - player.hitPosS.x , player.pos.y - player.moveSpeed - player.hitPosS.y };	// 右上

	player.UpDownSpeed += player.AddUpDownSpeed;

	//　RD と LD の y座標に-1して上のブロック情報を得る
	if (IsPass({ player_RU.x , player_RU.y - 1 }) && IsPass({ player_LU.x , player_LU.y - 1 }))
	{
		// 上に何もない
		player_state = PLAYER_JUMP_UP;
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
	Position player_RD = { player.pos.x + player.moveSpeed + player.hitPosE.x , player.pos.y - 1 };		// 右下
	Position player_LD = { player.pos.x - player.moveSpeed - player.hitPosS.x , player.pos.y - 1 };		// 左下

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
		if (trgKey[P1_UP])
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
	}
	else
	{
		player.pos.y = GetWorldPos_Map({ player_RD.x , (player_RD.y + player.UpDownSpeed) }, DIR_UP).y;
	}


}

void PlWirePrepare(void)
{
	furiko_pos.y = furiko_pos.y - ONEFRAME_WIRE_UP;

	Position furiko_RU = { furiko_pos.x + player.size.x / 2, furiko_pos.y };
	Position furiko_LU = { furiko_pos.x - player.size.x / 2, furiko_pos.y };

	if ( !(WireBlockPass({ furiko_RU.x , furiko_RU.y })) || !(WireBlockPass({ furiko_LU.x , furiko_LU.y })))  
	{
		if(player.pos.y - furiko_pos.y <= 300)	// 指定範囲
		{
			// 指定範囲以内 & 指定ブロックがある
			// 指定ブロックにワイヤーを繋げる
			player.pos.x = furiko_RU.x - mapPos.x;
			player.pos.y = furiko_RU.y + player.size.y - mapPos.y;
			player.visible = false;
			player.visible2 = true;
			player.wireFlag = false;
			player.wireOkFlag = true;
			player_state = PLAYER_W_ACTION;
			return;		// 1つ目に見つけたブロックでいいのでreturnで抜ける
		}
		else
		{
			// 指定範囲外 & 指定ブロックがある
			//player.visible = true;
			//player.visible2 = false;
			WirePreTimeCnt = 0;
			player.wireFlag = false;
			player.wireOkFlag = false;
			player_state = PLAYER_NORMAL;
		}
		
	}
	else
	{
		// 指定範囲外 & 指定ブロックがない
		
	}

	//player.wireOkFlag = true;
	//player_state = PLAYER_Y_ACTION;
}


// 前と座標の中心点が変わっているから注意!!
void PlWireAction(void)
{
	if (player.wireOkFlag)
	{
		if (WireTimeCnt < 150)
		{
			//ひもの支点を定義する
			if (player.moveDir == DIR_RIGHT)
			{
				_endPoint.x = furiko_pos.x;
			}
			else
			{
				_endPoint.x = furiko_pos.x;
			}
			_endPoint.y = furiko_pos.y;

			Vec2 v = (_endPoint - player.pos);//振り子の支点から振り子の錘までのベクトル
			v = v.normalized();//正規化する

			//外積と内積を利用して角度を計算
			float cost = Dot(v, Vec2(-1, 0));
			float sint = Cross(v, Vec2(-1, 0));
			float theta = atan2f(cost, sint);

			_v += _g * cost;

			OnMove(player.pos.x, player.pos.y, _v * sint, _v * cost);
			OnAdjust();		// これがないとひもが伸びていく
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
			if (player.moveDir == DIR_RIGHT)
			{
				_v = 0;
				furiko_pos = { 0,0 };
			}
			if (player.moveDir == DIR_LEFT)
			{
				_v = 0;
				furiko_pos = { 2400,0 };
			}

		}


	}
}

void PlWireJump(void)
{
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
	default:
		break;
	}
	player.animCnt++;

}