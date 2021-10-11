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

int x[NUM];		// 機体の左上X座標
int y[NUM];        // 機体の左上Y座標
int num = 1;	//球を表示する個数

float speed = 2;  // 機体のスピード
float x_random;
float y_random;

bool x_counter[NUM] = { false };
bool y_counter[NUM] = { false };
bool down = false;


#endif