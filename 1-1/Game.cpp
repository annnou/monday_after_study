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

void move_vector2()
{
	switch (rand() % 4) {
	case 0:
		x_counter = false;
		y_counter = false;
		break;
	case 1:
		x_counter = true;
		y_counter = false;
		break;
	case 2:
		x_counter = false;
		y_counter = true;
		break;
	case 3:
		x_counter = true;
		y_counter = true;
		break;
	}

}
void square(int ball_x,int ball_y) 
{
	int a = 200;
	int b = 150;
	int c = 100;

	if ((a < ball_x + size && ball_x < a + c) && (b + c < ball_y && ball_y + size < b))color1 = 0;
	if ((a * 2 < ball_x + size && ball_x < (a * 2 + c)) && (b + c < ball_y && ball_y + size < b))color2 = 0;
	if ((a * 3 < ball_x + size && ball_x < (a * 3 + c)) && (b + c < ball_y && ball_y + size < b))color3 = 0;

	setBrush(RGB(0, 0, color1));
	setPen(RGB(100, 100, 255), PS_SOLID, 3);
	prtRect(a, b, c, c, 1);

	setBrush(RGB(0, 0, color2));
	setPen(RGB(100, 100, 255), PS_SOLID, 3);
	prtRect(a * 2, b, c, c, 1);

	setBrush(RGB(0, 0, color3));
	setPen(RGB(100, 100, 255), PS_SOLID, 3);
	prtRect(a * 3, b, c, c, 1);

}

void InitGame()
{
	size_ = size;

	color1 = 255;
	color2 = 255;
	color3 = 255;

	srand((unsigned)time(NULL));

	x = rand() % (800-size);
	y = rand() % (600-size);

	move_vector2();
}

void GameMain()
{

	setPen(RGB(0, 255, 0), PS_SOLID, 4);
	prtLine(x_stick, HEIGHT - 50, x_stick + bar_size, HEIGHT - 50);

	if (KEY_LBUTTON == HOLD_KEY) {
		x_stick -= bar_speed;
		if (x_stick < 0) {
			x_stick = 0;
		}
	}
	if (KEY_RBUTTON == HOLD_KEY) {
		x_stick += bar_speed;
		if (x_stick > WIDTH - bar_size) {
			x_stick = WIDTH - bar_size;
		}
	}

	setBrush(RGB(255, 255, 255));
	setPen(RGB(255, 255, 0), PS_SOLID, 3);
	prtEllipse(x, y, size_, size_, 1);

	if (x + size_ >= WIDTH)x_counter = true;
	else if (x <= 0)x_counter = false;
	if (y + size_ >= HEIGHT)y_counter = true;
	else if (y <= 0)y_counter = false;

	if((x_stick <= x && x + size_ <= x_stick + bar_size) && y <= HEIGHT - 49 && y >= HEIGHT - 50 && y_counter)
		y_counter = false;
	if((x_stick <= x && x + size_ <= x_stick + bar_size) && y + size_ >= HEIGHT - 51 && y + size_ <= HEIGHT - 50 && !y_counter)
		y_counter = true;

	square(x,y);	

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
