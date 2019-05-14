#pragma once

enum MAP_ID {
	MAP_ID_CAGUHT,		 // ���C���[������������Ƃ���
	MAP_ID_UG,			 // �n��
	MAP_ID_WALL,		 // ��ȕǁA�n��
	MAP_ID_WALL_KICK,	 // �ǏR��W�����v
	MAP_ID_BACK,	 	 // �w�i
	MAP_ID_MAX
};
#define CHIP_SIZE_X 32
#define CHIP_SIZE_Y 32

#define MAP_X 76
//#define MAP_X 50

#define MAP_Y 52
//#define MAP_Y 30



void StageSystmInit(void);
void StageGameInit(void);
void StageControl(void);
void StageDraw(void);

bool MapLoad(void);				// ϯ�߂�۰�ށ@�K�v�ł���Έ���


XY MapPosToIndex(XY pos);
bool IsPass(XY pos);
//bool WireBlockPass(XY pos);

XY MapIndexToPos(XY index);

XY GetWorldPos_Map(XY pos, MOVE_DIR der);



//class stage
//{
//public:
//	static stage &GetInstance(void)
//	{
//		static stage  s_Instance;
//		return (s_Instance);
//	}
//
//	bool ret = true;
//
//};