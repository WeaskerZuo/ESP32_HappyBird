#ifndef _HAPPYBIRD_H_
#define _HAPPYBIRD_H_

#include <Arduino.h>
#include <TFT_eSPI.h>

typedef struct Bird{
	int x1,y1;
}BirdBody;
typedef struct Wall{
	int x1,y1 = 0;
	int blank_y1,blank_y2;
}WallBody;
void Bird_Init(BirdBody *Bird);
void Wall_Init(WallBody *Wall);
void Wall_Draw(WallBody *Wall);
void Score_Judge();

void Config();
void EventDraw();
void EventWithoutInput();
void EventWithInput();
void HappyBirdInit();
void HappyBirdRun();

#endif