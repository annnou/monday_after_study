﻿/**
 * ゲームプログラムヘッダー
 * @author 作者名
 * @copyright コピーライト
 * @version 0.1.0
 *
 * history:	
 *
 */
#define __GAME_H__
#define __GAME_H__
#include "./WinGDILib.h"
#define NUM 3
#define WIDTH 800
#define HEIGHT 600
#define size 40
#define ball_size 10

int x[1000] = { 0 };
int y[1000] = { 0 };

int player_x = 0;
int player_y = 0;
int enemy_x[1000] = {0};
int enemy_y[1000] = {0};

int ene_num = 0;
int n = 0;
int num = 0;
int array_ = 0;

bool counter[1000] = { false };
bool isdown[1000] = { false };