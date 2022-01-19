#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>

#define Color_Bird TFT_RED
#define Color_Tube TFT_DARKGREEN
#define Color_Back TFT_SKYBLUE
#define Val_Touch 26
TFT_eSPI tft = TFT_eSPI();

int high, width;
int birdX, birdY, birdOldX, birdOldY;
int wallY1,wallY2,wallY3;
int rand1, rand2, rand3;
char Score[4];
int score;
bool touchFlag = false;

void Config(){

  tft.fillScreen(Color_Back);

	birdX = 20;
	birdY = width/4;

	wallY1 = width - 1;
	wallY2 = width * 3;
	wallY3 = width * 3;

  //randomSeed(touchRead(T4));
	rand1 = rand() % (high - 20) + 20;

	score = 0;

}


void EventDraw() {
	for (int i = 0; i < high; i++) {
		for (int j = 0; j < width; j++) {
			if ((i == birdY) && (j == birdX))
      {
        tft.drawTriangle(birdX,birdOldY+2,birdX+2,birdOldY-2,birdX-2,birdOldY-2,Color_Back);
				tft.drawTriangle(birdX,birdY+2,birdX+2,birdY-2,birdX-2,birdY-2,Color_Bird);
      }
			else if (j == wallY1 && ((i <= rand1) || (i >= rand1 + high / 4)))
      {
        tft.drawPixel(wallY1+1,i,Color_Back);
				tft.drawPixel(wallY1,i,Color_Tube);
      }
			else if (j == wallY2 && ((i <= rand2) || (i >= rand2 + high / 4)))
      {
        tft.drawPixel(wallY2+1,i,Color_Back);
        tft.drawPixel(wallY2,i,Color_Tube);
      }
			
			else if (j == wallY3 && ((i <= rand3) || (i >= rand3 + high / 4)))
      {
        tft.drawPixel(wallY3+1,i,Color_Back);
        tft.drawPixel(wallY3,i,Color_Tube);
      }
		}
	}
  tft.fillRect(160,0,16,20,TFT_BLACK);
  
  tft.drawString(Score,166,5);
}

void EventWithoutInput() {
	if (birdX == wallY1) {
		if ((birdY > rand1) && (birdY < (rand1 + high / 4)))
			score++;
		else{
      exit(0);
    }
			
	}
	else if (birdX == wallY2) {
		if ((birdY > rand2) && (birdY < (rand2 + high / 4)))
			score++;
		else{
      exit(0);
    }
	}
	else if (birdX == wallY3) {
		if ((birdY > rand3) && (birdY < (rand3 + high / 4)))
			score++;
		else{
      exit(0);
    }
	}
	if (birdY >= high) {
      exit(0);
	}
  sprintf(Score,"%d",score);
  birdOldY = birdY;
	birdY +=2;
	wallY1--;
	wallY2--;
	wallY3--;

	if (wallY1 == 0) {
		rand1 = rand() % (high-8);
		wallY1 = width;
	}
	if (wallY1 == (width / 3) * 2) {
		rand2 = rand() % (high - 8);
		wallY2 = width;
	}
	if (wallY2 == (width / 3) * 2) {
		rand3 = rand() % (high - 8);
		wallY3 = width;
	}

	delay(8);
}

void EventWithInput() {
  if(touchRead(T4) < Val_Touch && !touchFlag){
    touchFlag = true;
    birdY -= 24;
    if(birdY < -48)
      birdY = -48;
  }
  if(touchRead(T4) > Val_Touch)
    touchFlag = false;
}

void setup() {
  tft.init();
  high = 220;
	width = 176;
  touchRead(T4);
}


void loop() {
  
  if(touchRead(T4) < Val_Touch){
    Config();
    while(1){
		  EventDraw();
		  EventWithoutInput();
		  EventWithInput();
    }
  }
  else{
    tft.drawString("Touch to start",50,110);
  }
  // for(int i=1;i<=360;i++){
  //   x = 86  + r * cos(i * PI /180);
  //   y = 110 + r * sin(i * PI /180);
  //   tft.drawLine(86,110,x,y,TFT_RED);
  //   tft.drawLine(86,110,x,y,TFT_RED);
  //   delay(20);
  //   tft.drawLine(86,110,x,y,TFT_WHITE);
  //}
}