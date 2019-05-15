#include "Dxlib.h"				// DxLibﾗｲﾌﾞﾗﾘを使用する　独自で準備したﾍｯﾀﾞｰﾌｧｲﾙは""で指定する
#include <math.h>				// 標準ﾍｯﾀﾞｰﾌｧｲﾙは<>で指定する
#include "main.h"
#include"effect.h"

int effectImage;
EFFECT effect[EFFECT_MAX];
float acc;	// 加速度
float angle;  // 計算用 
int ecnt;

void EffectSystmInit(void)
{
	effectImage = LoadGraph("image/suna.png");
}

void EffectGameInit(void)
{
	for (int i = 0; i < EFFECT_MAX; i++) {
		effect[i].flag = false;
	}

}

void EffectControl(void)
{
	for (int i = 0; i < EFFECT_IMAGE_MAX; i++) {
		if (effect[i].flag) {
			effect[i].pos.x -= effect[i].mov.x;
			effect[i].pos.y -= effect[i].mov.y;
			effect[i].speed-=0.1f;
			effect[i].mov.x = cos((PI / 180)*effect[i].angle)*effect[i].speed;
			effect[i].mov.y = sin((PI / 180)*effect[i].angle) * effect[i].speed - acc;
			acc += 0.1f;
			ecnt++;
			if (ecnt>200) effect[i].flag = false;
		}
		

	}

}

void EffectDraw(void)
{
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (effect[i].flag) {
			DrawRotaGraph(effect[i].pos.x, effect[i].pos.y, 1, 0, effectImage, true);
		}
	}
}

void SetEffct(XY pPos)
{
	acc = 0;
	angle = 0;

	for (int i = 0; i < EFFECT_IMAGE_MAX; i++) {
		if (!effect[i].flag) {
			effect[i].pos = pPos;
			effect[i].angle = angle;
			effect[i].speed = GetRand(10) + 1;
			effect[i].mov.x = cos((PI / 180)*angle)*effect[i].speed;
			effect[i].mov.y = sin((PI / 180)*angle)*effect[i].speed;
			effect[i].flag = true;
			angle += 3.0f;
		}
	}
	ecnt = 0;
}