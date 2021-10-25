#include "Game.h"
#include<stdio.h> 
#include<stdlib.h> 
#include<time.h> 
#include<Windows.h>

#define WIDTH 800
#define HEIGHT 600
#define size 100



void InitGame()
{
	srand((unsigned)time(NULL));
	num1 = rand() % 4;
	num2 = rand() % 4;

	for (int i = 0; i < 4; i++) {
		for (int o = 0; o < 4; o++) {
			r[i][o] = 1;
		}
	}
	r[num1][num2] = 0;
}

void GameMain()
{

	for (int i = 0; i < 4; i++) {
		for (int o = 0; o < 4; o++) {
			setPen(RGB(100, 100, 100), PS_SOLID, 5);
			prtRect(200 + size * o , 100 + size * i, size, size,1);
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int o = 0; o < 4; o++) {
			if (r[i][o] == 0) {
				if (!down && !i == 0 && KEY_UP == PUSH_KEY) {
					r[i][o] = 1;
					r[i - 1][o] = 0;
					down = true;
				}
				if (!down && (i == 0 || i == 1 || i == 2) && KEY_DOWN == PUSH_KEY) {
					r[i][o] = 1;
					r[i + 1][o] = 0;
					down = true;
				}
				if (!down && (o == 0 || o == 1 || o == 2) && KEY_RIGHT == PUSH_KEY) {
					r[i][o] = 1;
					r[i][o + 1] = 0;
					down = true;
				}
				if (!down && !o == 0 && KEY_LEFT == PUSH_KEY) {
					r[i][o] = 1;
					r[i][o - 1] = 0;
					down = true;
				}
				setPen(RGB(255, 0, 0), PS_SOLID, 5);
				prtRect(200 + size * o, 100 + size * i, size, size, 1);
			}
		}
	}



	setPen(RGB(255, 0, 0), PS_SOLID, 5);


	down = false;

	return;
}

void ExitGameMain()
{

}
//setBrush(RGB(0, 0, 255));
//setPen(RGB(255, 0, 0), PS_SOLID, 5);
//prtRect(1, 1, WIDTH, HEIGHT, 1);
//prtLine(WIDTH / 4, 1, WIDTH / 4, HEIGHT);
