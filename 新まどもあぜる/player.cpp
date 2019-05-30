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

#define ONEFRAME_WIRE_UP	  30    // 1フレームで上がるワイヤーの速度

#define FURIKO_SPEED_DEF      10.0f	// 振り子の初速度
#define FURIKO_ADD			  0.4f	// 振り子の加減算速度

//#define PAI 3.141592
//#define JUMPSPEED 2

//PLAYER_STATE player_state;
//ITEM_STATE	 item_state;

// ﾌﾟﾚｲﾔｰｽﾋﾟｰﾄﾞ
enum PLAYER_SPEED {
	PLAYER_SPEED_NORMAL = 8,
	PLAYER_SPEED_SEGWEY = 12,
	PLAYER_SPEED_WIRE = 14
};



// 画像タイプ
enum IMAGE_TYPE {
	IMAGE_TYPE_STOP,	// 立ち止まっている画像
	IMAGE_TYPE_RUN,		// 走っている画像
	IMAGE_TYPE_ICON,	// 顔ｱｲｺﾝ
	IMAGE_TYPE_JUMP,
	IMAGE_TYPE_STOPJUMP,
	IMAGE_TYPE_SEGWAY,
	IMAGE_TYPE_MAX
};
#define RUN_ANIM_MAX  10			// ｺﾏ数
#define SEGWAY_ANIM_MAX 2

typedef struct {
	int playerImage;					// 止まった画像//int playerImage[2];
	int runImage[RUN_ANIM_MAX];			// 走っている画像//int runImage[2][10];
	int iconImage;						// 顔画像
	int jumpImage;						// ジャンプ画像//int jumpImage[2];
	int stopJumpImage;					// ｼﾞｬﾝﾌﾟしてから落ちる時//int stopJumpImage[2];
	int segwayImage[SEGWAY_ANIM_MAX];	// ｾｸﾞｳｪｲ画像		int segweyImage[2][2];

}CHARACTER_IMAGE;
CHARACTER_IMAGE charImage[CHARA_MAX];
CHARACTER player[PLAYER_MAX];



int p1Wak[2];
int p2Wak[2];
int p3Wak[2];
int p4Wak[2];
// 枠
int* pWak[PLAYER_MAX] = {
	p1Wak,p2Wak/*,p3Wak,p4Wak*/
};

int yazirusiImage[2];

//int WirePreTimeCnt;					// ワイヤーを少しずつ伸ばして途中で途切れるまでの時間
//int WireTimeCnt;					// ワイヤーの表示時間

bool _isPushJump;
bool _isJumped;

//Position furiko_pos;
float _g;
float _v;
//Position _length;

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
//float furikoSpeed;

float jumpSpeed;

float defDeg;

//int Segwey_Cnt;

// アイテム関連 
//int itemcnt;
bool itemBoxFlag = true;
int itemBoxPosX = CHIP_SIZE_X * 15 - mapPos.x;
int itemBoxPosY = CHIP_SIZE_Y * 16 - mapPos.y;
int hatenaImage;		// ？ﾎﾞｯｸｽ画像
int segweyImage_icon;
//int kabosuImage;
//int UFOImage;
//int specialImage;

// BGM & SE
int charsel_ok;
int charsel_no;
int charsel;
int item_get;
int seg;
int wire;

// 挟まった時の飛ばす方向
const Position hasamuJump[DIR_MAX] = { {-10,-10},{10,-10},{10,10},{-10,10} };	// 上・右・下・左

//float hpcnt;



typedef Position Vec2;


inline float Dot(const Vec2 & a, const Vec2 & b)
{
	return a.x* b.x + a.y * b.y;
}

inline float Cross(const Vec2 & a, const Vec2 & b)
{
	return a.x* b.y - b.x * a.y;
}

Position moved;
MOVE_DIR runDir;
void ScrollMap(Position pos, int speed, MOVE_DIR dir);
int PlayerTop(int number, Position pos, MOVE_DIR dir);

void PlayerSystmInit(void)
{
	p1Wak[0] = LoadGraph("image/p1CSWak.png");
	p1Wak[1] = LoadGraph("image/p1GWak.png");
	p2Wak[0] = LoadGraph("image/p2CSWak.png");
	p2Wak[1] = LoadGraph("image/p2GWak.png");
	p3Wak[0] = LoadGraph("image/p3CSWak.png");
	p3Wak[1] = LoadGraph("image/p3GWak.png");
	p4Wak[0] = LoadGraph("image/p4CSWak.png");
	p4Wak[1] = LoadGraph("image/p4GWak.png");
	for (int i = 0; i < 2; i++)
	{
		yazirusiImage[i] = LoadGraph("image/yazirusi.png");
	}
	const char *fileNameList[CHARA_MAX][IMAGE_TYPE_MAX] = {
	{"image/playerR_stop.png","image/playerR_run.png","image/R_icon.png","image/playerR_jump.png","image/playerR_stop_jump.png","image/playerR_segway.png"},	//0 red
	{"image/playerB_stop.png","image/playerB_run.png","image/B_icon.png","image/playerB_jump.png","image/playerB_stop_jump.png","image/playerB_segway.png"},	//1blue
	{"image/playerG_stop.png","image/playerG_run.png","image/G_icon.png","image/playerG_jump.png","image/playerG_stop_jump.png","image/playerG_segway.png"},
	{"image/playerP_stop.png","image/playerP_run.png","image/P_icon.png","image/playerP_jump.png","image/playerP_stop_jump.png","image/playerP_segway.png"},
	{"image/playerO_stop.png","image/playerO_run.png","image/O_icon.png","image/playerO_jump.png","image/playerO_stop_jump.png","image/playerO_segway.png"},
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
		LoadDivGraph(fileNameList[j][IMAGE_TYPE_SEGWAY],
			SEGWAY_ANIM_MAX,			// 全体枚数	
			SEGWAY_ANIM_MAX, SEGWAY_ANIM_MAX / 2,			// 横ｺﾏ数、縦ｺﾏ数
			72, 72,							// 縦サイズ、横ｻｲｽﾞ
			&charImage[j].segwayImage[0]);
	}

	// アイテム関連
	hatenaImage = LoadGraph("image/hatena.png");
	segweyImage_icon = LoadGraph("image/segway.png");
	//kabosuImage = LoadGraph("image/kabosu.png");
	//UFOImage = LoadGraph("image/UFOIC.png");
	//specialImage = LoadGraph("image/hatena.png");

	// BGM & SE
	charsel_ok = LoadSoundMem("BGM/charok_se.mp3");
	charsel_no = LoadSoundMem("BGM/charno_se.mp3");
	charsel = LoadSoundMem("BGM/charsel_se.mp3");
	item_get = LoadSoundMem("BGM/itemget_se.mp3");
	seg = LoadSoundMem("BGM/car-horn1.mp3");
	wire = LoadSoundMem("BGM/wirehook_se.mp3");

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
		player[j].BlockFlag = false;

		player[j].wireOkFlag = false;
		player[j].wireFlag = false;
		player[j].dropFlag = false;

		player[j].visible = false;
		player[j].visible2 = false;
		player[j].imgLocCnt = 0;

		//player[j].velocity;
		//player[j].cnt;
		player[j].UpDownSpeed;		// 落下速度
		player[j].AddUpDownSpeed;	// 上下の加算量
	}
	// PL1
	player[0].type = CHARA_RED;		 // selectで初めに出てくるｷｬﾗ
	player[0].right = false;
	player[0].left = false;
	player[0].moveDir = DIR_RIGHT;
	player[0].pos = { CHIP_SIZE_X * 15, CHIP_SIZE_Y * 14 };// 初期位置
	player[0].nowDeg = 0.0f;
	player[0].JumpDeg = 0;
	// PL2
	player[1].type = CHARA_BLUE;		 // selectで初めに出てくるｷｬﾗ
	player[1].right = false;
	player[1].left = false;
	player[1].moveDir = DIR_RIGHT;
	player[1].pos = { CHIP_SIZE_X * 16, CHIP_SIZE_Y * 14 };// 初期位置
	player[1].nowDeg = 0.0f;
	player[1].JumpDeg = 0;
	// PL3
	//player[2].type = CHARA_ORANGE;		 // selectで初めに出てくるｷｬﾗ
	//player[2].right = false;
	//player[2].left = false;
	//player[2].moveDir = DIR_RIGHT;
	//player[2].pos = { CHIP_SIZE_X * 17, CHIP_SIZE_Y * 14 };// 初期位置
	//player[2].nowDeg = 0.0f;
	//player[2].JumpDeg = 0;
	// PL4
	//player[3].type = CHARA_GREEN;		 // selectで初めに出てくるｷｬﾗ
	//player[3].right = false;
	//player[3].left = false;
	//player[3].moveDir = DIR_RIGHT;
	//player[3].pos = { CHIP_SIZE_X * 18, CHIP_SIZE_Y * 14 };// 初期位置
	//player[3].nowDeg = 0.0f;
	//player[3].JumpDeg = 0;


	//ひもの支点の初期化
	_v = 0;			// 振り子のふり幅

	player[0].WireTimeCnt = 0;
	player[1].WireTimeCnt = 0;

	_g = 2.0f;		//重力の定義
	player[0]._length = { 0 , 0 };	//紐の長さの計算
	player[1]._length = { 0 , 0 };	//紐の長さの計算



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

	player[0].state = PLAYER_NORMAL;
	player[1].state = PLAYER_NORMAL;

	player[0].item_state = ITEM_NON;
	player[1].item_state = ITEM_NON;


	player[0].WirePreTimeCnt = 0;
	player[1].WirePreTimeCnt = 0;


	//Maxrad = { 0,0 };
	//minrad = { 0,0 };

	MaxDeg = 0.0f;
	minDeg = 0.0f;
	//player[0].nowDeg = 0.0f;
	player[0].furikoSpeed = 0.0f;
	player[1].furikoSpeed = 0.0f;

	jumpSpeed = 0.0f;
	defDeg = 0.0f;

	player[0].itemcnt = 0;
	player[1].itemcnt = 0;

	//player[0].JumpDeg = 0;

	player[0].Segwey_Cnt = 0;
	player[1].Segwey_Cnt = 0;

	player[0].hpcnt = 0.0;
	player[1].hpcnt = 0.0;

	runDir = DIR_DOWN;

}

void PlayerControl(int padNo)
{
	//KeyCheck(padNo);
	switch (GetGameMode())
	{
	case GMODE_CHARASERE:	// ｷｬﾗｾﾚｸﾄ
		if (!player[padNo].visible)
		{
			if (pad[padNo].trgKey[PAD_TBL_RIGHT])
			{
				PlaySoundMem(charsel, DX_PLAYTYPE_BACK, true);

				player[padNo].type = (CHARA_TYPE)(player[padNo].type + 1);
				if (player[padNo].type > CHARA_ORANGE)
				{
					player[padNo].type = CHARA_RED;
				}
			}
			if (pad[padNo].trgKey[PAD_TBL_LEFT])
			{
				PlaySoundMem(charsel, DX_PLAYTYPE_BACK, true);

				player[padNo].type = (CHARA_TYPE)(player[padNo].type - 1);
				if (player[padNo].type < CHARA_RED)
				{
					player[padNo].type = CHARA_ORANGE;
				}
			}
		}
		//else if ((pad[0].trgKey[PAD_TBL_JUMP]) && (pad[1].trgKey[PAD_TBL_JUMP])
		//	&& (player[0].type != player[1].type))
		//{
		//	player[padNo].animCnt++;
		//}
		else
		{
			player[padNo].animCnt++;
		}

		if (pad[padNo].trgKey[PAD_TBL_JUMP])
		{
			//player[padNo].visible = !player[padNo].visible;
			if (!player[padNo].visible)
			{
				PlaySoundMem(charsel_ok, DX_PLAYTYPE_BACK, true);
				player[padNo].visible = true;
			}
			else
			{
				PlaySoundMem(charsel_no, DX_PLAYTYPE_BACK, true);
				player[padNo].visible = false;
			}
		}
		break;

	case GMODE_GAME:
		moved = player[padNo].pos;
		PlayerState(padNo);
		ItemState(padNo);

		// ｶﾒﾗ
		if (padNo == PlayerTop(padNo, player[padNo].pos, runDir)) {
			if (player[padNo].visible && !player[padNo].visible2) {
				if ((player[padNo].moveDir == DIR_LEFT)&&(runDir == DIR_LEFT)) {
					ScrollMap(player[padNo].pos, moved.x - player[padNo].pos.x, DIR_LEFT);
					if (mapPos.x <= 0) runDir = DIR_DOWN;
				}
				if ((player[padNo].pos.y > moved.y)&&(runDir==DIR_DOWN)) {
					ScrollMap(player[padNo].pos, player[padNo].pos.y - moved.y, DIR_DOWN);
					if (mapPos.y >= PLAY_SIZE_Y - (SCREEN_SIZE_Y - (CHIP_SIZE_Y * 2))) runDir = DIR_RIGHT;
				}
				if ((player[padNo].moveDir == DIR_RIGHT) && (runDir == DIR_RIGHT)) {
					ScrollMap(player[padNo].pos, player[padNo].pos.x - moved.x, DIR_RIGHT);
					if (mapPos.x >= PLAY_SIZE_X - SCREEN_SIZE_X) runDir = DIR_UP;
				}
				if ((player[padNo].pos.y < moved.y) && (runDir==DIR_UP)) {
					ScrollMap(player[padNo].pos, moved.y - player[padNo].pos.y, DIR_UP);
					if (mapPos.y <= 0) runDir = DIR_LEFT;
				}
			}
		}
		break;
	}

	// アイテム取得
	if ((player[padNo].dropFlag == false) && (itemBoxFlag == true))
	{
		if (player[padNo].pos.x - player[padNo].hitPosS.x < itemBoxPosX + CHIP_SIZE_X		    // player左 < box右	
			&& itemBoxPosX < player[padNo].pos.x + player[padNo].hitPosS.x					// box左    < player右
			&& player[padNo].pos.y - player[padNo].size.y < itemBoxPosY + CHIP_SIZE_Y			// player上 < box下
			&& itemBoxPosY < player[padNo].pos.y)										// box上    < player下
		{
			player[padNo].item_state = ITEM_SEGWEY;
			player[padNo].dropFlag = true;												// ここをtrueにしているので、アイテムを使ってフラグをfalseにするまで次のアイテムは取得できない
			PlaySoundMem(item_get, DX_PLAYTYPE_BACK, true);
			itemBoxFlag = false;												// ここに入ればhitがでる
		}
	}
}

void PlayerDraw(int padNo)
{
	switch (GetGameMode())
	{
	case GMODE_CHARASERE:
		
		for (int j = 0; j < PLAYER_MAX; j++)
		{
			UIDrawSel(j);
		}

		break;

	case GMODE_GAME:
		if (player[padNo].visible && !player[padNo].visible2)
		{
			int img = charImage[player[padNo].type].playerImage;
			//int img = playerImage[player.type];

			if ((player[padNo].runFlag) && (!player[padNo].jumpFlag))
			{
				img = charImage[player[padNo].type].runImage[(player[padNo].animCnt / 3) % 10];
			}
			//if ((player.runFlag) && (!player.jumpFlag)) 
			//	img = runImage[player.type][(player.animCnt / 3) % 10];

			if ((player[padNo].jumpFlag) && (player[padNo].runFlag))
			{
				img = charImage[player[padNo].type].jumpImage;
			}
			//if ((player.jumpFlag) && (player.runFlag)) img = jumpImage[player.type];

			if ((player[padNo].jumpFlag) && (!player[padNo].runFlag))
			{
				img = charImage[player[padNo].type].stopJumpImage;
			}
			//if ((player.jumpFlag) && (!player.runFlag)) img = stopJumpImage[player.type];

			if (player[padNo].segweyFlag)
			{
				img = charImage[player[padNo].type].segwayImage[(player[padNo].animCnt / 5) % 2];
			}
			//if (player.segweyFlag) img = segweyImage[player.type][(player.animCnt / 5) % 2];

			if (player[padNo].state == PLAYER_WALL_RIGHT)
			{
				DrawBox(player[padNo].pos.x - player[padNo].hitPosS.y - mapPos.x,
					player[padNo].pos.y - player[padNo].hitPosE.x - mapPos.y,
					player[padNo].pos.x - mapPos.x, player[padNo].pos.y + player[padNo].hitPosS.x - mapPos.y,
					0x00ffff, false);
			}
			else if (player[padNo].state == PLAYER_WALL_LEFT)
			{
				DrawBox(player[padNo].pos.x - mapPos.x,
					player[padNo].pos.y - player[padNo].hitPosE.x - mapPos.y,
					player[padNo].pos.x + player[padNo].hitPosS.y - mapPos.x,
					player[padNo].pos.y + player[padNo].hitPosS.x - mapPos.y, 0x00ffff, false);
			}

			if (player[padNo].moveDir == DIR_RIGHT)
			{
				if (player[padNo].state == PLAYER_WALL_RIGHT)
				{
					DrawRotaGraph(player[padNo].pos.x - 36 - mapPos.x, player[padNo].pos.y - mapPos.y,
						1, -PI / 2, img, true, false);
				}
				else
				{
					DrawGraph(player[padNo].pos.x + player[padNo].offsetSize.x - mapPos.x,
						player[padNo].pos.y + player[padNo].offsetSize.y - mapPos.y, img, true);
				}
			}
			else if (player[padNo].moveDir == DIR_LEFT)
			{
				if (player[padNo].state == PLAYER_WALL_LEFT)
				{
					DrawRotaGraph(player[padNo].pos.x + 36 - mapPos.x, player[padNo].pos.y - mapPos.y,
						1, PI / 2, img, true, true);
				}
				else
				{
					DrawTurnGraph(player[padNo].pos.x + player[padNo].offsetSize.x - mapPos.x,
						player[padNo].pos.y + player[padNo].offsetSize.y - mapPos.y, img, true);
				}
			}

			if ((player[padNo].state != PLAYER_WALL_RIGHT) && (player[padNo].state != PLAYER_WALL_LEFT))
			{
				DrawBox(player[padNo].pos.x - player[padNo].offsetSize.x - mapPos.x,
					player[padNo].pos.y + player[padNo].offsetSize.y - mapPos.y,
					player[padNo].pos.x + player[padNo].offsetSize.x - mapPos.x,
					player[padNo].pos.y - mapPos.y, 0xff0000, false);

				DrawBox(player[padNo].pos.x - player[padNo].hitPosS.x - mapPos.x,
					player[padNo].pos.y - player[padNo].hitPosS.y - mapPos.y,
					player[padNo].pos.x + player[padNo].hitPosE.x - mapPos.x,
					player[padNo].pos.y - mapPos.y, 0x00ffff, false);

				DrawLine(player[padNo].pos.x + player[padNo].offsetSize.x - mapPos.x,
					player[padNo].pos.y - (player[padNo].size.y / 2) - mapPos.y,
					player[padNo].pos.x - player[padNo].offsetSize.x - mapPos.x,
					player[padNo].pos.y - (player[padNo].size.y / 2) - mapPos.y, 0x00ffff, true);

				DrawLine(player[padNo].pos.x - mapPos.x,
					player[padNo].pos.y + player[padNo].offsetSize.y - mapPos.y,
					player[padNo].pos.x - mapPos.x, player[padNo].pos.y - mapPos.y, 0x00ffff, true);
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

		if (!player[padNo].visible && player[padNo].visible2)
		{
			if (player[padNo].right)
			{
				// キャラクタをおもりとして描画
				DrawGraph(player[padNo].pos.x - mapPos.x - 56,
					player[padNo].pos.y - player[padNo].size.y - mapPos.y - 23,
					charImage[player[padNo].type].jumpImage, true);
			}
			else
			{
				// キャラクタをおもりとして描画
				DrawTurnGraph(player[padNo].pos.x - mapPos.x - 15,
					player[padNo].pos.y - player[padNo].size.y - mapPos.y - 23,
					charImage[player[padNo].type].jumpImage, true);
			}
		}
		if (player[padNo].state == PLAYER_W_PRE || player[padNo].state == PLAYER_W_ACTION)
		{
			// 指定ブロックがとりあえず自分より上にある状態なら
			if (player[padNo].wireOkFlag)
			{
				// ワイヤー
				DrawLine(player[padNo].pos.x - mapPos.x,
					player[padNo].pos.y - player[padNo].size.y - mapPos.y,
					player[padNo].furiko_pos.x - mapPos.x, player[padNo].furiko_pos.y - mapPos.y, 0xffffffff, 2);
			}
			else
			{
				if (player[padNo].WirePreTimeCnt <= player[padNo].furiko_pos.y)
				{
					// ワイヤー
					DrawLine(player[padNo].pos.x - mapPos.x,
						player[padNo].pos.y - player[padNo].size.y - mapPos.y,
						player[padNo].furiko_pos.x - mapPos.x, player[padNo].furiko_pos.y - mapPos.y, 0xffffffff, 2);
					player[padNo].WirePreTimeCnt++;
				}
			}
			// 指定ブロックが上にないとき(何か障害物にあたるまでは伸ばしきりたい)
		}

		for (int j = 0; j < PLAYER_MAX; j++)
		{
			UIDraw(j);
		}	

		//itemBoxDraw();
		if (itemBoxFlag == true)
		{
			DrawGraph(CHIP_SIZE_X * 15 - mapPos.x, CHIP_SIZE_X * 16 - mapPos.y, hatenaImage, true);// 左上1つ
			DrawGraph(SCREEN_SIZE_X - CHIP_SIZE_X * 3 - mapPos.x,
				SCREEN_SIZE_Y * 2 - CHIP_SIZE_X * 3 - mapPos.y, hatenaImage, true);// 下部の真ん中
			DrawGraph(SCREEN_SIZE_X * 2 - CHIP_SIZE_X * 10 - mapPos.x,
				SCREEN_SIZE_Y - mapPos.y, hatenaImage, true);// 左側真ん中
			player[padNo].itemcnt = 0;

		}
		else
		{
			if (player[padNo].itemcnt == 200)
			{
				itemBoxFlag = true;
				player[padNo].itemcnt = 0;
			}
			else
			{
				player[padNo].itemcnt++;
			}
			//DrawString(SCREEN_SIZE_X / 2 - 40, SCREEN_SIZE_Y / 2 - 5, "HIT", 0xffffff);
		}


		DrawFormatString(0, 20, 0xff0000, "top : %d", PlayerTop(padNo, player[padNo].pos, DIR_RIGHT));
		DrawFormatString(0, 40, 0xff0000, "runDir : %d", runDir);

		break;
	}
}

void UIDraw(int padNo)
{
	

	// ｱｲｺﾝ
	// オフセット( 画面上で一番左に描画される <所持アイテムが描画される枠> を基準に設定 )
	int offset_x[PLAYER_MAX] = {
	20,220/*,420,620*/
	};

	int offset_y[PLAYER_MAX] = {
	5,5/*,5,5*/
	};

	// 枠
	int* pWak[PLAYER_MAX] = {
		p1Wak,p2Wak/*,p3Wak,p4Wak*/
	};

	// プレイヤーステータスの枠
	DrawGraph(30 + offset_x[padNo], 35 + offset_y[padNo], pWak[padNo][1], true);

	// プレイヤーステータスのキャラアイコン
	DrawGraph(38 + offset_x[padNo], 69 + offset_y[padNo], charImage[player[padNo].type].iconImage, true);

	// 所持アイテムが描画される枠
	DrawBox(offset_x[padNo], offset_y[padNo], 66 + offset_x[padNo], 66 + offset_y[padNo], 0xffffff, true);
	DrawBox(offset_x[padNo], offset_y[padNo], 66 + offset_x[padNo], 66 + offset_y[padNo], 0x000000, false);

	// プレイヤーステータスのHPバーの本体 → カメラの外にでたら削れていく

	// プレイヤーのposがカメラの中にあるかどうかで判定したい
	if (player[padNo].pos.x - player[padNo].hitPosE.x < mapPos.x
		|| player[padNo].pos.x - player[padNo].hitPosE.x >= mapPos.x + SCREEN_SIZE_X
		|| player[padNo].pos.y - player[padNo].hitPosS.y < mapPos.y
		|| player[padNo].pos.y - player[padNo].hitPosS.y >= mapPos.y + SCREEN_SIZE_Y)
	{
		if (player[padNo].hpcnt > 98.0f)
		{
			player[padNo].hpcnt = 98.0f;
			player[padNo].state = PLAYER_DEATH;
		}
		else
		{
			player[padNo].hpcnt = player[padNo].hpcnt + 0.1f;
			player[padNo].flydir = hasamuJump[runDir];	// 最初に飛ばす方向
			player[padNo].state = PLAYER_HASAMU_JUMP;
		}
	}

	// プレイヤーステータスのHPバーの枠
	DrawBox(80 + offset_x[padNo], 75 + offset_y[padNo], 180 + offset_x[padNo], 90 + offset_y[padNo], 0x000000, true);
	DrawBox(81 + offset_x[padNo], 76 + offset_y[padNo], 179 + offset_x[padNo] - player[padNo].hpcnt, 89 + offset_y[padNo], 0x00ff00, true);


	

	//DrawBox(81 + offset_x[padNo], 76 + offset_y[padNo], 179 + offset_x[padNo] /*- hpcnt*/, 89 + offset_y[padNo], 0x00ff00, true);

	// プレイヤーのアイテムドロップ
	if (player[padNo].dropFlag == true)
	{
		DrawGraph(offset_x[padNo], 5 + offset_y[padNo], segweyImage_icon, true);
	}
}

void UIDrawSel(int padNo)
{
	int selOffset_x[PLAYER_MAX] = {
		0,600,//0,600,
	};
	int selOffset_y[PLAYER_MAX] = {
		120,120,//420,420,
	};

	// ｷｬﾗｾﾚｸﾄ時のプレイヤーの枠
	DrawGraph(selOffset_x[padNo], selOffset_y[padNo], pWak[padNo][0], true);

	if (player[padNo].visible)
	{
		// 決定
		// ｷｬﾗ決定！
		DrawString(120 + selOffset_x[padNo], 60 + selOffset_y[padNo], "キャラ決定！", 0x000000);
		//　キャラクター
		DrawRotaGraph(160 + selOffset_x[padNo], 180 + selOffset_y[padNo], 3, 0,
			charImage[player[padNo].type].runImage[(player[padNo].animCnt / 3) % 10], true);
	}
	else
	{
		// 選択中
		// 矢印
		DrawTurnGraph(selOffset_x[padNo], 120 + selOffset_y[padNo], yazirusiImage[0], true);
		DrawGraph(260 + selOffset_x[padNo], 120 + selOffset_y[padNo], yazirusiImage[1], true);

		// キャラクター
		DrawRotaGraph(160 + selOffset_x[padNo], 180 + selOffset_y[padNo],
			3, 0, charImage[player[padNo].type].playerImage, true);
	}
}

bool GetPlayerV(void)
{
	return player[0].visible;
}

void PlNormal(int padNo)
{
	//KeyCheck(padNo);
	player[padNo].jumpFlag = false;
	player[padNo].runFlag = false;

	Position player_RD = { player[padNo].pos.x + player[padNo].moveSpeed + player[padNo].hitPosE.x , player[padNo].pos.y - 1 };
	Position player_LD = { player[padNo].pos.x - player[padNo].moveSpeed - player[padNo].hitPosS.x - 1 , player[padNo].pos.y - 1 };

	if (pad[padNo].newKey[PAD_TBL_RIGHT])
	{
		player[padNo].runFlag = true;
		player[padNo].moveDir = DIR_RIGHT;

		//// 壁確認
		//if ((WallBlockPass(player_RD)) && (player[padNo].wallRunSpeed == player[padNo].moveSpeed) /*&& !(pad[padNo].oldKey[PAD_TBL_JUMP])*/)
		//{
		//	player[padNo].state = PLAYER_WALL_RIGHT;
		//}
		//else
		//{
		if (IsPass(player_RD))// RDのchipID取得して移動できるIDか調べる
		{
			player[padNo].pos.x += player[padNo].moveSpeed;
		}
		else
		{
			// 隣まで計算してplayer.pos.xに代入
			player[padNo].pos.x = GetWorldPos_Map(player_RD, DIR_LEFT).x - player[padNo].hitPosS.x;
		}

	}

	if (pad[padNo].newKey[PAD_TBL_LEFT])
	{
		player[padNo].runFlag = true;
		player[padNo].moveDir = DIR_LEFT;

		//// 壁確認
		//if ((WallBlockPass(player_LD)) && (player[padNo].wallRunSpeed == player[padNo].moveSpeed) /*&& !(pad[padNo].oldKey[PAD_TBL_JUMP])*/)
		//{
		//	if (player[padNo].wallRunSpeed == player[padNo].moveSpeed)
		//	{
		//		player[padNo].state = PLAYER_WALL_LEFT;
		//	}
		//}
		//else
		//{
		if (IsPass(player_LD))	// LDのchipID取得して移動できるIDか調べる
		{
			player[padNo].pos.x -= player[padNo].moveSpeed;
		}
		else
		{
			// 隣まで計算してplayer.pos.xに代入
			player[padNo].pos.x = GetWorldPos_Map(player_LD, DIR_RIGHT).x + player[padNo].hitPosS.x;
		}

	}

	if (pad[padNo].trgKey[PAD_TBL_WIRE])
	{
		if (player[padNo].moveDir == DIR_RIGHT)
		{
			player[padNo].right = true;
			player[padNo].left = false;
		}

		if (player[padNo].moveDir == DIR_LEFT)
		{
			player[padNo].right = false;
			player[padNo].left = true;
		}
		player[padNo].furiko_pos = { player[padNo].pos.x , player[padNo].pos.y - player[padNo].size.y };
		player[padNo].wireFlag = true;

		player[padNo].state = PLAYER_W_PRE;
	}
}

void PlJumpUp(int padNo)
{
	Position player_RU = { player[padNo].pos.x + player[padNo].hitPosE.x - 1 ,
							player[padNo].pos.y - player[padNo].moveSpeed - player[padNo].hitPosS.y };	// 左上
	Position player_LU = { player[padNo].pos.x - player[padNo].hitPosS.x ,
							player[padNo].pos.y - player[padNo].moveSpeed - player[padNo].hitPosS.y };	// 右上

	player[padNo].UpDownSpeed += player[padNo].AddUpDownSpeed;

	//　RD と LD の y座標に-1して上のブロック情報を得る
	if (IsPass({ player_RU.x , player_RU.y - 1 }) && IsPass({ player_LU.x , player_LU.y - 1 }))
	{
		// 上に何もない
		player[padNo].state = PLAYER_JUMP_UP;
		player[padNo].jumpFlag = true;
	}
	else
	{
		// 上にブロックがある
		player[padNo].UpDownSpeed = DOWN_SPEED_DEF;
		player[padNo].AddUpDownSpeed = 0.5f;
		player[padNo].state = PLAYER_DOWN;
	}

	// 上昇の処理(ある一定値を超えそうになると落下に切り替わるようにする)
	if (player[padNo].UpDownSpeed <= 0.0f)
	{
		if (IsPass({ player_RU.x , (player_RU.y + player[padNo].UpDownSpeed) })
			&& IsPass({ player_LU.x , (player_LU.y + player[padNo].UpDownSpeed) }))
		{
			player[padNo].pos.y += player[padNo].UpDownSpeed;
		}
		else
		{
			player[padNo].pos.y = GetWorldPos_Map({ player_RU.x , (player_RU.y + player[padNo].UpDownSpeed) }, DIR_DOWN).y + player[padNo].hitPosS.y;
		}
	}
	else
	{
		player[padNo].state = PLAYER_DOWN;
	}
}

void PlDown(int padNo)
{
	Position player_RD = { player[padNo].pos.x + player[padNo].hitPosE.x - 1, player[padNo].pos.y - 1 };	// 右下
	Position player_LD = { player[padNo].pos.x - player[padNo].hitPosS.x , player[padNo].pos.y - 1 };		// 左下

	//　RD と LD の y座標に+1して下のブロック情報を得る
	if (IsPass({ player_RD.x , player_RD.y + 1 }) && IsPass({ player_LD.x , player_LD.y + 1 }))
	{
		if (player[padNo].state == PLAYER_NORMAL)
		{
			player[padNo].UpDownSpeed = DOWN_SPEED_DEF;
			player[padNo].AddUpDownSpeed = 0.8f;
			player[padNo].state = PLAYER_DOWN;
		}
	}
	else
	{
		if (pad[padNo].trgKey[PAD_TBL_JUMP])
		{
			player[padNo].UpDownSpeed = UP_SPEED_DEF;
			player[padNo].AddUpDownSpeed = 0.8f;
			player[padNo].state = PLAYER_JUMP_UP;
		}
		else
		{
			if (player[padNo].state != PLAYER_W_ACTION)
			{
				player[padNo].state = PLAYER_NORMAL;
			}
		}
		return;
	}
	player[padNo].UpDownSpeed += player[padNo].AddUpDownSpeed;

	// 落下処理
	if (IsPass({ player_RD.x , (player_RD.y + player[padNo].UpDownSpeed) })
		&& IsPass({ player_LD.x , (player_LD.y + player[padNo].UpDownSpeed) }))
	{
		player[padNo].pos.y += player[padNo].UpDownSpeed;
		player[padNo].jumpFlag = true;
	}
	else
	{
		player[padNo].pos.y = GetWorldPos_Map({ player_RD.x , (player_RD.y + player[padNo].UpDownSpeed) }, DIR_UP).y;
		player[padNo].wallRunSpeed = player[padNo].moveSpeed;
		player[padNo].jumpFlag = false;
		SetEffect(player[padNo].pos);
	}
}

void PlWirePrepare(int padNo)
{
	player[padNo].furiko_pos.y = player[padNo].furiko_pos.y - ONEFRAME_WIRE_UP;

	Position furiko_RU = { player[padNo].furiko_pos.x + player[padNo].size.x / 2, player[padNo].furiko_pos.y };
	Position furiko_LU = { player[padNo].furiko_pos.x - player[padNo].size.x / 2, player[padNo].furiko_pos.y };
	if (player[padNo].pos.y - player[padNo].furiko_pos.y <= 350 && player[padNo].pos.y > 350)	// 指定範囲 && 上のほうでmap外に出たらエラーがでるのを防ぐ
	{
		PlaySoundMem(wire, DX_PLAYTYPE_BACK, true);
		if (!(WireBlockPass({ furiko_RU.x , furiko_RU.y }))
			|| !(WireBlockPass({ furiko_LU.x , furiko_LU.y })))
		{
			// 長さを出す
			player[padNo]._length = player[padNo].furiko_pos - player[padNo].pos;

			// プレイヤーの座標を-90°で設定?
			player[padNo].pos.x = cos((-90.0f * PI) / 180.0f) * player[padNo]._length.y + player[padNo].furiko_pos.x;
			player[padNo].pos.y = sin((-90.0f * PI) / 180.0f) * player[padNo]._length.y + player[padNo].furiko_pos.y;

			MaxDeg = -20.0f;
			minDeg = -160.0f;

			player[padNo].nowDeg = -90.0f;

			defDeg = -90.0f;

			player[padNo].furikoSpeed = FURIKO_SPEED_DEF;

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
			player[padNo].visible = false;
			player[padNo].visible2 = true;
			player[padNo].wireFlag = false;
			player[padNo].wireOkFlag = true;
			player[padNo].state = PLAYER_W_ACTION;
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
		player[padNo].WirePreTimeCnt = 0;
		//player.pos.x = furiko_RU.x - player.size.x / 2;
		//player.pos.y = furiko_RU.y + player.size.y  - mapPos.y;
		//player.visible = true;
		//player.visible2 = false;
		player[padNo].wireFlag = false;
		player[padNo].wireOkFlag = false;
		player[padNo].state = PLAYER_NORMAL;
	}

	//player.wireOkFlag = true;
	//player_state = PLAYER_Y_ACTION;
}

// 前と座標の中心点が変わっているから注意!!
void PlWireAction(int padNo)
{
	if (player[padNo].wireOkFlag)
	{
		if (player[padNo].WireTimeCnt < 500)
		{
			if ((pad[padNo].trgKey[PAD_TBL_ITEM]))// ワイヤーを伸ばしてぶらぶらしている間はアイテムは使えないようにする処理
			{
				pad[padNo].trgKey[PAD_TBL_ITEM] = !pad[padNo].trgKey[PAD_TBL_ITEM];
			}

			if (pad[padNo].trgKey[PAD_TBL_WIRE])
			{
				if (!IsPass({ player[padNo].pos.x , player[padNo].pos.y - player[padNo].size.y }))			// ここでそもそも壁の中でワイヤーを切り離したときの処置を書いとく
				{
					player[padNo].pos.y = GetWorldPos_Map(player[padNo].pos, DIR_DOWN).y + player[padNo].hitPosS.y + player[padNo].size.y * 2;		// 要調整
					player[padNo].pos.x = player[padNo].furiko_pos.x;
					player[padNo].visible = true;
					player[padNo].visible2 = false;
					player[padNo].state = PLAYER_NORMAL;
					return;
				}
				jumpSpeed = FURIKO_SPEED_DEF;
				player[padNo].BlockFlag = true;
				player[padNo].state = PLAYER_W_JUMP;
				return;
			}
			AddRad(padNo);
			player[padNo].WireTimeCnt++;
		}
		else
		{
			player[padNo].wireFlag = false;
			player[padNo].wireOkFlag = false;
			player[padNo].visible = true;
			player[padNo].visible2 = false;
			player[padNo].state = PLAYER_NORMAL;

			// ワイヤー表示時間の初期化
			player[padNo].WireTimeCnt = 0;

			// 振り子スタート位置の初期化
			_v = 0;
			player[padNo].furiko_pos = { 0,0 };

			player[padNo].furikoSpeed = FURIKO_SPEED_DEF;
		}
	}
}

void AddRad(int padNo)
{
	// 角度を足すんだよぉぉぉぉ!!!!!!

	player[padNo].pos.x = cos((player[padNo].nowDeg * PI) / 180.0f) * player[padNo]._length.y + player[padNo].furiko_pos.x;
	player[padNo].pos.y = sin((player[padNo].nowDeg * PI) / 180.0f) * player[padNo]._length.y + player[padNo].furiko_pos.y;

	if (player[padNo].right == true)
	{
		if (player[padNo].moveDir == DIR_RIGHT)
		{
			// プレイヤーが右向き
			player[padNo].furikoSpeed = player[padNo].furikoSpeed - FURIKO_ADD;							// 加速
			player[padNo].nowDeg = player[padNo].nowDeg - player[padNo].furikoSpeed;		// 1°ずつ引いてくやつ
			if (player[padNo].nowDeg >= defDeg)
			{
				player[padNo].moveDir = DIR_LEFT;
			}
		}
		else
		{
			// プレイヤーが左向き
			player[padNo].furikoSpeed = player[padNo].furikoSpeed + FURIKO_ADD;							// 減速
			player[padNo].nowDeg = player[padNo].nowDeg - player[padNo].furikoSpeed;		// 1°ずつ足してくやつ
			if (player[padNo].nowDeg <= defDeg)
			{
				player[padNo].moveDir = DIR_RIGHT;
			}
		}
	}

	if (player[padNo].left == true)
	{
		if (player[padNo].moveDir == DIR_RIGHT)
		{
			// プレイヤーが右向き
			player[padNo].furikoSpeed = player[padNo].furikoSpeed + FURIKO_ADD;							// 減速
			player[padNo].nowDeg = player[padNo].nowDeg - player[padNo].furikoSpeed;		// 1°ずつ引いてくやつ
			if (player[padNo].nowDeg <= defDeg)
			{
				player[padNo].moveDir = DIR_LEFT;
			}
		}
		else
		{
			// プレイヤーが左向き
			player[padNo].furikoSpeed = player[padNo].furikoSpeed - FURIKO_ADD;							// 加速
			player[padNo].nowDeg = player[padNo].nowDeg - player[padNo].furikoSpeed;		// 1°ずつ足してくやつ
			if (player[padNo].nowDeg >= defDeg)
			{
				player[padNo].moveDir = DIR_RIGHT;
			}
		}
	}
}

void PlWireJump(int padNo)
{
	if (pad[padNo].trgKey[PAD_TBL_WIRE])
	{
		if (player[padNo].moveDir == DIR_RIGHT)
		{
			player[padNo].right = true;
			player[padNo].left = false;
		}
		if (player[padNo].moveDir == DIR_LEFT)
		{
			player[padNo].right = false;
			player[padNo].left = true;
		}
		player[padNo].furiko_pos = { player[padNo].pos.x , player[padNo].pos.y - player[padNo].size.y };
		player[padNo].wireFlag = true;
		player[padNo].state = PLAYER_W_PRE;
	}

	Position player_RU = { player[padNo].pos.x + player[padNo].hitPosE.x - 1 ,
							player[padNo].pos.y - player[padNo].moveSpeed - player[padNo].hitPosS.y };	// 右上
	Position player_LU = { player[padNo].pos.x - player[padNo].hitPosS.x ,
							player[padNo].pos.y - player[padNo].moveSpeed - player[padNo].hitPosS.y };		// 左上

	Position player_RD = { player[padNo].pos.x + player[padNo].hitPosE.x - 1,
							player[padNo].pos.y - 1 };										// 右下
	Position player_LD = { player[padNo].pos.x - player[padNo].hitPosS.x ,
							player[padNo].pos.y - 1 };										// 左下

	player[padNo].visible = true;			// アニメーションするキャラが表示される
	player[padNo].visible2 = false;		// ワイヤー中の静止画キャラが非表示になる
	player[padNo].wireFlag = false;		// ワイヤーが非表示になる
	//player.JumpDeg = 180 - (player.nowDeg + 90);		// 打ち出し角

	if (player[padNo].right == true)
	{
		player[padNo].moveDir = DIR_RIGHT;
		player[padNo].JumpDeg = 180 - (player[padNo].nowDeg + 90);		// 打ち出し角
		if (player[padNo].BlockFlag == true)
		{
			player[padNo].pos.x += PLAYER_SPEED_WIRE;
			if (player[padNo].pos.y >= player[padNo].JumpDeg - player[padNo].pos.y)
			{
				player[padNo].pos.y -= jumpSpeed;
			}
			//if (player[padNo].pos.x > SCREEN_SIZE_X / 2)//カメラが右に行く
			//{
			//	mapPos.x += PLAYER_SPEED_WIRE;
			//}
			jumpSpeed = jumpSpeed - FURIKO_ADD;
		}
	}

	if (player[padNo].left == true)
	{
		player[padNo].moveDir = DIR_LEFT;
		player[padNo].JumpDeg = -(180 - (player[padNo].nowDeg + 90));		// 打ち出し角
		if (player[padNo].BlockFlag == true)
		{
			player[padNo].pos.x -= PLAYER_SPEED_WIRE;
			if (player[padNo].pos.y >= player[padNo].JumpDeg - player[padNo].pos.y)
			{
				player[padNo].pos.y -= jumpSpeed;
			}
			//if (player[padNo].pos.x < PLAY_SIZE_X - SCREEN_SIZE_X / 2)//カメラが左に行く
			//{
			//	mapPos.x -= PLAYER_SPEED_WIRE;
			//}
			jumpSpeed = jumpSpeed - FURIKO_ADD;
		}
	}

	if (!IsPass({ player_RU.x , player_RU.y }) || !IsPass({ player_LU.x , player_LU.y }) || !IsPass({ player_RD.x , player_RD.y }) || !IsPass({ player_LD.x , player_LD.y }))
	{
		player[padNo].BlockFlag = false;
	}

	if (player[padNo].BlockFlag == false)
	{
		if (IsPass({ player_RU.x , player_RU.y }) || IsPass({ player_LU.x , player_LU.y }) || IsPass({ player_RD.x , player_RD.y }) || IsPass({ player_LD.x , player_LD.y }))
		{
			player[padNo].state = PLAYER_DOWN;
		}
	}
}

void PlWall_Check(int padNo)
{
	Position player_RD = { player[padNo].pos.x + player[padNo].moveSpeed + player[padNo].hitPosE.x , player[padNo].pos.y - 1 };
	Position player_LD = { player[padNo].pos.x - player[padNo].moveSpeed - player[padNo].hitPosS.x - 1 , player[padNo].pos.y - 1 };

	// 壁確認
	if ((WallBlockPass(player_RD)) && (player[padNo].wallRunSpeed == player[padNo].moveSpeed) && !(pad[padNo].oldKey[PAD_TBL_JUMP]))
	{
		player[padNo].state = PLAYER_WALL_RIGHT;
	}


	// 壁確認
	if ((WallBlockPass(player_LD)) && (player[padNo].wallRunSpeed == player[padNo].moveSpeed) && !(pad[padNo].oldKey[PAD_TBL_JUMP]))
	{
		if (player[padNo].wallRunSpeed == player[padNo].moveSpeed)
		{
			player[padNo].state = PLAYER_WALL_LEFT;
		}
	}

}

// 壁を走る
// 右壁
void PlWall_R(int padNo)
{
	player[padNo].runFlag = false;
	player[padNo].jumpFlag = false;


	// 位置修正
	player[padNo].pos.x = GetWorldPos_Map({ player[padNo].pos.x + player[padNo].moveSpeed + player[padNo].hitPosE.x , player[padNo].pos.y - 1 }, DIR_LEFT).x;

	Position player_RU = { player[padNo].pos.x - 1,
		player[padNo].pos.y - player[padNo].wallRunSpeed - player[padNo].hitPosE.x };

	// 専用の壁か？
	if (WallBlockPass(player[padNo].pos))
	{
		// もう走れない・途中で降りる
		if ((player[padNo].wallRunSpeed <= 0) || (pad[padNo].newKey[PAD_TBL_JUMP]))
		{
			// 位置修正
			player[padNo].pos.x = GetWorldPos_Map(player_RU, DIR_LEFT).x;
			if (pad[padNo].oldKey[PAD_TBL_LEFT])
			{
				player[padNo].wallRunSpeed = player[padNo].moveSpeed;
				player[padNo].state = PLAYER_WALL_LEFT;
			}
			else
			{
				player[padNo].state = PLAYER_NORMAL;		// 通常に戻す
			}
		}
		else if (player[padNo].wallRunSpeed >= 0)
		{
			// 上へ
			if (pad[padNo].newKey[PAD_TBL_RIGHT])
			{
				player[padNo].runFlag = true;
				player[padNo].moveDir = DIR_RIGHT;
				// 壁確認
				if (IsPass(player_RU))
				{
					player[padNo].pos.y -= player[padNo].wallRunSpeed;
				}
				else
				{
					player[padNo].pos.y = GetWorldPos_Map(player_RU, DIR_DOWN).y + player[padNo].hitPosS.x;
				}
			}
			player[padNo].wallRunSpeed -= 0.1f;	// 減速
		}
	}
	else
	{
		player[padNo].state = PLAYER_NORMAL;		// 通常に戻す
	}
}

// 左壁
void PlWall_L(int padNo)
{
	player[padNo].runFlag = false;
	player[padNo].jumpFlag = false;

	// 位置修正
	player[padNo].pos.x = GetWorldPos_Map({ player[padNo].pos.x - player[padNo].moveSpeed - player[padNo].hitPosS.x - 1 , player[padNo].pos.y - 1 }, DIR_RIGHT).x;

	Position player_LU = { player[padNo].pos.x,
		player[padNo].pos.y - player[padNo].moveSpeed - player[padNo].hitPosE.x };

	// 専用の壁か？
	if (WallBlockPass({ player[padNo].pos.x - 1,player[padNo].pos.y }))
	{
		// もう走れない・途中で降りる
		if ((player[padNo].wallRunSpeed <= 0) || (pad[padNo].newKey[PAD_TBL_JUMP]))
		{
			//位置修正
			player[padNo].pos.x = GetWorldPos_Map(player_LU, DIR_RIGHT).x;
			if (pad[padNo].oldKey[PAD_TBL_RIGHT])
			{
				player[padNo].wallRunSpeed = player[padNo].moveSpeed;
				// 右壁に変更
				player[padNo].state = PLAYER_WALL_RIGHT;
			}
			else
			{
				player[padNo].state = PLAYER_NORMAL;		// 通常に戻す
			}
		}
		else if (player[padNo].wallRunSpeed >= 0)
		{
			// 上へ
			if (pad[padNo].newKey[PAD_TBL_LEFT])
			{
				player[padNo].runFlag = true;
				player[padNo].moveDir = DIR_LEFT;
				// 壁確認
				if (IsPass(player_LU))
				{
					player[padNo].pos.y -= player[padNo].moveSpeed;
				}
				else
				{
					player[padNo].pos.y = GetWorldPos_Map(player_LU, DIR_DOWN).y + player[padNo].hitPosE.x;
				}
			}
			player[padNo].wallRunSpeed -= 0.1f;	// 減速
		}
	}
	else
	{
		// 通常に戻す
		player[padNo].state = PLAYER_NORMAL;
	}
}

void PlHasamuJump(int padNo)
{
	// 画面上に当たった時の反射
	if (player[padNo].pos.y - player[padNo].hitPosS.y <= mapPos.y)
	{
		runDir = DIR_DOWN;
	}

	// 画面右に当たった時の反射
	if (player[padNo].pos.x + player[padNo].hitPosS.x >= mapPos.x + SCREEN_SIZE_X)
	{
		runDir = DIR_LEFT;

	}

	// 画面下に当たった時の反射
	if (player[padNo].pos.y + player[padNo].hitPosS.y >= mapPos.y + SCREEN_SIZE_Y)
	{
		runDir = DIR_UP;
	}

	// 画面左に当たった時の反射
	if (player[padNo].pos.x + player[padNo].hitPosS.x <= mapPos.x)
	{
		runDir = DIR_RIGHT;
	}
	player[padNo].pos.x = player[padNo].pos.x + player[padNo].flydir.x;
	player[padNo].pos.y = player[padNo].pos.y + player[padNo].flydir.y;

}

void Pl_Death(int padNo)
{
	player[padNo].visible = false;
	player[padNo].visible2 = false;
	if ((pad[padNo].trgKey[PAD_TBL_ITEM]))// ワイヤーを伸ばしてぶらぶらしている間はアイテムは使えないようにする処理
	{
		pad[padNo].trgKey[PAD_TBL_ITEM] = !pad[padNo].trgKey[PAD_TBL_ITEM];

	}


}


void PlayerState(int padNo)
{
	switch (player[padNo].state)
	{
	case PLAYER_DOWN:	        // ジャンプ下降
		PlWall_Check(padNo);
	case PLAYER_NORMAL:			// 左右移動 
		PlNormal(padNo);
		if (player[padNo].state != PLAYER_W_PRE)
		{
			PlDown(padNo);
		}
		break;
	case PLAYER_JUMP_UP:		// ジャンプの上昇
		PlNormal(padNo);
		if (player[padNo].state != PLAYER_W_PRE)
		{
			PlJumpUp(padNo);
		}
		PlWall_Check(padNo);
		break;
	case PLAYER_W_PRE:
		PlWirePrepare(padNo);		// ﾜｲﾔｰｱｸｼｮﾝの準備(ワイヤーを伸ばせるか判定)
		break;
	case PLAYER_W_ACTION:		// ﾜｲﾔｰｱｸｼｮﾝ
		PlWireAction(padNo);
		break;
	case PLAYER_W_JUMP:			// ﾜｲﾔｰｼﾞｬﾝﾌﾟ
		PlWireJump(padNo);
		PlWall_Check(padNo);
		break;
	case PLAYER_WALL_RIGHT:		// 右壁
		PlWall_R(padNo);
		break;
	case PLAYER_WALL_LEFT:		// 左壁
		PlWall_L(padNo);
		break;
	case PLAYER_HASAMU_JUMP:
		PlHasamuJump(padNo);
		break;
	case PLAYER_DEATH:
		Pl_Death(padNo);
		break;
	default:
		break;
	}
	player[padNo].animCnt++;
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

void ItemSegwey(int padNo)
{
	if (player[padNo].state != PLAYER_WALL_RIGHT && player[padNo].state != PLAYER_WALL_LEFT)
	{
		if (pad[padNo].trgKey[PAD_TBL_ITEM])
		{
			player[padNo].segweyFlag = true;
			PlaySoundMem(seg, DX_PLAYTYPE_BACK, true);
		}
	}


	if (player[padNo].segweyFlag == true)
	{
		player[padNo].moveSpeed = PLAYER_SPEED_SEGWEY;

		if (player[padNo].Segwey_Cnt == 80)
		{
			player[padNo].segweyFlag = false;
			player[padNo].dropFlag = false;
			player[padNo].Segwey_Cnt = 0;
			player[padNo].moveSpeed = PLAYER_SPEED_NORMAL;
			player[padNo].item_state = ITEM_NON;
		}
		else
		{
			player[padNo].Segwey_Cnt++;
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

void ItemState(int padNo)
{
	switch (player[padNo].item_state)
	{
	case ITEM_NON:
		//PlNormal();
		break;
	case ITEM_SEGWEY:
		ItemSegwey(padNo);
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

void ScrollMap(Position pos, int speed, MOVE_DIR dir)
{
	switch (dir)
	{
	case DIR_UP:
		if ((mapPos.x <= 0) || (mapPos.x >= PLAY_SIZE_X - SCREEN_SIZE_X)) {
			if (pos.y <= mapPos.y + (SCREEN_SIZE_Y - SCREEN_SIZE_Y / 4)) {
				mapPos.y -= speed;
			}
		}
		break;
	case DIR_RIGHT:
		if ((mapPos.y <= 0) || (mapPos.y >= PLAY_SIZE_Y - SCREEN_SIZE_Y)) {
			if (pos.x > SCREEN_SIZE_X / 2) {
				mapPos.x += speed;
			}
		}
		break;
	case DIR_DOWN:
		if ((mapPos.x <= 0) || (mapPos.x >= PLAY_SIZE_X - SCREEN_SIZE_X)) {
			if (pos.y >= SCREEN_SIZE_Y - (SCREEN_SIZE_Y / 4)) {
				mapPos.y += speed;	// 下がる
			}
		}
		break;
	case DIR_LEFT:
		if ((mapPos.y <= 0) || (mapPos.y >= PLAY_SIZE_Y - SCREEN_SIZE_Y)) {
			if (pos.x < mapPos.x + SCREEN_SIZE_X / 2) {
				mapPos.x -= speed;
			}
		}
		break;
	default:
		break;
	}

}

int PlayerTop(int number, Position pos, MOVE_DIR dir)
{
	int top = false;
	switch (dir)
	{
	case DIR_UP:
		for (int j = 0; j < PLAYER_MAX; j++) {
			if ((number != j) && (pos.y < GetPlayerPos(j).y)) {
				top = number;
			}
		}
		break;
	case DIR_RIGHT:
		for (int j = 0; j < PLAYER_MAX; j++) {
			if ((number != j) && (pos.x > GetPlayerPos(j).x)) {
				top = number;
			}
		}
		break;
	case DIR_DOWN:
		for (int j = 0; j < PLAYER_MAX; j++) {
			if ((number != j) && (pos.y > GetPlayerPos(j).y)) {
				top = number;
			}
		}
		break;
	case DIR_LEFT:
		for (int j = 0; j < PLAYER_MAX; j++) {
			if ((number != j) && (pos.x < GetPlayerPos(j).x)) {
				top = number;
			}
		}
		break;
	default:
		break;
	}

	return top;
}

Position GetPlayerPos(int padNo)
{
	return player[padNo].pos;
}