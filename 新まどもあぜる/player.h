#pragma once

//enum PLAYER_STATE {
//	PLAYER_NORMAL,		// ���E�ړ� 
//	PLAYER_DOWN,	    // �W�����v���~
//	PLAYER_JUMP_UP,		// �W�����v�̏㏸
//	PLAYER_W_PRE,		// ܲ԰����݂̏���(���C���[��L�΂�)
//	PLAYER_W_ACTION,	// ܲ԰�����
//	PLAYER_W_JUMP,		// ܲ԰�ެ���
//	PLAYER_WALL_RIGHT,	// ��(�E)�𑖂�
//	PLAYER_WALL_LEFT,	// ��(��)�𑖂�
//};

//enum ITEM_STATE {
//	ITEM_NON,			// �A�C�e�����擾���Ă��Ȃ����(�Ȃɂ��Ȃ�)
//	ITEM_SEGWEY,		// �Z�O�E�F�C
//	ITEM_KABOSU,		// ���ڂ�(�啪���Y)
//	ITEM_UFO,			// UFO
//	ITEM_SPECIAL,		// �K�E�Z1��v���X
//};

void PlayerSystmInit(void);
void PlayerGameInit(void);
void PlayerControl(int padNo);
void PlayerDraw(int padNo);

void AddRad(int padNo);

void PlNormal(int padNo);
void PlJumpUp(int padNo);
void PlDown(int padNo);
void PlWirePrepare(int padNo);
void PlWireAction(int padNo);
void PlWireJump(int padNo);
void PlWall_Check(int padNo); // �Ǒ���ł���ǂł��邩�ǂ���
void PlWall_R(int padNo);	// �ǂ𑖂�(�E)
void PlWall_L(int padNo);	// �ǂ𑖂�(��)
void PlHasamuJump(int padNo);
void Pl_Death(int padNo); // HP0�̂Ƃ���


void UIDraw(int padNo);
void UIDrawSel(int padNo);

// �A�C�e���֘A

void GetItemRand(void);	// player.dropFlag��true�ɂȂ����Ƃ��ɃA�C�e���������_���Ō��肳����
void ItemSegwey(int padNo);
void ItemKabosu(void);
void ItemUFO(void);
void ItemSP(void);

void PlayerState(int padNo);
void ItemState(int padNo);

void Passing(int padNo);
void CameraControl(int padNo);
void ScrollMap(Position pos, int speed, MOVE_DIR dir);
int PlayerTop(int padNo, int passCnt, Position pos, MOVE_DIR dir);

bool GetPlayerV(void);	// ��ڲ԰�����m�F
Position GetPlayerPos(int padNo);
int GetPassingCnt(int padNo);