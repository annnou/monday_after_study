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

void set_color(int color)
{

}

void InitGame()
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < NUM; i++) {
		x[i] = rand() % WIDTH + 1;
		y[i] = rand() % HEIGHT + 1;
	}
}

void GameMain()
{
	if (KEY_A == PUSH_KEY && num <= 30) {
		num++;
	}
	if (KEY_S == PUSH_KEY && num >= 0) {
		num--;
	}

	for (int i = 0; i < num; i++) {

		if (num == 30) {
			setBrush(RGB(0, 0, 255));
		}
		else {
			setBrush(RGB(0, 255, 0));
		}
		setPen(RGB(255, 0, 0), PS_SOLID, 3);
		prtEllipse(x[i], y[i], size, size, 1);

		if (x[i] >= WIDTH - size)x_counter[i] = true;
		if (y[i] >= HEIGHT - size)y_counter[i] = true;
		if (x[i] <= 0)x_counter[i] = false;
		if (y[i] <= 0)y_counter[i] = false;

		if (!x_counter[i])x[i] += speed;
		else x[i] -= speed;
		if (!y_counter[i])y[i] += speed;
		else y[i] -= speed;

	}
	/*
		setBrush(RGB(0, 255, 0));
		setPen(RGB(255, 0, 0), PS_SOLID, 3);
		prtEllipse(x, y, size, size, 1);

	if (KEY_RIGHT == HOLD_KEY && x < WIDTH - size)x += speed;
	if (KEY_LEFT == HOLD_KEY && 0 < x) x -= speed;
	if (KEY_UP == HOLD_KEY && 0 < y)y -= speed;
	if (KEY_DOWN == HOLD_KEY && y < HEIGHT - size)y += speed;
	*/
	return;
}

void ExitGameMain()
{

}
//setBrush(RGB(0, 0, 255));
//setPen(RGB(255, 0, 0), PS_SOLID, 5);
//prtRect(1, 1, R, D, 1);
//prtLine(R / 4, 1, R / 4, D);
