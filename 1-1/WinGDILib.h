/**
 * Windows用C言語学習ライブラリ
 *
 * history:	
 */
#ifndef __WINGDILIB_H__
#define __WINGDILIB_H__
#define _DEBUG_ 1
//-------------------------------------------------------
//インクルードファイル↓　（変更不可）
//-------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>

//#include <windowsx.h>
//-------------------------------------------------------
//ライブラリ動作環境用　定数定義↓　（値の変更可能だが分からない場合は触れないように）
//-------------------------------------------------------
// Windowタイトル
#define WINDOW_TITLE	"WIN GAME"

// Windowスクリーンサイズ
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

// デフォルトスクリーンモード （FULL_SCREENかWINDOW_SCREENか）
//#define DEFAULT_SCREEN FULL_SCREEN
#define DEFAULT_SCREEN WINDOW_SCREEN

#define VIEW_MOUSE 		1	// マウスカーソルを表示するかどうか？ 1:表示 0:非表示

// デフォルト背景色
#define BACKGROUND_COLOR RGB(0,0,0)

// Window FPS（Frame Per Second　一秒間に何回画面を更新するか）
#define WINDOW_FPS		60

//-------------------------------------------------------
//ライブラリ定数定義↓　（変更不可）
//-------------------------------------------------------
// キー状態
#define	FREE_KEY	0 // キーが押されていない状態
#define	PUSH_KEY	1 // キーを押された瞬間
#define	PULL_KEY	2 // キーが放された瞬間
#define	HOLD_KEY	3 // キーが押されている状態

// スクリーンモード
#define FULL_SCREEN 0
#define WINDOW_SCREEN 1

// 塗りつぶし
#define MODE_NOFILL 0	// 塗りつぶさない
#define MODE_FILL	1	// 塗りつぶす

// 円弧Arc()関数の図形モード
#define MODE_ARC	0	// 円弧
#define MODE_PIE	1	// 扇形
#define MODE_CHORD	2	// 弓形

// 円周率
#define PAI 3.14159265358979323846
//角度をラジアンに変換する(t = 0～360)
#define RAD(t) 	( PAI/180*(t) )


//サウンド関連

//MIDIファイルの最大登録数
#define MIDI_MAX 16

#ifdef _DEBUG_

// デバッグ用マクロ -----------------------
// 画面出力
#define D_IN char _str[256]; sprintf(_str,
#define D_OUT(x,y) ); prtText(x,y,_str,RGB(255,255,255));

// メッセージボックス出力
#define DM_IN char _str[256]; sprintf(_str,
#define DM_OUT ); MessageBox(g_hwnd,_str,"DBG_MESSAGE",MB_OK);

#endif

//-------------------------------------------------------
//グローバル変数定義↓　（変更不可）
//-------------------------------------------------------
// Window ハンドル
extern HWND g_hwnd;
// HDC
extern HDC g_hdc;
// バックバッファ用ビットマップ
extern HBITMAP g_buffBMP;
// バックバッファ用DC
extern HDC g_buffDC;

// スクリーンモード
extern int g_screen ;

// マウス状態
extern int MOUSE_PX;						// マウスの現在のX座標
extern int MOUSE_PY;						// マウスの現在のY座標

// ループフラグ
extern bool g_fLoop;	// true:ループ　false:ループ中止

// キーボード、ボタンの状態
// 状態監視したいキーを追加する場合はGetDeviceState()に追記
extern int KEY_LBUTTON;		// マウス左ボタン
extern int KEY_RBUTTON;		// マウス右ボタン
extern int KEY_UP;			// ↑キー
extern int KEY_DOWN;		// ↓キー
extern int KEY_LEFT;		// ←キー
extern int KEY_RIGHT;		// →キー
extern int KEY_RETURN;		// ENTERキー
extern int KEY_SPACE;		// スペースキー
extern int KEY_SHIFT;		// SHIFTキー
extern int KEY_CONTROL;		// CTRLキー
extern int KEY_ALT;			// ALTキーVK_MENU 
extern int KEY_ESCAPE;		// ESCキー
extern int KEY_NUM1;			// NUM1
extern int KEY_NUM2;			// NUM2
extern int KEY_NUM3;			// NUM3
extern int KEY_NUM4;			// NUM4
extern int KEY_NUM5;			// NUM5
extern int KEY_NUM6;			// NUM6
extern int KEY_NUM7;			// NUM7
extern int KEY_NUM8;			// NUM8
extern int KEY_NUM9;			// NUM9
extern int KEY_NUM0;			// NUM0
extern int KEY_1;			// 1
extern int KEY_2;			// 2
extern int KEY_3;			// 3
extern int KEY_4;			// 4
extern int KEY_5;			// 5
extern int KEY_6;			// 6
extern int KEY_7;			// 7
extern int KEY_8;			// 8
extern int KEY_9;			// 9
extern int KEY_0;			// 0
extern int KEY_Q;			// Q
extern int KEY_W;			// W
extern int KEY_E;			// E
extern int KEY_R;			// R
extern int KEY_A;			// A
extern int KEY_S;			// S
extern int KEY_D;			// D
extern int KEY_F;			// F
extern int KEY_Z;			// Z
extern int KEY_X;			// X
extern int KEY_C;			// C
extern int KEY_V;			// V

//-------------------------------------------------------
//型宣言↓　（変更不可）
//-------------------------------------------------------
// 矩形情報構造体
typedef struct _TRect{
	int px;			// 左上X座標
	int py;			// 左上Y座標
	int width;		// 幅
	int height;		// 高さ
}TRect;

// ベクトル構造体
typedef struct _TVec{
	double vx;
	double vy;
}TVec;

//-------------------------------------------------------
//基本関数プロトタイプ宣言↓　（変更不可）
//-------------------------------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE, LPCSTR);
BOOL InitInstance(HINSTANCE, LPCSTR, int);
//<++ 2015/11/20 fhirashima修正
void SetClientRect(HWND hwnd, int cx, int cy);
//++>
//-------------------------------------------------------
// システム関連
bool InitLib();
void ExitLib();
void GetDisplayMode(DEVMODE *);		// 現在のディスプレイモードを取得する
bool FullScreenMode();				// Windowをフルスクリーンモードにする

//-------------------------------------------------------
// ゲームメイン関連
void InitGame();					// ゲーム起動前初期化処理
void GameMain();					// ゲームメインループ
void ExitGameMain();				// ゲーム終了処理（解放とかいろいろ）

//-------------------------------------------------------
// 入力デバイス関連
void GetDeviceState();				// 入力デバイスの状態取得　内部用
void GetKey(unsigned int ,int *);	// キーの状態取得			内部用

//-------------------------------------------------------
// 描画用関数関連

// 情報取得 -----------------
COLORREF getPenColor();								// 現在のペンの色を取得する
COLORREF getBrushColor();							// 現在のブラシの色を取得する
COLORREF getPix(int x, int y);						// 指定した位置の色情報を取得する

// テキスト描画 -----------------
void prtPix( int px, int py, COLORREF col=-1);		// 指定した位置に点を描画する
void prtText(int, int, char *, COLORREF col=-1);	// 指定した位置に文字を表示する
void prtText(TRect *, char *, int format=DT_WORDBREAK, COLORREF col=-1 ); // 指定した矩形に文字を表示する

// 各種情報変更 -----------------
void setColor(COLORREF, COLORREF);					// ペンとブラシの色を変える
void setPen(COLORREF , int style = -1, int width=-1);	// ペンを変更する
void setNullPen();									// ペンの色を透明にする
void setBrush(COLORREF);							// ブラシ（塗りつぶし色）を変更する
void setPolyFillMode(int mode);						// 多角形の塗り潰しモードを変更する
void setNullBrush();								// ブラシの色を透明にする
void setBgColor(COLORREF);							// 背景色の変更
void setFont(LOGFONT *lf);							// フォントを変更する
void setCharacterExtra(int);						// 文字間隔を指定する

// 図形描画 -----------------
void prtLine(int, int,int ,int );					// 始点から終点まで線をひく
void prtPolyline(POINT *p, int);					// 複数の頂点を持つ線を描く
void prtTriangle(int ,int, int, int, int, int, int fill=1);		// 三角形を描く
void prtRect(int, int, int, int, int fill=MODE_FILL);		//四角形を描く 
void prtRoundRect(int, int, int, int, int, int, int fill=MODE_FILL); //丸みのある四角形を描く 
void prtPolygon( POINT *, int, int fill=MODE_FILL);			// 複数の頂点を結ぶ多角形を描く
void prtEllipse( int px, int py , int width, int height, int fill=MODE_FILL);	// 円（楕円）を描画する
void prtArc( int px, int py , int width, int height, int sx, int sy, int ex, int ey, int mode=MODE_ARC,int fill=MODE_FILL); // 円弧を描画する


//-------------------------------------------------------
// サウンド関連
void Play(char *);							// 音を鳴らす(wav形式）
void Stop();								// 音を止める
bool midiOpen(char *, int);					// midiファイルをオープンする										
void midiPlay(int);							// midiファイルを再生する（1回のみ）		
void midiSetLoop(int no);					// midiファイルをループモードにセットする										
void midiLoopPlay(int no);					// midiファイルをループモードで再生する
void midiStop(int);							// midiファイルを停止する			
void midiSeekStart(int);					// midiファイルの演奏位置を最初に戻す										
void midiClose(int);						// midiファイルを閉じる
void midiLoopCheckPlay();					// 内部処理用					
void midiAllClose();						// 内部処理用			

//-------------------------------------------------------
// その他関連
int getFPS();						// FPSを取得する
void checkFPS(ULONG);				// FPS計算（内部用）

//-------------------------------------------------------
// ゲームプログラム関連
bool hit(TRect *, TRect *);			// 当り判定
double vec_rad(TVec *p, TVec *e);	// 2点間を結んだベクトルの角度（ラジアン）を返す（pを中心として）
double vec_rad(double _px, double _py, double _ex, double _ey); 	// 2点間を結んだベクトルの角度（ラジアン）を返す（(_px,_py)を中心として）

double distance(TVec *, TVec *);					// 2点間の距離を返す
double distance(double, double, double, double );	// 2点間の距離を返す別バージョン

TVec unit_vec(TVec *, TVec *);		// 単位ベクトル算出
TVec unit_vec(double, double, double, double); 	// 単位ベクトル算出

TVec rotate(TVec *, double );		// 原点を中心としたベクトルの回転
TVec rotate(TVec *,TVec *, double); // 中心点を指定したベクトルの回転
TVec rotate(double _vx, double _vy, double rad); // 原点を中心としたベクトルの回転　別バージョン
TVec rotate(double _vx, double _vy, double _cx, double _cy,  double rad);	// 中心点を指定したベクトルの回転　別バージョン

#endif