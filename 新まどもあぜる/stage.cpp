#include "Dxlib.h"	//DxLibײ���؂��g�p����@�Ǝ��ŏ�������ͯ�ް̧�ق�""�Ŏw�肷��
#include <vector>
#include <math.h>	// �W��ͯ�ް̧�ق�<>�Ŏw�肷��
#include "main.h"
#include "player.h"
#include "stage.h"

struct DataHeader {
	unsigned char identifier[4];
	unsigned int  size;				// �T�C�Y
	unsigned int  mapWidth;			// ���� 76
	unsigned int  mapHeight;		// �c�� 52
	unsigned char chipWidth;		// �`�b�v����
	unsigned char chipHeight;		// �`�b�v�c��
	unsigned char layerCount;		// ���C���[��
	unsigned char bitCount;			// 8bit
};
#define MAP_FILE_ID "FMF_"

DataHeader expData;

std::vector<MAP_ID>mapDataBase; // �f�[�^�����锠
std::vector<MAP_ID*>mapData; // �A�h���X������锠

//int mapImage[5];
int mapImage2[5];

//int hatenaImage;

// �w�i
int bgImage1;
int bgPosX1 = 0;				// ���T�C�Y1
int bgPosX2 = -PLAY_SIZE_X;	// ���T�C�Y2
int bgImage_gr;

void StageSystmInit(void)
{
	LoadDivGraph("image/back.png", 5, 5, 1, CHIP_SIZE_X, CHIP_SIZE_Y, mapImage2);
	bgImage_gr = LoadGraph("image/map1_gr.png");
	bgImage1 = LoadGraph("image/nback.png");
}

void StageGameInit(void)
{
	// ϯ�ߍ��W
	mapPos.x = 0;
	mapPos.y = 0;

	MapLoad();
}

void StageControl(void)
{
	// X����
	if (mapPos.x < 0)
	{
		mapPos.x = 0;
	}
	if (mapPos.x > expData.mapWidth *CHIP_SIZE_X - SCREEN_SIZE_X)
	{
		mapPos.x = expData.mapWidth * CHIP_SIZE_X - SCREEN_SIZE_X;
	}
	//// y����
	if (mapPos.y < 0)
	{
		mapPos.y = 0;
	}
	if (mapPos.y > expData.mapHeight*CHIP_SIZE_Y - SCREEN_SIZE_Y)
	{
		mapPos.y = expData.mapHeight * CHIP_SIZE_Y - SCREEN_SIZE_Y;
	}
}

void BgControl(void)
{
	// �w�i
	bgPosX1 += 1;
	if (bgPosX1 >= PLAY_SIZE_X)
	{
		bgPosX1 = -PLAY_SIZE_X;
	}
	bgPosX2 += 1;
	if (bgPosX2 >= PLAY_SIZE_X)
	{
		bgPosX2 = -PLAY_SIZE_X;
	}
}

void StageDraw(void)
{
	// �����w�i
	DrawTurnGraph(bgPosX1 - mapPos.x, -mapPos.y, bgImage1, false);
	DrawGraph(bgPosX2 - mapPos.x, -mapPos.y, bgImage1, false);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawGraph(-mapPos.x, -mapPos.y, bgImage_gr, true);
	//	// �w�i�̃|���|�������Ƃ���̉摜
	//	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	//	DrawGraph(-mapPos.x, -mapPos.y, bgImage_gr, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ���ׂẲ摜���|���|�����Ȃ��悤�ɂ����ŕ��ʂɖ߂�

	for (int y = 0; y < expData.mapHeight; y++)
	{
		for (int x = 0; x < expData.mapWidth; x++)
		{
			DrawGraph(x*expData.chipWidth - mapPos.x, y*expData.chipHeight - mapPos.y,
				mapImage2[mapData[y][x]], true);
		}
	}


	//DrawGraph(SCREEN_SIZE_X / 2 - mapPos.x, SCREEN_SIZE_Y + 500 - mapPos.y, hatenaImage, true);

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
	fopen_s(&file, "MapData/map1.fmf", "rb");
	fread(&expData, sizeof(expData), 1, file);		// �w�b�_�[�����ǂݍ���

	mapDataBase.resize(expData.mapWidth*expData.mapHeight);
	for (int j = 0; j < expData.mapWidth*expData.mapHeight; j++)
	{
		fread(&mapDataBase[j], sizeof(unsigned char), 1, file); // 1�̃f�[�^���o��
	}
	fclose(file);

	mapData.resize(expData.mapHeight);

	for (int j = 0; j < expData.mapHeight; j++)
	{
		mapData[j] = &mapDataBase[expData.mapWidth*j];
	}

	return true;
}

XY MapPosToIndex(Position pos)	// pos = mvedPos
{
	XY mapIndex;

	//mapIndex = { (int)pos.x / CHIP_SIZE_X, (int)pos.y / CHIP_SIZE_Y };
	mapIndex = { (int)pos.x / expData.chipWidth, (int)pos.y / expData.chipHeight };

	return mapIndex;
}

bool IsPass(Position pos)
{
	bool block = true;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	
	int mapNo;
	mapNo = mapData[mapIndex.y][mapIndex.x];

	switch (mapNo) 
	{
	case MAP_ID::MAP_ID_CAGUHT:		// ���C���[������������Ƃ���
	case MAP_ID::MAP_ID_UG:			// �n��
	case MAP_ID::MAP_ID_WALL:		// ��ȕǁA�n��
	case MAP_ID::MAP_ID_WALL_KICK:	// �ǏR��W�����v
		block = false;			// �u���b�N������̂ł���������
		break;
	case MAP_ID::MAP_ID_BACK:		  // �w�i
		break;
	}
	return block;
}
 
bool WireBlockPass(Position pos)
{
	bool blueblock = true;		// ���C���[���������u���b�N�����邩�̔���: true = �Ȃ��@false = ����
	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = mapData[mapIndex.y][mapIndex.x];

	switch (mapNo)
	{
	case MAP_ID::MAP_ID_CAGUHT:		// ���C���[������������Ƃ���
		blueblock = false;		// �u���b�N������̂ł���������
		break;
	}
	return blueblock;
}

// �ǂ𑖂�
bool WallBlockPass(Position pos)
{
	bool wall = false;

	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = mapData[mapIndex.y][mapIndex.x];

	switch (mapNo)
	{
	case MAP_ID::MAP_ID_WALL_KICK:	// �ǏR��W�����v
		wall = true;
		break;
	}
	return wall;
}

// �ǂɂ҂����肭�����悤�ɂ���
Position MapIndexToPos(XY index)
{
	Position mapPos;

	//mapPos = { (float)(index.x * CHIP_SIZE_X), (float)(index.y * CHIP_SIZE_Y) };
	mapPos = { (float)(index.x * expData.chipWidth), (float)(index.y * expData.chipHeight) };

	return mapPos;
}


// �����ׂ̗̃u���b�N��LEFT�Ȃ獶��,RIGHT�Ȃ�E��
Position GetWorldPos_Map(Position pos, MOVE_DIR der) {
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
