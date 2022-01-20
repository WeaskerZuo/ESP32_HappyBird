#include "HappyBird.h"
TFT_eSPI tft = TFT_eSPI();

#define COLOR_BIRD TFT_RED
#define COLOR_WALL TFT_RED
#define COLOR_BACKGROUND TFT_SKYBLUE
#define Val_Ttouch 26
#define BIRD_WIDTH 8
#define WALL_WIDTH 16
#define WALL_LIMIT 30
#define BLANK_HIGH 48
/*
(0,0)
	(176,220);
*/

BirdBody NewBird,OldBird;
WallBody NewWall1,NewWall2,NewWall3;
WallBody OldWall1,OldWall2,OldWall3;
int score;
bool TouchFlag = false;
bool RunFlag = true;

void Bird_Init(BirdBody *Bird){
	Bird->x1 = TFT_WIDTH / 4;
	Bird->y1 = TFT_HEIGHT / 4;
}
void Wall_Init(WallBody *Wall){
	Wall->x1 = TFT_WIDTH;
	Wall->blank_y1 = rand() % (TFT_HEIGHT - 2*WALL_LIMIT - BLANK_HIGH) +  WALL_LIMIT;   //WALL_LIMIT <= blank_y1 <= (160 + 30)     blank_y1 + BLANK_HIGH <= TFT_HEIGHT - WALL_LIMIT        
	Wall->blank_y2 = Wall->blank_y1 + BLANK_HIGH;
	//Serial.printf("blank_y1:%d   blank_y2:%d\r\n",Wall->blank_y1,Wall->blank_y2);
}
void Wall_Draw(WallBody *Wall){
	if(Wall->x1 < 0){
		int x =  WALL_WIDTH + Wall->x1;
		tft.fillRect(0, Wall->y1, x+1, Wall->blank_y1, COLOR_BACKGROUND);
		tft.fillRect(0, Wall->blank_y2, x+1, TFT_HEIGHT - Wall->blank_y2, COLOR_BACKGROUND);
    	tft.fillRect(0, Wall->y1, x, Wall->blank_y1, COLOR_WALL);
		tft.fillRect(0, Wall->blank_y2, x, TFT_HEIGHT - Wall->blank_y2, COLOR_WALL);
	}
	else{
		tft.fillRect(Wall->x1+1, Wall->y1, WALL_WIDTH, Wall->blank_y1, COLOR_BACKGROUND);
		tft.fillRect(Wall->x1+1, Wall->blank_y2, WALL_WIDTH, TFT_HEIGHT - Wall->blank_y2, COLOR_BACKGROUND);
    	tft.fillRect(Wall->x1, Wall->y1, WALL_WIDTH, Wall->blank_y1, COLOR_WALL);
		tft.fillRect(Wall->x1, Wall->blank_y2, WALL_WIDTH, TFT_HEIGHT - Wall->blank_y2, COLOR_WALL);
	}
}
void Score_Judge(){

	if ((NewBird.x1 + BIRD_WIDTH) >= NewWall1.x1 && NewBird.x1 <= (NewWall1.x1 + WALL_WIDTH)) {
		if((NewBird.y1 <= NewWall1.blank_y1) || ((NewBird.y1 + BIRD_WIDTH) >=NewWall1.blank_y2)){
			//Serial.printf("BirdXY:%d,%d     Wall111XY:%d,%d,%d\r\n",NewBird.x1,NewBird.y1,NewWall1.x1,NewWall1.blank_y1,NewWall1.blank_y2);
			RunFlag = false;
		}
		else if(NewBird.x1 == (NewWall1.x1 + WALL_WIDTH)){
			score++;
		}
	}

	else if((NewBird.x1 + BIRD_WIDTH) >= NewWall2.x1 && NewBird.x1 <= (NewWall2.x1 + WALL_WIDTH))
	{
		if((NewBird.y1 <= NewWall2.blank_y1) || ((NewBird.y1 + BIRD_WIDTH) >=NewWall2.blank_y2)){
			//Serial.printf("BirdXY:%d,%d     Wall222XY:%d,%d,%d\r\n",NewBird.x1,NewBird.y1,NewWall2.x1,NewWall2.blank_y1,NewWall2.blank_y2);
			RunFlag = false;
		}
		else if(NewBird.x1 == (NewWall2.x1 + WALL_WIDTH)){
			score++;
		}
	}
	else if((NewBird.x1 + BIRD_WIDTH) >= NewWall3.x1 && NewBird.x1 <= (NewWall3.x1 + WALL_WIDTH))
	{
		if((NewBird.y1 <= NewWall3.blank_y1) || ((NewBird.y1 + BIRD_WIDTH) >=NewWall3.blank_y2)){
			Serial.printf("BirdXY:%d,%d     Wall222XY:%d,%d,%d\r\n",NewBird.x1,NewBird.y1,NewWall3.x1,NewWall3.blank_y1,NewWall3.blank_y2);
			RunFlag = false;
		}
		else if(NewBird.x1 == (NewWall3.x1 + WALL_WIDTH)){
			score++;
		}
	}
	if ((NewBird.y1 + BIRD_WIDTH) > TFT_HEIGHT) {
      	RunFlag = false;
	}
}
void Config(){
  	tft.fillScreen(COLOR_BACKGROUND);
	Bird_Init(&NewBird);
	Bird_Init(&OldBird);
	Wall_Init(&NewWall1);
	Wall_Init(&NewWall2);
	Wall_Init(&NewWall3);
	score = 0;
}

void EventDraw() {
  	tft.drawNumber(score,165,5);
	for (int i = 0; i < TFT_HEIGHT; i++) 
	{
		for (int j = -30; j < TFT_WIDTH; j++) 
		{
			if ((i == NewBird.y1) && (j == NewBird.x1))
			{
				tft.fillRect(OldBird.x1, OldBird.y1, BIRD_WIDTH,BIRD_WIDTH, COLOR_BACKGROUND);
				tft.fillRect(NewBird.x1, NewBird.y1, BIRD_WIDTH,BIRD_WIDTH, COLOR_BIRD);
      		}
			else if (j == NewWall1.x1 && i == NewWall1.y1)
			{
				Wall_Draw(&NewWall1);
      		}
			else if (j == NewWall2.x1 && i == 0)
			{
				Wall_Draw(&NewWall2);
      		}
			else if (j == NewWall3.x1 && i == 0)
			{
       			Wall_Draw(&NewWall3);
      		}
		}
	}
}

void EventWithoutInput() {
	Score_Judge();
	OldBird = NewBird;
	OldWall1 = NewWall1;
	OldWall2 = NewWall2;
	OldWall3 = NewWall3;
	NewBird.y1 += 2;
	NewWall1.x1--;
	NewWall2.x1--;
	NewWall3.x1--;
	if (NewWall1.x1 + WALL_WIDTH == -WALL_WIDTH*2) {
		Wall_Init(&NewWall1);
	}
	if (NewWall1.x1 + WALL_WIDTH == (TFT_WIDTH / 3) * 2) {
		Wall_Init(&NewWall2);
	}
	if (NewWall2.x1 + WALL_WIDTH == (TFT_WIDTH / 3) * 2) {
		Wall_Init(&NewWall3);
	}
	delay(8);
}

void EventWithInput() {
  if(touchRead(T4) < Val_Ttouch && !TouchFlag){
    TouchFlag = true;
    NewBird.y1 -= 24;
    if(NewBird.y1 < -48)
      NewBird.y1 = -48;
  }
  if(touchRead(T4) > Val_Ttouch)
    TouchFlag = false;
}

void HappyBirdInit(){
	RunFlag = true;
	tft.init();
	touchRead(T4);
	Serial.begin(115200);
}

void HappyBirdRun() {   
    if(touchRead(T4) < Val_Ttouch){
        Config();
        while(RunFlag){
		    EventDraw();
		    EventWithoutInput();
		    EventWithInput();
        }
    }
    else{
        tft.drawString("Touch to start",50,110);
		RunFlag = true;
    }
}