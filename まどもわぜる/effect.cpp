#include "Dxlib.h"				// DxLibﾗｲﾌﾞﾗﾘを使用する　独自で準備したﾍｯﾀﾞｰﾌｧｲﾙは""で指定する
#include <math.h>				// 標準ﾍｯﾀﾞｰﾌｧｲﾙは<>で指定する
#include "main.h"
#include"effect.h"

int effectImage;
EFFECT effect[EFFECT_MAX];
float acc;	// 加速度
float angle;  // 計算用 
int ecnt;
bool eFlag;

//bool newPEffect;
//bool trgPEffect;
//bool oldPEffect;

void EffectSystmInit(void)
{
	effectImage = LoadGraph("image/suna.png");
}

void EffectGameInit(void)
{
	for (int i = 0; i < EFFECT_MAX; i++) {
		effect[i].flag = false;
	}
	/*newPEffect = false;
	trgPEffect = false;
	oldPEffect = false;*/
}

void SetEffct(Position pPos)
{
	//newPEffect = true;

	//trgPEffect = newPEffect & ~oldPEffect;	// trgKey
	//oldPEffect = newPEffect;

	//if (trgPEffect) {
		acc = 0;
		angle = 0;

		for (int i = 0; i < EFFECT_IMAGE_MAX; i++) {
			if (!effect[i].flag) {
				effect[i].pos = pPos;
				effect[i].angle = angle;
				effect[i].speed = GetRand(16) + 1;
				effect[i].mov.x = cos((PI / 180)*angle)*effect[i].speed;
				effect[i].mov.y = sin((PI / 180)*angle)*effect[i].speed;
				effect[i].flag = true;
				angle += 3.0f;
				ecnt = 0;
			}
		}
	//}
	

}

void EffectControl(void)
{
	for (int i = 0; i < EFFECT_IMAGE_MAX; i++) {
		if (effect[i].flag) {
			effect[i].pos.x -= effect[i].mov.x;
			effect[i].pos.y -= effect[i].mov.y;
			effect[i].speed -= 0.1f;
			effect[i].mov.x = cos((PI / 180)*effect[i].angle)*effect[i].speed;
			effect[i].mov.y = sin((PI / 180)*effect[i].angle) * effect[i].speed - acc;
			acc += 0.1f;
			ecnt++;
			if (ecnt >= 250) {
				effect[i].flag = false;

			}
		}
	}
}

void EffectDraw(void)
{
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (effect[i].flag) {
			DrawRotaGraph(effect[i].pos.x - mapPos.x, effect[i].pos.y - mapPos.y, 1, 0, effectImage, true);
		}
	}
}

