/**
 * ゲームプログラムヘッダー
 * @author 作者名
 * @copyright コピーライト
 * @version 0.1.0
 *
 * history:	
 *
 */
#ifndef __GAME_H__
#define __GAME_H__
#include "./WinGDILib.h"
#define NUM 30
int num1 = 1;	
int num2 = 1;	
int _time;
int color1;
int color2;
int color3;

float x[100];		// 機体の左上X座標
float y[100];        // 機体の左上Y座標
float x_stick = 100;		        
float speed = 3;  // 機体のスピード
float x_random;
float y_random;

float size_;
float bar_size = 80;
float bar_speed = 7;

bool x_counter;
bool y_counter;
bool down = false;

int r[4][4] = { 0 };


#endif