#pragma once

#define EFFECT_MAX 500
#define EFFECT_IMAGE_MAX 80

typedef struct {
	XY pos;
	XY mov;
	float speed;
	float angle;
	bool flag;
}EFFECT;

void EffectSystmInit(void);
void EffectGameInit(void);
void EffectControl(void);
void EffectDraw(void);

void SetEffct(XY pPos);