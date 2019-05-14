#pragma once

#define CHIP_SIZE_X 32
#define CHIP_SIZE_Y 32

//#define MAP_X 38
#define MAP_X 76

#define MAP_Y 52
//#define MAP_Y 26

void StageSystmInit(void);
void StageGameInit(void);
void StageControl(void);
void StageDraw(void);
void BgControl(void);

bool MapLoad(void);				// Ï¯Ìß‚ğÛ°ÄŞ@•K—v‚Å‚ ‚ê‚Îˆø”

XY MapPosToIndex(XY pos);
bool IsPass(XY pos);
//bool WireBlockPass(XY pos);

XY MapIndexToPos(XY index);
XY MapPos(XY pos, MOVE_DIR der);

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