#include "Dxlib.h"	//DxLibﾗｲﾌﾞﾗﾘを使用する　独自で準備したﾍｯﾀﾞｰﾌｧｲﾙは""で指定する
#include <vector>
#include <math.h>	// 標準ﾍｯﾀﾞｰﾌｧｲﾙは<>で指定する
#include "main.h"
#include "player.h"
#include "stage.h"

struct DataHeader {
	unsigned char identifier[4];
	unsigned int  size;				// サイズ
	unsigned int  mapWidth;			// 横幅
	unsigned int  mapHeight;		// 縦幅
	unsigned char chipWidth;		// チップ横幅
	unsigned char chipHeight;		// チップ縦幅
	unsigned char layerCount;		// レイヤー数
	unsigned char bitCount;			// 8bit
};
#define MAP_FILE_ID "FMF_"

enum MAP_ID {
	MAP_ID_CAGUHT,		 // ワイヤーが引っかかるところ
	MAP_ID_UG,			 // 地中
	MAP_ID_WALL,		 // 主な壁、地面
	MAP_ID_WALL_KICK,	 // 壁蹴りジャンプ
	MAP_ID_BACK,	 	 // 背景
	MAP_ID_MAX
};

DataHeader expData;

std::vector<MAP_ID>mapDataBase; // データを入れる箱
std::vector<MAP_ID*>mapData; // アドレスを入れれる箱

int mapImage[5];
int mapImage2[5];

int hatenaImage;

void StageSystmInit(void)
{
	//LoadDivGraph("image/block.png", 5, 5, 1, CHIP_SIZE_X, CHIP_SIZE_Y, mapImage);
	LoadDivGraph("image/back.png", 5, 5, 1, CHIP_SIZE_X, CHIP_SIZE_Y, mapImage2);
	hatenaImage = LoadGraph("image/hatena.png");
}

void StageGameInit(void)
{
	// ﾏｯﾌﾟ座標
	mapPos.x = 0;
	mapPos.y = 0;

	//ItemHatenaDraw();
	MapLoad();
}

void StageControl(void)
{
	// カメラ
	// X方向　左右マップ外が出ないように
	if (mapPos.x < 0) mapPos.x = 0;
	if (mapPos.x > MAP_X*CHIP_SIZE_X - SCREEN_SIZE_X) mapPos.x
		= MAP_X * CHIP_SIZE_X - SCREEN_SIZE_X;

	//// y方向　上下マップ外に出ないように
	if (mapPos.y < 0) mapPos.y = 0;
	if (mapPos.y > MAP_Y*CHIP_SIZE_Y - SCREEN_SIZE_Y) mapPos.y
		= MAP_Y * CHIP_SIZE_Y - SCREEN_SIZE_Y;
}

void StageDraw(void)
{	
	for (int y = 0; y < expData.mapHeight;y++)
	{
		for (int x = 0; x < expData.mapWidth; x++)
		{
			DrawGraph(x*expData.chipWidth - mapPos.x,y*expData.chipHeight - mapPos.y,mapImage2[mapData[y][x]],true);
		}
	}
	DrawGraph(SCREEN_SIZE_X / 2 - mapPos.x, SCREEN_SIZE_Y + 500 - mapPos.y, hatenaImage, true);

	//for (int x = 0; x < MAP_X; x++) {
	//	for (int y = 0; y < MAP_Y; y++) {
	//		DrawGraph(x*CHIP_SIZE_X - mapPos.x, y*CHIP_SIZE_Y - mapPos.y, mapImage[mapData[y][x]], true);
	//	}
	//}	

	//for (int x = 0; x < MAP_X; x++) {
	//	for (int y = 0; y < MAP_Y; y++) {
	//		//DrawBox(50 * x - mapPos.x, 50 * y, 50 + (50 * x) - mapPos.x, 50 + (50 * y), 0xffffff, false);
	//		//DrawFormatString(x * CHIP_SIZE_X + 10, CHIP_SIZE_Y * y + 2, 0xffffff, "%02d", mapData[y][x]);
	//	}
	//}
}


bool MapLoad(void)
{
	FILE*file;
	fopen_s(&file, "MapData/map2.fmf", "rb");
	fread(&expData, sizeof(expData), 1,file);		// ヘッダー部分読み込み

	mapDataBase.resize(expData.mapWidth*expData.mapHeight);
	for (int j = 0; j < expData.mapWidth*expData.mapHeight; j++)
	{
		fread(&mapDataBase[j], sizeof(unsigned char), 1, file); // 1個のデータ取り出す
	}
	fclose(file);

	mapData.resize(expData.mapHeight);

	for (int j = 0; j < expData.mapHeight; j++)
	{
		mapData[j] = &mapDataBase[expData.mapWidth*j];
	}


   	return true;
}

XY MapPosToIndex(XY pos)	// pos = mvedPos
{
	XY mapIndex;

	mapIndex = { pos.x / CHIP_SIZE_X, pos.y / CHIP_SIZE_Y };

	return mapIndex;
}

bool IsPass(XY pos)
{
	bool ret = true;
	int mapNo;
	bool blueblock = true;		// ワイヤーをかけれるブロックがあるかの判定: true = ない　false = ある
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = mapData[mapIndex.y][mapIndex.x];

	switch (mapNo) // enumに
	{
	case MAP_ID::MAP_ID_CAGUHT:		// ワイヤーが引っかかるところ

	case MAP_ID::MAP_ID_UG:			// 地中
	case MAP_ID::MAP_ID_WALL:		// 主な壁、地面
	case MAP_ID::MAP_ID_WALL_KICK:	// 壁蹴りジャンプ
		blueblock = false;			// ブロックがあるのでくっつけたい
		break;
	case MAP_ID::MAP_ID_BACK:		  // 背景

		ret = false;
		break;
	}
	return blueblock;
	return ret;
}

// ワイヤー指定

//bool WireBlockPass(XY pos)
//{
//	bool blueblock = true;		// ワイヤーをかけれるブロックがあるかの判定: true = ない　false = ある
//	int mapNo;
//	XY mapIndex;
//
//	mapIndex = MapPosToIndex(pos);
//	mapNo = mapData[mapIndex.y][mapIndex.x];
//
//	switch (mapNo)
//	{
//	case 0:
//	case 1:						// 青色ブロック
//		blueblock = false;		// ブロックがあるのでくっつけたい
//		break;
//	case 2:
//	case 3:
//		break;
//	}
//	return blueblock;
//}

// 壁にぴったりくっつくようにする
XY MapIndexToPos(XY index)
{
	XY mapPos;

	mapPos = { index.x * CHIP_SIZE_X, index.y * CHIP_SIZE_Y };

	return mapPos;
}

XY MapPos(XY pos, MOVE_DIR der) {
	XY tmpIndex;
	tmpIndex = MapPosToIndex(pos);
	switch (der)
	{
	case DIR_UP:
		tmpIndex.y;
		break;
	case DIR_RIGHT:
		tmpIndex.x++;
		break;
	case DIR_DOWN:
		tmpIndex.y++;
		break;
	case DIR_LEFT:
		tmpIndex.x;
		break;
	}
	return MapIndexToPos(tmpIndex);
}
