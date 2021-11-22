#include "Game.h"
#include<stdio.h> 
#include<stdlib.h> 
#include<time.h> 
#include<Windows.h>
#include<math.h>

#define SIZE 40 
#define SPEED 2;

void player()
{
	if (KEY_LEFT == HOLD_KEY && player_x - SIZE / 2 > 0)
	{
		player_x -= 3;
	}
	if (KEY_RIGHT == HOLD_KEY && player_x + SIZE < WIDTH)
	{
		player_x += 3;
	}

	setPen(RGB(0, 255, 0), PS_SOLID, 3);
	prtLine(player_x, player_y, player_x - SIZE / 2, player_y + SIZE);
	prtLine(player_x - SIZE / 2, player_y + SIZE, player_x + SIZE / 2, player_y + SIZE);
	prtLine(player_x + SIZE, player_y + SIZE, player_x, player_y);
}
void enemy(int speed ,int o)
{
	setBrush(RGB(0, 0, 0));
	setPen(RGB(255, 0, 0), PS_SOLID, 3);
	prtRect(enemy_x[o], enemy_y[o], SIZE, SIZE, 1);

	if (enemy_x[o] + SIZE > WIDTH)
	{
		counter[o] = true;
	}
	else if (enemy_x[o] < 0)
	{
		counter[o] = false;
	}
	
	if (!counter[o])
	{
		enemy_x[o] += speed;
	}
	else
	{
		enemy_x[o] -= speed;
	}
}
void tama(int o)
{

	if (KEY_SPACE == PUSH_KEY)
	{
		x[array_] = player_x - 2;
		y[array_] = player_y - 5;

		num++;
		array_++;
	}

	for (int i = n; i < n + num; i++) {

		setBrush(RGB(0, 0, 0));
		setPen(RGB(0, 0, 255), PS_SOLID, 5);
		prtEllipse(x[i] , y[i], 5, 20);

		y[i]-=3;

		if (y[i] + 3 < 0)
		{
			n++;
			num--;
		}

		if (enemy_x[o] <= x[i] && x[i] <= enemy_x[o] + SIZE
			&& enemy_y[o] <= y[i] && y[i] <= enemy_y[o] + SIZE)
		{
			isdown[o] = true;
		}
	}

}

void InitGame()
{
	srand((unsigned)time(NULL));

	player_x = WIDTH / 2;
	player_y = HEIGHT - SIZE - 60;

	for (int i = 0; i < 100; i++)
	{
		enemy_x[i] = WIDTH / 2;
		enemy_y[i] = rand() % 100 ;
	}

	ene_num = 10;

}

void GameMain()
{
	player();

	for (int p = 0; p < ene_num; p++) {

		if (!isdown[p]) {

			enemy(p + 1, p);
		}
		tama(p);
	}
}

void ExitGameMain()
{

}
//setBrush(RGB(0, 0, 255));
//setPen(RGB(255, 0, 0), PS_SOLID, 5);
//prtRect(1, 1, WIDTH, HEIGHT, 1);
//prtLine(WIDTH / 4, 1, WIDTH / 4, HEIGHT);

//void prtLine(int, int, int, int);					// 始点から終点まで線をひく
//void prtPolyline(POINT* p, int);					// 複数の頂点を持つ線を描く
//void prtTriangle(int, int, int, int, int, int, int fill = 1);		// 三角形を描く
//void prtRect(int, int, int, int, int fill = MODE_FILL);		//四角形を描く 
//void prtRoundRect(int, int, int, int, int, int, int fill = MODE_FILL); //丸みのある四角形を描く 
//void prtPolygon(POINT*, int, int fill = MODE_FILL);			// 複数の頂点を結ぶ多角形を描く
//void prtEllipse(int px, int py, int width, int height, int fill = MODE_FILL);	// 円（楕円）を描画する
//void prtArc(int px, int py, int width, int height, int sx, int sy, int ex, int ey, int mode = MODE_ARC, int fill = MODE_FILL); // 円弧を描画する


/*
#define	FREE_KEY	0 // キーが押されていない状態
#define	PUSH_KEY	1 // キーを押された瞬間
#define	PULL_KEY	2 // キーが放された瞬間
#define	HOLD_KEY	3 // キーが押されている状態
*/

/*
extern int MOUSE_PX;						// マウスの現在のX座標
extern int MOUSE_PY;						// マウスの現在のY座標
*/

/*
int KEY_LBUTTON		=0;		// マウス左ボタン
int KEY_RBUTTON		=0;		// マウス右ボタン
int KEY_UP			=0;		// ↑キー
int KEY_DOWN		=0;		// ↓キー
int KEY_LEFT		=0;		// ←キー
int KEY_RIGHT		=0;		// →キー
int KEY_RETURN		=0;		// ENTERキー
int KEY_SPACE		=0;		// スペースキー
int KEY_SHIFT		=0;		// SHIFTキー
int KEY_CONTROL		=0;		// CTRLキー
int KEY_ALT			=0;		// ALTキーVK_MENU
int KEY_ESCAPE		=0;		// ESCキー
int KEY_NUM1		=0;		// NUM1
int KEY_NUM2		=0;		// NUM2
int KEY_NUM3		=0;		// NUM3
int KEY_NUM4		=0;		// NUM4
int KEY_NUM5		=0;		// NUM5
int KEY_NUM6		=0;		// NUM6
int KEY_NUM7		=0;		// NUM7
int KEY_NUM8		=0;		// NUM8
int KEY_NUM9		=0;		// NUM9
int KEY_NUM0		=0;		// NUM0
int KEY_1			=0;		// 1
int KEY_2			=0;		// 2
int KEY_3			=0;		// 3
int KEY_4			=0;		// 4
int KEY_5			=0;		// 5
int KEY_6			=0;		// 6
int KEY_7			=0;		// 7
int KEY_8			=0;		// 8
int KEY_9			=0;		// 9
int KEY_0			=0;		// 0
int KEY_Q			=0;		// Q
int KEY_W			=0;		// W
int KEY_E			=0;		// E
int KEY_R			=0;		// R
int KEY_A			=0;		// A
int KEY_S			=0;		// S
int KEY_D			=0;		// D
int KEY_F			=0;		// F
int KEY_Z			=0;		// Z
int KEY_X			=0;		// X
int KEY_C			=0;		// C
int KEY_V			=0;		// V
*/
