#include "Dxlib.h"	//DxLibײ���؂��g�p����@�Ǝ��ŏ�������ͯ�ް̧�ق�""�Ŏw�肷��
#include <vector>
#include <math.h>	// �W��ͯ�ް̧�ق�<>�Ŏw�肷��
#include"main.h"
#include "player.h"
#include"stage.h"

struct DataHeader {
	unsigned char identifier[4];
	unsigned int  size;				// �T�C�Y
	unsigned int  mapWidth;			// ����
	unsigned int  mapHeight;		// �c��
	unsigned char chipWidth;		// �`�b�v����
	unsigned char chipHeight;		// �`�b�v�c��
	unsigned char layerCount;		// ���C���[��
	unsigned char bitCount;			// 8bit
};
#define MAP_FILE_ID "FMF_"

//enum MAP_ID {
//	MAP_ID_CAGUHT,		 // ���C���[������������Ƃ���
//	MAP_ID_UG,			 // �n��
//	MAP_ID_WALL,		 // ��ȕǁA�n��
//	MAP_ID_WALL_KICK,	 // �ǏR��W�����v
//	MAP_ID_BACK,	 	 // �w�i
//	MAP_ID_MAX
//};

DataHeader expData;

std::vector<MAP_ID>mapDataBase; // �f�[�^�����锠
std::vector<MAP_ID*>mapData; // �A�h���X������锠

int mapImage[5];
int mapImage2[5];

int hatenaImage;


void StageSystmInit(void)
{
	LoadDivGraph("image/back.png", 5, 5, 1, CHIP_SIZE_X, CHIP_SIZE_Y, mapImage2);
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
	if (mapPos.x < 0) mapPos.x = 0;
	if (mapPos.x > expData.mapWidth*CHIP_SIZE_X - SCREEN_SIZE_X) mapPos.x
		= expData.mapWidth * CHIP_SIZE_X - SCREEN_SIZE_X;

	//// y����
	if (mapPos.y < 0) mapPos.y = 0;
	if (mapPos.y > expData.mapHeight *CHIP_SIZE_Y - SCREEN_SIZE_Y) mapPos.y
		= expData.mapHeight * CHIP_SIZE_Y - SCREEN_SIZE_Y;
}

void StageDraw(void)
{
	for (int y = 0; y < expData.mapHeight; y++)
	{
		for (int x = 0; x < expData.mapWidth; x++)
		{
			DrawGraph(x*expData.chipWidth - mapPos.x, y*expData.chipHeight - mapPos.y, mapImage2[mapData[y][x]], true);
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

	mapIndex = { (int)pos.x / CHIP_SIZE_X, (int)pos.y / CHIP_SIZE_Y };

	return mapIndex;
}

bool IsPass(Position pos)
{
	bool ret = true; // �����邩�ǂ����@true�Ȃ瓮���Ȃ��@false������
	int mapNo;
	bool block = true;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = mapData[mapIndex.y][mapIndex.x];

	switch (mapNo) // enum��
	{
	case MAP_ID::MAP_ID_CAGUHT:		// ���C���[������������Ƃ���
	case MAP_ID::MAP_ID_UG:			// �n��
	case MAP_ID::MAP_ID_WALL:		// ��ȕǁA�n��
	case MAP_ID::MAP_ID_WALL_KICK:	// �ǏR��W�����v
		block = false;			// �u���b�N������̂ł���������

		break;
	case MAP_ID::MAP_ID_BACK:		  // �w�i

		ret = false;
		break;
	}
	return block;
	return ret;

	//XY mapIndex;

	//mapIndex = MapPosToIndex(pos);
	//

	//for (int y = 0; y < expData.mapHeight; y++)
	//{
	//	for (int x =0; x < expData.mapWidth; x++)
	//	{
	//		MAP_ID mapNo = mapData[y][x];
	//		switch (mapNo) // enum��
	//		{
	//		case MAP_ID::MAP_ID_CAGUHT:		// ���C���[������������Ƃ���
	//		case MAP_ID::MAP_ID_UG:			// �n��
	//		case MAP_ID::MAP_ID_WALL:		// ��ȕǁA�n��
	//		case MAP_ID::MAP_ID_WALL_KICK:	// �ǏR��W�����v
	//			block = false;			// �u���b�N������̂ł���������

	//			break;
	//		case MAP_ID::MAP_ID_BACK:		  // �w�i

	//			ret = false;
	//			break;
	//		}
	//		return block;
	//		return ret;

	//	}
	//}

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
	case MAP_ID::MAP_ID_UG:			// �n��
	case MAP_ID::MAP_ID_WALL:		// ��ȕǁA�n��
	case MAP_ID::MAP_ID_WALL_KICK:	// �ǏR��W�����v
	case MAP_ID::MAP_ID_BACK:		  // �w�i
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

	mapPos = { (float)(index.x * CHIP_SIZE_X), (float)(index.y * CHIP_SIZE_Y) };

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
