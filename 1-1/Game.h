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

//-------------------------------------------------------
//オリジナルインクルードファイル↓　
//-------------------------------------------------------

//-------------------------------------------------------
//オリジナル定数定義↓　
//-------------------------------------------------------
#define SCREEN_W		800
#define SCREEN_H		600

// 形状
#define RECT			1	// 四角形
#define CIRCLE			2	// 円形
#define VLINE			3	// 縦線
#define HLINE			4	// 横線
#define U_TRI			5	// 上向きの三角形
#define D_TRI			6	// 下向きの三角形


// 自機関連
#define SHIP_W			40
#define SHIP_H			40
#define SHIP_X_POS		SCREEN_W/2
#define SHIP_Y_POS		SCREEN_H-60
#define SHIP_SPEED		5

// 弾関連
#define BULLET_W		1
#define BULLET_H		6
#define BULLET_SPEED	10

#define BULLET_MAX		1000


// 敵関連
#define ENEMY_MAX		10

#define ENEMY_W			40
#define ENEMY_H			40
#define ENEMY_X_POS		SCREEN_W/2
#define ENEMY_Y_POS		60
#define ENEMY_SPEED		8


//-------------------------------------------------------
//オリジナル型定義（構造体）↓　
//-------------------------------------------------------
typedef struct {
	double x, y;		// 位置座標
	double w, h;		// 幅、高さ
	double vx, vy;		// 移動方向
	double speed;		// 速度
	int		type;		// 形状
	COLORREF out_color;	// 外枠の色
	COLORREF in_color;	// 内側の色
	bool	move;		// 移動フラグ
	bool	view;		// 表示非表示フラグ
}TObject;
//-------------------------------------------------------
//オリジナルグローバル変数定義↓
//-------------------------------------------------------
// 自機関連
TObject ship = {
	SHIP_X_POS,SHIP_Y_POS,		// 位置情報
	SHIP_W,SHIP_H,				// 幅高さ
	0,0,						// 移動方向
	SHIP_SPEED,					// スピード
	U_TRI,						// 形状
	RGB(0,128,0),				// 外枠の色
	RGB(0,255,0),				// 内側の色
	false,						// 移動フラグ
	true						// 表示フラグ
};


// 弾関連
TObject bullet[BULLET_MAX];
int bullet_index;						// 弾の配列Index


// 敵関連
TObject enemy[ENEMY_MAX];


//-------------------------------------------------------
//オリジナル関数プロトタイプ宣言↓
//-------------------------------------------------------
void draw(TObject*);

#endif