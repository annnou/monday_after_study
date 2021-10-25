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
}

void GameMain()
{
	if (KEY_A == HOLD_KEY)
	{
		setBrush(RGB(255, 0, 0));
		prtEllipse(WIDTH/2-size/2, HEIGHT/2-size/2,size , size, 1);
	}
	if (KEY_S == HOLD_KEY)
	{
		setBrush(RGB(0, 0, 255));
		prtRect(WIDTH/2-size/2, HEIGHT/2-size/2,size , size, 1);
	}
	if (KEY_D == HOLD_KEY)
	{
		setPen(RGB(0, 255, 0), PS_SOLID, 5);
		prtLine(WIDTH / 2 - size / 2, HEIGHT / 2 - size / 2, WIDTH / 2 + size / 2, HEIGHT / 2 + size / 2);
		prtLine(WIDTH / 2 - size / 2, HEIGHT / 2 + size / 2, WIDTH / 2 + size / 2, HEIGHT / 2 - size / 2);
		setPen(RGB(0, 255, 0), PS_SOLID, 0);
	}

	return;
}

void ExitGameMain()
{

}
//setBrush(RGB(0, 0, 255));
//setPen(RGB(255, 0, 0), PS_SOLID, 5);
//prtRect(1, 1, WIDTH, HEIGHT, 1);
//prtLine(WIDTH / 4, 1, WIDTH / 4, HEIGHT);
