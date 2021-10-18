#include "Game.h"
#include<stdio.h> 
#include<stdlib.h> 
#include<time.h> 
#include<Windows.h>

#define WIDTH 800
#define HEIGHT 600
#define size 20

enum {
	RED,
	GREEN,
	BLUE,
};



void InitGame()
{

}

void GameMain()
{
	float size_ = size;

	setPen(RGB(0, 255, 0), PS_SOLID, 4);
	prtLine(x_stick, HEIGHT - 50, x_stick + 80, HEIGHT - 50);

	if (KEY_LBUTTON == HOLD_KEY) {
		if(0 <= x_stick && x_stick + 80 <= WIDTH)x_stick = MOUSE_PX;
	}

	setBrush(RGB(0, 255, 0));
	setPen(RGB(255, 0, 0), PS_SOLID, 3);
	prtEllipse(x, y, size_, size_, 1);

	if (x + size_ >= WIDTH)x_counter = true;
	else if (x <= 0)x_counter = false;
	if (y + size_ >= HEIGHT)y_counter = true;
	else if (y <= 0)y_counter = false;

	if((x_stick <= x && x + size_ <= x_stick + 80) && y == HEIGHT - 50)
		y_counter = false;
	if((x_stick <= x && x + size_ <= x_stick + 80) && y + size_ == HEIGHT - 50)
		y_counter = true;

	if (!x_counter)x += speed;
	else x -= speed;
	if (!y_counter)y += speed;
	else y -= speed;

	return;
}

void ExitGameMain()
{

}
//setBrush(RGB(0, 0, 255));
//setPen(RGB(255, 0, 0), PS_SOLID, 5);
//prtRect(1, 1, R, D, 1);
//prtLine(R / 4, 1, R / 4, D);
