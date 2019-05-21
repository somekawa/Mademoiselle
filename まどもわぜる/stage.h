#pragma once

enum MAP_ID {
	MAP_ID_CAGUHT,		 // ワイヤーが引っかかるところ
	MAP_ID_UG,			 // 地中
	MAP_ID_WALL,		 // 主な壁、地面
	MAP_ID_WALL_KICK,	 // 壁蹴りジャンプ
	MAP_ID_BACK,	 	 // 背景
	MAP_ID_MAX
};
#define CHIP_SIZE_X 32
#define CHIP_SIZE_Y 32



//#define MAP_X 76
////#define MAP_X 50
//
//#define MAP_Y 52
//#define MAP_Y 30



void StageSystmInit(void);
void StageGameInit(void);
void StageControl(void);
void StageDraw(void);

bool MapLoad(void);				// ﾏｯﾌﾟをﾛｰﾄﾞ　必要であれば引数


XY MapPosToIndex(Position pos);
bool IsPass(Position pos);
bool WireBlockPass(Position pos);
bool WallBlockPass(Position pos);

Position MapIndexToPos(Position index);

Position GetWorldPos_Map(Position pos, MOVE_DIR der);



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