#include "Dxlib.h"				// DxLibﾗｲﾌﾞﾗﾘを使用する　独自で準備したﾍｯﾀﾞｰﾌｧｲﾙは""で指定する
#include <math.h>				// 標準ﾍｯﾀﾞｰﾌｧｲﾙは<>で指定する
#include "keycheck.h"
#include "main.h"
#include "stage.h"
#include "player.h"
#include "effect.h"

#define INIT_VELOCITY 50			// 初期速度
#define SECOND_PER_FRAME 0.3		// 1ﾌﾚｰﾑの秒数

#define DOWN_SPEED_DEF	5.0f		// デフォルトの落下速度
#define UP_SPEED_DEF   -15.0f		// デフォルトの上昇速度

#define CORRECTION	  100			// ワイヤーの長さ補正用数値(特にy座標)

#define KEEPPOSX_CORRECTION  120	// posの値の補正用数値

#define ONEFRAME_WIRE_UP	  15    // 1フレームで上がるワイヤーの速度

#define FURIKO_SPEED_DEF      10.0f	// 振り子の初速度
#define FURIKO_SPEED_MAX      3.0f	// 振り子の最大速度
#define FURIKO_SPEED_MIN	  0.5f	// 振り子の最小速度
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



// 画像タイプ
enum IMAGE_TYPE {
	IMAGE_TYPE_STOP,	// 立ち止まっている画像
	IMAGE_TYPE_RUN,		// 走っている画像
	IMAGE_TYPE_ICON,	// 顔ｱｲｺﾝ
	IMAGE_TYPE_JUMP,
	IMAGE_TYPE_STOPJUMP,
	//IMAGE_TYPE_SEGWAY,
	IMAGE_TYPE_MAX
};
#define PLAYER_MAX 1
#define RUN_ANIM_MAX  10			// ｺﾏ数
#define SEGWAY_ANIM_MAX 2
typedef struct {
	int playerImage;					// 止まった画像
	int runImage[RUN_ANIM_MAX];			// 走っている画像
	int iconImage;						// 顔画像
	int jumpImage;						// ジャンプ画像
	int stopJumpImage;					// ｼﾞｬﾝﾌﾟしてから落ちる時
	//int segwayImage[SEGWAY_ANIM_MAX];	// ｾｸﾞｳｪｲ画像
}CHARACTER_IMAGE;
CHARACTER_IMAGE charImage[CHARA_MAX];
CHARACTER player[PLAYER_MAX];


int p1Wak[2];
int yazirusiImage[2];
//int playerImage[2];
//int runImage[2][10];
//int jumpImage[2];
//int stopJumpImage[2];
int segweyImage[2][2];

int WirePreTimeCnt;					// ワイヤーを少しずつ伸ばして途中で途切れるまでの時間
int WireTimeCnt;					// ワイヤーの表示時間

bool _isPushJump;
bool _isJumped;

void OnMove(float& x, float& y, float vx, float vy);
void OnAdjust();

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

float MaxDeg;		// 最大角度
float minDeg;		// 最小角度
float furikoSpeed;	// 振り子のスピード

float defDeg;

// アイテム関連 
bool itemBoxFlag = true;
int itemBoxPosX = CHIP_SIZE_X * 15 - mapPos.x;
int itemBoxPosY = CHIP_SIZE_Y * 16 - mapPos.y;
int hatenaImage;		// ？ﾎﾞｯｸｽ画像
int segwayICImage;
int kabosuImage;
int UFOImage;
int specialImage;

typedef Position Vec2;

inline float Dot(const Vec2 & a, const Vec2 & b)
{
	return a.x* b.x + a.y * b.y;
}

inline float Cross(const Vec2 & a, const Vec2 & b)
{
	return a.x* b.y - b.x * a.y;
}

void PlayerSystmInit(void)
{
	p1Wak[0] = LoadGraph("image/p1CSWak.png");
	p1Wak[1] = LoadGraph("image/p1GWak.png");
	for (int i = 0; i < 2; i++)
	{
		yazirusiImage[i] = LoadGraph("image/yazirusi.png");
	}
	const char *fileNameList[CHARA_MAX][IMAGE_TYPE_MAX] = {
	{"image/playerR_stop.png","image/playerR_run.png","image/R_icon.png","image/playerR_jump.png","image/playerR_stop_jump.png"/*,"image/playerR_segway.png"*/},	//0 red
	{"image/playerB_stop.png","image/playerB_run.png","image/B_icon.png","image/playerB_jump.png","image/playerB_stop_jump.png"/*,"image/playerB_segway.png"*/},	//1blue
	{"image/playerG_stop.png","image/playerG_run.png","image/G_icon.png","image/playerG_jump.png","image/playerG_stop_jump.png"/*,"image/playerG_segway.png"*/},
	{"image/playerP_stop.png","image/playerP_run.png","image/P_icon.png","image/playerP_jump.png","image/playerP_stop_jump.png"/*,"image/playerP_segway.png"*/},
	{"image/playerO_stop.png","image/playerO_run.png","image/O_icon.png","image/playerO_jump.png","image/playerO_stop_jump.png"/*,"image/playerO_segway.png"*/},
	};
	for (int j = 0; j < CHARA_MAX; j++)
	{
		// 止まった画像
		charImage[j].playerImage = LoadGraph(fileNameList[j][IMAGE_TYPE_STOP]);

		// 走っている画像
		LoadDivGraph(fileNameList[j][IMAGE_TYPE_RUN],
			RUN_ANIM_MAX,			// 全体枚数	
			RUN_ANIM_MAX / 2, RUN_ANIM_MAX / 5,			// 横ｺﾏ数、縦ｺﾏ数
			72, 72,							// 縦サイズ、横ｻｲｽﾞ
			&charImage[j].runImage[0]);

		// 顔アイコン
		charImage[j].iconImage = LoadGraph(fileNameList[j][IMAGE_TYPE_ICON]);
		// ジャンプしているとき
		charImage[j].jumpImage = LoadGraph(fileNameList[j][IMAGE_TYPE_JUMP]);
		// ジャンプの上ら辺のとき
		charImage[j].stopJumpImage = LoadGraph(fileNameList[j][IMAGE_TYPE_STOPJUMP]);

		// ｾｸﾞｳｪｲ
		//LoadDivGraph(fileNameList[j][IMAGE_TYPE_SEGWAY],
		//	SEGWAY_ANIM_MAX,			// 全体枚数	
		//	SEGWAY_ANIM_MAX/2, SEGWAY_ANIM_MAX,			// 横ｺﾏ数、縦ｺﾏ数
		//	72, 72,							// 縦サイズ、横ｻｲｽﾞ
		//	&charImage[j].segwayImage[0]);
	}

	// アイテム関連
	hatenaImage = LoadGraph("image/hatena.png");
	segwayICImage = LoadGraph("image/segway.png");
	//kabosuImage = LoadGraph("image/kabosu.png");
	//UFOImage = LoadGraph("image/UFOIC.png");
	//specialImage = LoadGraph("image/hatena.png");


	//// 赤
	//playerImage[PLAYER_RED] = LoadGraph("image/playerR_stop.png");
	//jumpImage[PLAYER_RED] = LoadGraph("image/playerR_jump.png");
	//stopJumpImage[PLAYER_RED] = LoadGraph("image/playerR_stop_jump.png");
	//LoadDivGraph("image/playerR_run.png", 10, 5, 2, 72, 72, runImage[PLAYER_RED], true);
	//LoadDivGraph("image/playerR_segway.png", 2, 2, 1, 72, 72, segweyImage[PLAYER_RED], true);
	//// 青
	//playerImage[PLAYER_BLUE] = LoadGraph("image/playerB_stop.png");
	//jumpImage[PLAYER_BLUE] = LoadGraph("image/playerB_jump.png");
	//stopJumpImage[PLAYER_BLUE] = LoadGraph("image/playerB_stop_jump.png");
	//LoadDivGraph("image/playerB_run.png", 10, 5, 2, 72, 72, runImage[PLAYER_BLUE], true);
	//LoadDivGraph("image/playerB_segway.png", 2, 2, 1, 72, 72, segweyImage[PLAYER_BLUE], true);


}

void PlayerGameInit(void)
{
	for (int j = 0; j < PLAYER_MAX; j++)
	{
		player[j].size = { 72,72 }; // プレイヤーの画像サイズ
		player[j].offsetSize = { -player[j].size.x / 2,-player[j].size.y };
		player[j].hitPosE = { 20,0 };
		player[j].hitPosS = { 20,62 };

		player[j].wallRunSpeed = player[j].moveSpeed;	// 壁での速度
		player[j].moveSpeed = PLAYER_SPEED_NORMAL;
		player[j].animCnt = 0;
		player[j].runFlag = false;
		player[j].jumpFlag = false;
		player[j].jumpCnt = 0;			// ｼﾞｬﾝﾌﾟできる回数
		player[j].segweyFlag = false;	// ｾｸﾞｳｪｲ

		player[j].wireOkFlag = false;
		player[j].wireFlag = false;

		player[j].visible = false;
		player[j].visible2 = false;
		player[j].imgLocCnt = 0;

		//player[j].velocity;
		//player[j].cnt;
		player[j].UpDownSpeed;		// 落下速度
		player[j].AddUpDownSpeed;	// 上下の加算量
	}
	player[0].type = CHARA_RED;		 // selectで初めに出てくるｷｬﾗ
	player[0].right = false;
	player[0].left = false;
	player[0].moveDir = DIR_RIGHT;
	player[0].pos = { CHIP_SIZE_X * 15, CHIP_SIZE_Y * 14 };// 初期位置
	//player[0].pos = {CHIP_SIZE_X * 5,SCREEN_SIZE_Y+CHIP_SIZE_Y * 13 - 25 };// 初期位置

	//player[1].type = CHARA_RED;		 // selectで初めに出てくるｷｬﾗ
	//player[1].right = false;
	//player[1].left = false;
	//player[1].moveDir = DIR_RIGHT;
	//player[1].pos = { CHIP_SIZE_X * 14, CHIP_SIZE_Y * 14 };// 初期位置


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
	player[0].nowDeg = 0.0f;
	furikoSpeed = 0.0f;
	defDeg = 0.0f;

	player[0].dropFlag = false;

	//rot = -rand() % 90;

}

void PlayerControl(void)
{
	switch (GetGameMode())
	{
	case GMODE_CHARASERE:	// ｷｬﾗｾﾚｸﾄ
		if (!player[0].visible)
		{
			if (trgKey[P1_RIGHT])
			{
				player[0].type = (CHARA_TYPE)(player[0].type + 1);
				if (player[0].type > CHARA_ORANGE)
				{
					player[0].type = CHARA_RED;
				}
			}
			if (trgKey[P1_LEFT])
			{
				player[0].type = (CHARA_TYPE)(player[0].type - 1);
				if (player[0].type < CHARA_RED)
				{
					player[0].type = CHARA_ORANGE;
				}
			}
		}
		else
		{
			player[0].animCnt++;
		}
		if (trgKey[P1_A]) player[0].visible = !player[0].visible;
		//if (trgKey[P1_UP]) player[0].visible = !player[0].visible;

		//if (!player[1].visible)
		//{
			//if (trgKey[P2_RIGHT])
			//{
			//	player[1].type = (CHARA_TYPE)(player[1].type + 1);
			//	if (player[1].type > CHARA_ORANGE)
			//	{
			//		player[1].type = CHARA_RED;
			//	}
			//}
			//if (trgKey[P2_LEFT])
			//{
			//	player[1].type = (CHARA_TYPE)(player[1].type - 1);
			//	if (player[1].type < CHARA_RED)
			//	{
			//		player[1].type = CHARA_ORANGE;
			//	}
			//}
			//else
			//{
			//	player[1].animCnt++;
			//}
			//if (trgKey[P2_UP]) player[1].visible = !player[1].visible;
		//}
		break;

	case GMODE_GAME:
		PlayerState();
		break;
	}
}

void PlayerDraw(void)
{
	switch (GetGameMode())
	{
	case GMODE_CHARASERE:
		DrawGraph(0, 120, p1Wak[0], true);
		if (player[0].visible)
		{
			// 大きく描画するためにDrawRotaGraph
			DrawRotaGraph(160, 300, 3, 0,
				charImage[player[0].type].runImage[(player[0].animCnt / 3) % 10], true);
			DrawString(120, 180, "キャラ決定！", 0x000000);

			//DrawRotaGraph(760, 300, 3, 0,
			//	charImage[player[1].type].runImage[(player[1].animCnt / 3) % 10], true);
			//DrawString(720, 180, "キャラ決定！", 0x000000);
		}
		else
		{
			DrawRotaGraph(160, 300, 3, 0, charImage[player[0].type].playerImage, true);
			DrawTurnGraph(0, 240, yazirusiImage[0], true);
			DrawGraph(260, 240, yazirusiImage[1], true);

			//DrawRotaGraph(760, 300, 3, 0, charImage[player[1].type].playerImage, true);
			//DrawTurnGraph(0, 240, yazirusiImage[0], true);
			//DrawGraph(260, 240, yazirusiImage[1], true);
		}
		//if (player[1].visible) // PL2
		//{
		//	// 大きく描画するためにDrawRotaGraph
		//	//DrawRotaGraph(760, 300, 3, 0,
		//	//	charImage[player[1].type].runImage[(player[1].animCnt / 3) % 10],
		//	//	true);
		//	//DrawString(720, 180, "キャラ決定！", 0x000000);
		//}
		//else
		//{
		//	//DrawRotaGraph(760, 300, 3, 0, charImage[player[1].type].playerImage, true);
		//}

		break;

	case GMODE_GAME:
		if (player[0].visible && !player[0].visible2)
		{
			int img = charImage[player[0].type].playerImage;
			//int img = playerImage[player.type];

			if ((player[0].runFlag) && (!player[0].jumpFlag))
				img = charImage[player[0].type].runImage[(player[0].animCnt / 3) % 10];
			//if ((player.runFlag) && (!player.jumpFlag)) 
			//	img = runImage[player.type][(player.animCnt / 3) % 10];

			if ((player[0].jumpFlag) && (player[0].runFlag)) img = charImage[player[0].type].jumpImage;
			//if ((player.jumpFlag) && (player.runFlag)) img = jumpImage[player.type];

			if ((player[0].jumpFlag) && (!player[0].runFlag)) img = charImage[player[0].type].stopJumpImage;
			//if ((player.jumpFlag) && (!player.runFlag)) img = stopJumpImage[player.type];

			//if (player.segweyFlag) img = segweyImage[player.type][(player.animCnt / 5) % 2];
			if (player[0].segweyFlag)img = segweyImage[player[0].type][(player[0].animCnt / 5) % 2];

			if (player_state == PLAYER_WALL_RIGHT)
			{
				DrawBox(player[0].pos.x - player[0].hitPosS.y - mapPos.x,
					player[0].pos.y - player[0].hitPosE.x - mapPos.y,
					player[0].pos.x - mapPos.x, player[0].pos.y + player[0].hitPosS.x - mapPos.y,
					0x00ffff, false);
			}
			else if (player_state == PLAYER_WALL_LEFT)
			{
				DrawBox(player[0].pos.x - mapPos.x, player[0].pos.y - player[0].hitPosE.x - mapPos.y,
					player[0].pos.x + player[0].hitPosS.y - mapPos.x,
					player[0].pos.y + player[0].hitPosS.x - mapPos.y, 0x00ffff, false);
			}

			if (player[0].moveDir == DIR_RIGHT)
			{
				if (player_state == PLAYER_WALL_RIGHT)
				{
					DrawRotaGraph(player[0].pos.x - 36 - mapPos.x, player[0].pos.y - mapPos.y,
						1, -PI / 2, img, true, false);
				}
				else
				{
					DrawGraph(player[0].pos.x + player[0].offsetSize.x - mapPos.x,
						player[0].pos.y + player[0].offsetSize.y - mapPos.y, img, true);
				}
			}
			else if (player[0].moveDir == DIR_LEFT)
			{
				if (player_state == PLAYER_WALL_LEFT)
				{
					DrawRotaGraph(player[0].pos.x + 36 - mapPos.x, player[0].pos.y - mapPos.y,
						1, PI / 2, img, true, true);
				}
				else
				{
					DrawTurnGraph(player[0].pos.x + player[0].offsetSize.x - mapPos.x,
						player[0].pos.y + player[0].offsetSize.y - mapPos.y, img, true);
				}
			}

			if ((player_state != PLAYER_WALL_RIGHT) && (player_state != PLAYER_WALL_LEFT))
			{
				DrawBox(player[0].pos.x - player[0].offsetSize.x - mapPos.x,
					player[0].pos.y + player[0].offsetSize.y - mapPos.y,
					player[0].pos.x + player[0].offsetSize.x - mapPos.x, player[0].pos.y - mapPos.y, 0xff0000, false);

				DrawBox(player[0].pos.x - player[0].hitPosS.x - mapPos.x,
					player[0].pos.y - player[0].hitPosS.y - mapPos.y,
					player[0].pos.x + player[0].hitPosE.x - mapPos.x, player[0].pos.y - mapPos.y, 0x00ffff, false);

				DrawLine(player[0].pos.x + player[0].offsetSize.x - mapPos.x,
					player[0].pos.y - (player[0].size.y / 2) - mapPos.y,
					player[0].pos.x - player[0].offsetSize.x - mapPos.x,
					player[0].pos.y - (player[0].size.y / 2) - mapPos.y, 0x00ffff, true);

				DrawLine(player[0].pos.x - mapPos.x, player[0].pos.y + player[0].offsetSize.y - mapPos.y,
					player[0].pos.x - mapPos.x, player[0].pos.y - mapPos.y, 0x00ffff, true);
			}
		}
		//DrawBox(player[0].pos.x - player[0].offsetSize.x - mapPos.x, player[0].pos.y - player[0].offsetSize.y - mapPos.y,
		//	player[0].pos.x + player[0].offsetSize.x - mapPos.x, player[0].pos.y + player[0].offsetSize.y - mapPos.y, 0xff0000, false);
		//DrawBox(player[0].pos.x - player[0].hitPosS.x - mapPos.x, player[0].pos.y - player[0].hitPosS.y - mapPos.y,
		//	player[0].pos.x + player[0].hitPosE.x - mapPos.x, player[0].pos.y + player[0].hitPosE.y - mapPos.y, 0x00ffff, false);
		//DrawLine(player[0].pos.x - player[0].offsetSize.x - mapPos.x, player[0].pos.y - mapPos.y,
		//	player[0].pos.x + player[0].offsetSize.x - mapPos.x, player[0].pos.y - mapPos.y, 0x00ffff, true);
		//DrawLine(player[0].pos.x - mapPos.x, player[0].pos.y - player[0].offsetSize.y - mapPos.y,
		//	player[0].pos.x - mapPos.x, player[0].pos.y + player[0].offsetSize.y - mapPos.y, 0x00ffff, true);

		if (!player[0].visible && player[0].visible2)
		{
			if (player[0].right)
			{
				// キャラクタをおもりとして描画
				DrawGraph(player[0].pos.x - mapPos.x - 56,
					player[0].pos.y - player[0].size.y - mapPos.y - 23,
					charImage[player[0].type].jumpImage, true);
			}
			else
			{
				// キャラクタをおもりとして描画
				DrawTurnGraph(player[0].pos.x - mapPos.x - 15,
					player[0].pos.y - player[0].size.y - mapPos.y - 23,
					charImage[player[0].type].jumpImage, true);
			}
		}
		if (player_state == PLAYER_W_PRE || player_state == PLAYER_W_ACTION)
		{
			// 指定ブロックがとりあえず自分より上にある状態なら
			if (player[0].wireOkFlag)
			{
				DrawLine(player[0].pos.x - mapPos.x, player[0].pos.y - player[0].size.y - mapPos.y,
					furiko_pos.x - mapPos.x, furiko_pos.y - mapPos.y, 0xffffffff, 2);		// ワイヤー
			}
			else
			{
				if (WirePreTimeCnt <= furiko_pos.y)
				{
					DrawLine(player[0].pos.x - mapPos.x, player[0].pos.y - player[0].size.y - mapPos.y,
						furiko_pos.x - mapPos.x, furiko_pos.y - mapPos.y, 0xffffffff, 2);	// ワイヤー
					WirePreTimeCnt++;
				}
			}
			// 指定ブロックが上にないとき(何か障害物にあたるまでは伸ばしきりたい)
		}
		// ｱｲｺﾝ
		DrawGraph(50, 40, p1Wak[1], true);
		DrawGraph(58, 74, charImage[0].iconImage, true);
		// アイテムが描画される枠
		DrawBox(20, 5, 86, 71, 0xffffff, true);
		DrawBox(20, 5, 86, 71, 0x000000, false);
		DrawGraph(20, 5, segwayICImage, true);

		// ｷｬﾗステータスが描画される枠
		DrawBox(100, 80, 200, 95, 0x000000, true);
		DrawBox(101, 81, 199, 94, 0x00ff00, true);

		//itemBoxDraw();
		if (itemBoxFlag == true)
		{
			DrawGraph(CHIP_SIZE_X * 15 - mapPos.x, CHIP_SIZE_X * 16 - mapPos.y, hatenaImage, true);// 左上1つ
			DrawGraph(SCREEN_SIZE_X - CHIP_SIZE_X * 3 - mapPos.x, SCREEN_SIZE_Y * 2 - CHIP_SIZE_X * 3 - mapPos.y, hatenaImage, true);// 下部の真ん中
			DrawGraph(SCREEN_SIZE_X * 2 - CHIP_SIZE_X * 10 - mapPos.x, SCREEN_SIZE_Y - mapPos.y, hatenaImage, true);// 左側真ん中
		}
		else
		{
			DrawString(SCREEN_SIZE_X / 2 - 40, SCREEN_SIZE_Y / 2 - 5, "HIT", 0x00000);
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
	//				//Disassembly_C(radian);			// x軸
	//				//Disassembly_S(radian);			// y軸
	//				player.visible = true;			// アニメーションするキャラが表示される
	//				player.visible2 = false;		// ワイヤー中の静止画キャラが非表示になる
	//				player.wireFlag = false;		// ワイヤーが非表示になる
	//				// 着地地点の描画位置
	//				player.pos.x = _pos.x + mapPos.x + player.offsetSize.x;
	//				//player.pos.y = _pos.y + mapPos.y;
	//				DrawGraph(Disassembly_C(radian), Disassembly_S(radian , _g), stopJumpImage[player.type], true);
	//				// ここに二段ジャンプ用処理の追加が必要かも
	//				PlayerDraw();
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
	//		OnMove(_pos.x, _pos.y, _v * sint, _v * cost);
	//		OnAdjust();		// これがないとひもが伸びていく
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

void OnMove(float& x, float& y, float vx, float vy)
{
	// 移動のための処理
	x += vx;
	y += vy;
}

void OnAdjust()
{
	//Vec2 v = (furiko_pos - _endPoint);
	//if (v.Length() > _length) {
	//	furiko_pos = _endPoint + v.normalized() * _length;
	//}
}

bool GetPlayerV(void)
{
	return player[0].visible;
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
	player[0].jumpFlag = false;
	player[0].runFlag = false;

	Position player_RD = { player[0].pos.x + player[0].moveSpeed + player[0].hitPosE.x , player[0].pos.y - 1 };
	Position player_LD = { player[0].pos.x - player[0].moveSpeed - player[0].hitPosS.x - 1 , player[0].pos.y - 1 };

	for (int j = 0; j < PLAYER_MAX; j++)
	{
		if (player[j].pos.y > SCREEN_SIZE_Y)mapPos.y += ACC_G / 3;	// 下がる
		if (player[j].pos.y < SCREEN_SIZE_Y + 200)mapPos.y -= ACC_G / 2;// 上がる
	}

	if (newKey[P1_RIGHT])
	{
		player[0].runFlag = true;
		player[0].moveDir = DIR_RIGHT;

		// 壁確認
		if ((WallBlockPass(player_RD)) && (player[0].wallRunSpeed == player[0].moveSpeed) && (!newKey[P1_A]))
		{
			player_state = PLAYER_WALL_RIGHT;
		}
		else
		{
			if (IsPass(player_RD))// RDのchipID取得して移動できるIDか調べる
			{
				player[0].pos.x += player[0].moveSpeed;
				if (player[0].pos.x > SCREEN_SIZE_X / 2)//カメラが右に行く
				{
					mapPos.x += player[0].moveSpeed;
				}
			}
			else
			{
				// 隣まで計算してplayer.pos.xに代入
				player[0].pos.x = GetWorldPos_Map(player_RD, DIR_LEFT).x - player[0].hitPosS.x;
			}
		}
	}

	if (newKey[P1_LEFT])
	{
		player[0].runFlag = true;
		player[0].moveDir = DIR_LEFT;

		// 壁確認
		if ((WallBlockPass(player_LD)) && (player[0].wallRunSpeed == player[0].moveSpeed) && (!newKey[P1_A]))
		{
			if (player[0].wallRunSpeed == player[0].moveSpeed)
			{
				player_state = PLAYER_WALL_LEFT;
			}
		}
		else
		{
			if (IsPass(player_LD))	// LDのchipID取得して移動できるIDか調べる
			{
				player[0].pos.x -= player[0].moveSpeed;
				if (player[0].pos.x < PLAY_SIZE_X - SCREEN_SIZE_X / 2)//カメラが左に行く
				{
					mapPos.x -= player[0].moveSpeed;
				}
			}
			else
			{
				// 隣まで計算してplayer.pos.xに代入
				player[0].pos.x = GetWorldPos_Map(player_LD, DIR_RIGHT).x + player[0].hitPosS.x;
			}
		}
	}

	if (trgKey[P2_UP])
	{
		if (player[0].moveDir == DIR_RIGHT)
		{
			player[0].right = true;
			player[0].left = false;
		}

		if (player[0].moveDir == DIR_LEFT)
		{
			player[0].right = false;
			player[0].left = true;
		}

		furiko_pos = { player[0].pos.x , player[0].pos.y - player[0].size.y };
		player[0].wireFlag = true;

		player_state = PLAYER_W_PRE;
	}

	// item当たり判定
	if ((player[0].dropFlag == false) && (itemBoxFlag == true))
	{
		if (player[0].pos.x - player[0].size.x / 2 + player[0].size.y < itemBoxPosX + CHIP_SIZE_X
			&& itemBoxPosX < player[0].pos.x - player[0].size.x / 2 + player[0].size.y
			&& player[0].pos.y - player[0].size.y < itemBoxPosY + CHIP_SIZE_Y
			&& itemBoxPosY < player[0].pos.y - player[0].size.y)
		{
			player[0].dropFlag = false;
			itemBoxFlag = false;// ここに入ればhitがでる
		}
	}
	if (itemBoxFlag == false)
	{
		itemBoxFlag = true;
	}

}

void PlJumpUp(void)
{
	Position player_RU = { player[0].pos.x + player[0].hitPosE.x - 1 ,
							player[0].pos.y - player[0].moveSpeed - player[0].hitPosS.y };	// 左上
	Position player_LU = { player[0].pos.x - player[0].hitPosS.x ,
							player[0].pos.y - player[0].moveSpeed - player[0].hitPosS.y };	// 右上

	player[0].UpDownSpeed += player[0].AddUpDownSpeed;

	//　RD と LD の y座標に-1して上のブロック情報を得る
	if (IsPass({ player_RU.x , player_RU.y - 1 }) && IsPass({ player_LU.x , player_LU.y - 1 }))
	{
		// 上に何もない
		player_state = PLAYER_JUMP_UP;
		player[0].jumpFlag = true;
	}
	else
	{
		// 上にブロックがある
		player[0].UpDownSpeed = DOWN_SPEED_DEF;
		player[0].AddUpDownSpeed = 0.5f;
		player_state = PLAYER_DOWN;
	}

	// 上昇の処理(ある一定値を超えそうになると落下に切り替わるようにする)
	if (player[0].UpDownSpeed <= 0.0f)
	{
		if (IsPass({ player_RU.x , (player_RU.y + player[0].UpDownSpeed) })
			&& IsPass({ player_LU.x , (player_LU.y + player[0].UpDownSpeed) }))
		{
			player[0].pos.y += player[0].UpDownSpeed;
		}
		else
		{
			player[0].pos.y = GetWorldPos_Map({ player_RU.x , (player_RU.y + player[0].UpDownSpeed) }, DIR_DOWN).y + player[0].hitPosS.y;
		}
	}
	else
	{
		player_state = PLAYER_DOWN;
	}
}

void PlDown(void)
{
	Position player_RD = { player[0].pos.x + player[0].hitPosE.x - 1, player[0].pos.y - 1 };		// 右下
	Position player_LD = { player[0].pos.x - player[0].hitPosS.x , player[0].pos.y - 1 };		// 左下

	//　RD と LD の y座標に+1して下のブロック情報を得る
	if (IsPass({ player_RD.x , player_RD.y + 1 }) && IsPass({ player_LD.x , player_LD.y + 1 }))
	{
		if (player_state == PLAYER_NORMAL)
		{
			player[0].UpDownSpeed = DOWN_SPEED_DEF;
			player[0].AddUpDownSpeed = 0.8f;
			player_state = PLAYER_DOWN;
		}
	}
	else
	{
		if (trgKey[P1_A])
		{
			player[0].UpDownSpeed = UP_SPEED_DEF;
			player[0].AddUpDownSpeed = 0.8f;
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
	player[0].UpDownSpeed += player[0].AddUpDownSpeed;

	// 落下処理
	if (IsPass({ player_RD.x , (player_RD.y + player[0].UpDownSpeed) })
		&& IsPass({ player_LD.x , (player_LD.y + player[0].UpDownSpeed) }))
	{
		player[0].pos.y += player[0].UpDownSpeed;
		player[0].jumpFlag = true;
	}
	else
	{
		player[0].pos.y = GetWorldPos_Map({ player_RD.x , (player_RD.y + player[0].UpDownSpeed) }, DIR_UP).y;
		player[0].wallRunSpeed = player[0].moveSpeed;
		player[0].jumpFlag = false;
		SetEffct(player[0].pos);
	}
}

void PlWirePrepare(void)
{
	furiko_pos.y = furiko_pos.y - ONEFRAME_WIRE_UP;

	Position furiko_RU = { furiko_pos.x + player[0].size.x / 2, furiko_pos.y };
	Position furiko_LU = { furiko_pos.x - player[0].size.x / 2, furiko_pos.y };
	if (player[0].pos.y - furiko_pos.y <= 300)	// 指定範囲
	{
		if (!(WireBlockPass({ furiko_RU.x , furiko_RU.y }))
			|| !(WireBlockPass({ furiko_LU.x , furiko_LU.y })))
		{
			// 長さを出す
			_length = furiko_pos - player[0].pos;


			// プレイヤーの座標を-90°で設定?
			player[0].pos.x = cos((-90.0f * PI) / 180.0f) * _length.y + furiko_pos.x;
			player[0].pos.y = sin((-90.0f * PI) / 180.0f) * _length.y + furiko_pos.y;

			MaxDeg = -20.0f;
			minDeg = -160.0f;

			player[0].nowDeg = -90.0f;

			defDeg = -90.0f;

			furikoSpeed = FURIKO_SPEED_DEF;

			// 最大
			//Maxrad.x = (-20.0f * PI / 180.0f) + furiko_pos.x;
			//Maxrad.y = (-20.0f * PI / 180.0f) + furiko_pos.y;
			//
			//Maxrad.x = (-20 * PI / 180) + _length.x + furiko_pos.x;
			//Maxrad.y = (-20 * PI / 180) + _length.y + furiko_pos.y;
			//
			// 最小
			//minrad.x = (-160.0f * PI / 180.0f) + furiko_pos.x;
			//minrad.y = (-160.0f * PI / 180.0f) + furiko_pos.y;
			//
			//minrad.x = (-160 * PI / 180) + _length.x + furiko_pos.x;
			//minrad.y = (-160 * PI / 180) + _length.y + furiko_pos.y;
			//

			// 指定範囲以内 & 指定ブロックがある
			// 指定ブロックにワイヤーを繋げる
			//player[0].pos.x = furiko_RU.x - mapPos.x;
			//player[0].pos.y = furiko_RU.y + player[0].size.y - mapPos.y;
			player[0].visible = false;
			player[0].visible2 = true;
			player[0].wireFlag = false;
			player[0].wireOkFlag = true;
			player_state = PLAYER_W_ACTION;
			return;		// 1つ目に見つけたブロックでいいのでreturnで抜ける
		}
		//else
		//{
		//	// 指定範囲外 & 指定ブロックがある
		//	//player.visible = true;
		//	//player.visible2 = false;
		//	WirePreTimeCnt = 0;
		//	player[0].wireFlag = false;
		//	player[0].wireOkFlag = false;
		//	player_state = PLAYER_NORMAL;
		//}		
	}
	else
	{
		// 背景以外がある
		WirePreTimeCnt = 0;
		//player.pos.x = furiko_RU.x - player.size.x / 2;
		//player.pos.y = furiko_RU.y + player.size.y  - mapPos.y;
		//player.visible = true;
		//player.visible2 = false;
		player[0].wireFlag = false;
		player[0].wireOkFlag = false;
		player_state = PLAYER_NORMAL;
	}

	//player.wireOkFlag = true;
	//player_state = PLAYER_Y_ACTION;
}


// 前と座標の中心点が変わっているから注意!!
void PlWireAction(void)
{
	if (player[0].wireOkFlag)
	{
		if (WireTimeCnt < 200)
		{
			if (newKey[P2_B])
			{
				player_state = PLAYER_W_JUMP;
				return;
			}
			//Vec2 v = (furiko_pos - player.pos);//振り子の支点から振り子の錘までのベクトル
			//v = v.normalized();//正規化する

			////外積と内積を利用して角度を計算
			//float cost = Dot(v, Vec2(-1, 0));
			//float sint = Cross(v, Vec2(-1, 0));
			//float theta = atan2f(cost, sint);

			//_v += _g * cost;

			//OnMove(player.pos.x, player.pos.y, _v * sint, _v * cost);
			//OnAdjust();		// これがないとひもが伸びていく

			AddRad();
			WireTimeCnt++;
		}
		else
		{
			player[0].wireFlag = false;
			player[0].wireOkFlag = false;
			player[0].visible = true;
			player[0].visible2 = false;
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

	player[0].pos.x = cos((player[0].nowDeg * PI) / 180.0f) * _length.y + furiko_pos.x;
	player[0].pos.y = sin((player[0].nowDeg * PI) / 180.0f) * _length.y + furiko_pos.y;

	if (player[0].right == true)
	{
		if (player[0].moveDir == DIR_RIGHT)
		{
			// プレイヤーが右向き
			furikoSpeed = furikoSpeed - FURIKO_ADD;							// 加速
			player[0].nowDeg = player[0].nowDeg - furikoSpeed;		// 1°ずつ引いてくやつ
			if (player[0].nowDeg >= defDeg)
			{
				player[0].moveDir = DIR_LEFT;
			}
		}
		else
		{
			// プレイヤーが左向き
			furikoSpeed = furikoSpeed + FURIKO_ADD;							// 減速
			player[0].nowDeg = player[0].nowDeg - furikoSpeed;		// 1°ずつ足してくやつ
			if (player[0].nowDeg <= defDeg)
			{
				player[0].moveDir = DIR_RIGHT;
			}
		}
	}

	if (player[0].left == true)
	{
		if (player[0].moveDir == DIR_RIGHT)
		{
			// プレイヤーが右向き
			furikoSpeed = furikoSpeed + FURIKO_ADD;							// 減速
			player[0].nowDeg = player[0].nowDeg - furikoSpeed;		// 1°ずつ引いてくやつ
			if (player[0].nowDeg <= defDeg)
			{
				player[0].moveDir = DIR_LEFT;
			}
		}
		else
		{
			// プレイヤーが左向き
			furikoSpeed = furikoSpeed - FURIKO_ADD;							// 加速
			player[0].nowDeg = player[0].nowDeg - furikoSpeed;		// 1°ずつ足してくやつ
			if (player[0].nowDeg >= defDeg)
			{
				player[0].moveDir = DIR_RIGHT;
			}
		}
	}
}


void PlWireJump(void)
{
	player[0].visible = true;			// アニメーションするキャラが表示される
	player[0].visible2 = false;		// ワイヤー中の静止画キャラが非表示になる
	player[0].wireFlag = false;		// ワイヤーが非表示になる

	// 着地地点の描画位置
	//if (player.moveDir == DIR_RIGHT)
	//{
	//	player.pos.x = furiko_pos.x + mapPos.x - player.size.x / 2;
	//}
	//else
	//{
	//	player.pos.x = furiko_pos.x - mapPos.x + player.size.x / 2;
	//}
	//player.pos.x = furiko_pos.x + mapPos.x - player.size.x / 2;
	//player.pos.y = furiko_pos.y + mapPos.y + player.size.y;
	player_state = PLAYER_NORMAL;
}

// 壁を走る
// 右壁
void PlWall_R(void)
{
	player[0].runFlag = false;
	player[0].jumpFlag = false;

	for (int j = 0; j < PLAYER_MAX; j++)
	{
		if (player[j].pos.y < SCREEN_SIZE_Y + 200)mapPos.y -= ACC_G / 2;// 上がる
	}

	// 位置修正
	player[0].pos.x = GetWorldPos_Map({ player[0].pos.x + player[0].moveSpeed + player[0].hitPosE.x , player[0].pos.y - 1 }, DIR_LEFT).x;

	Position player_RU = { player[0].pos.x - 1, player[0].pos.y - player[0].wallRunSpeed - player[0].hitPosE.x };

	// 専用の壁か？
	if (WallBlockPass(player[0].pos))
	{
		// もう走れない・途中で降りる
		if ((player[0].wallRunSpeed <= 0) || (newKey[P1_A]))
		{
			// 位置修正
			player[0].pos.x = GetWorldPos_Map(player_RU, DIR_LEFT).x;
			if (oldKey[P1_LEFT])
			{
				player[0].wallRunSpeed = player[0].moveSpeed;
				player_state = PLAYER_WALL_LEFT;
			}
			else
			{
				player_state = PLAYER_NORMAL;		// 通常に戻す
			}
		}
		else if (player[0].wallRunSpeed >= 0)
		{
			// 上へ
			if (newKey[P1_RIGHT])
			{
				player[0].runFlag = true;
				player[0].moveDir = DIR_RIGHT;
				// 壁確認
				if (IsPass(player_RU))
				{
					player[0].pos.y -= player[0].wallRunSpeed;
				}
				else
				{
					player[0].pos.y = GetWorldPos_Map(player_RU, DIR_DOWN).y + player[0].hitPosS.x;
				}
			}
			player[0].wallRunSpeed -= 0.1f;	// 減速
		}
	}
	else
	{
		player_state = PLAYER_NORMAL;		// 通常に戻す
	}
}

// 左壁
void PlWall_L(void)
{

	player[0].runFlag = false;
	player[0].jumpFlag = false;

	for (int j = 0; j < PLAYER_MAX; j++)
	{
		if (player[j].pos.y < SCREEN_SIZE_Y + 200)mapPos.y -= ACC_G / 2;// 上がる
	}

	// 位置修正
	player[0].pos.x = GetWorldPos_Map({ player[0].pos.x - player[0].moveSpeed - player[0].hitPosS.x - 1 , player[0].pos.y - 1 }, DIR_RIGHT).x;

	Position player_LU = { player[0].pos.x, player[0].pos.y - player[0].moveSpeed - player[0].hitPosE.x };

	// 専用の壁か？
	if (WallBlockPass({ player[0].pos.x - 1,player[0].pos.y }))
	{
		// もう走れない・途中で降りる
		if ((player[0].wallRunSpeed <= 0) || (newKey[P1_A]))
		{
			//位置修正
			player[0].pos.x = GetWorldPos_Map(player_LU, DIR_RIGHT).x;
			if (oldKey[P1_RIGHT])
			{
				player[0].wallRunSpeed = player[0].moveSpeed;
				// 右壁に変更
				player_state = PLAYER_WALL_RIGHT;
			}
			else
			{
				player_state = PLAYER_NORMAL;		// 通常に戻す
			}
		}
		else if (player[0].wallRunSpeed >= 0)
		{
			// 上へ
			if (newKey[P1_LEFT])
			{
				player[0].runFlag = true;
				player[0].moveDir = DIR_LEFT;
				// 壁確認
				if (IsPass(player_LU))
				{
					player[0].pos.y -= player[0].moveSpeed;
				}
				else
				{
					player[0].pos.y = GetWorldPos_Map(player_LU, DIR_DOWN).y + player[0].hitPosE.x;
				}
			}
			player[0].wallRunSpeed -= 0.1f;	// 減速
		}
	}
	else
	{
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
	player[0].animCnt++;
}

void ItemSegwey(void)
{
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