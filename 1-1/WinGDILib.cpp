/**
 * Windows用C言語学習ライブラリ
 *
 * history:	
 *
 *
 */
#include "./WinGDILib.h"
//  リンクするライブラリファイル
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "imm32.lib")
//-------------------------------------------------------
//グローバル変数定義↓　（変更不可）
//-------------------------------------------------------
// Window ハンドル
HWND g_hwnd=NULL;
// HDC
HDC g_hdc=NULL;
// バックバッファ用ビットマップ
HBITMAP g_buffBMP=NULL;
// バックバッファ用DC
HDC g_buffDC=NULL;

// スクリーンモード
int g_screen = DEFAULT_SCREEN;
//int g_screen = WINDOW_SCREEN;

// ペンオブジェクト
HPEN g_pen=NULL;
// ペン情報取得用オブジェクト
LOGPEN g_penInfo;
// ブラシオブジェクト
HBRUSH g_brush=NULL; 
// ブラシ情報取得用オブジェクト
LOGBRUSH g_burshInfo;
// 背景用ブラシ
HBRUSH g_bgcolor=NULL;

// フォントオブジェクト
HFONT g_font=NULL;
// フォント情報取得用オブジェクト
LOGFONT g_fontInfo;

// ループフラグ
bool g_fLoop=true;

// マウス状態
int MOUSE_PX=0;		// マウスの現在のX座標
int MOUSE_PY=0;		// マウスの現在のY座標

// キーボード、ボタンの状態
// 状態監視したいキーを追加する場合はGetDeviceState()内に追記。またWinGDLib.hにextern宣言を追加
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



//-------------------------------------------------------
//ソース内グローバル変数定義↓　（変更不可）
//-------------------------------------------------------
// フレーム制御用変数
ULONG	l_waitTime = (ULONG)(1000/WINDOW_FPS+1);	// 1フレームに関するフレーム秒数（ミリ秒）
ULONG	l_currentTime=0;				// 現在の秒数（ミリ秒）
ULONG	l_lastTime=0;					// フレーム更新する前に最後に取得した秒数（ミリ秒）
ULONG	l_delayTime=0;					// フレームにディレイをかけたい場合
double	d_frameTime=0.0;				// 経過時間

// FPS計算用
ULONG	l_fpsTime=0;					// FPSを計算するために秒数を保持する変数
int		i_fpsCount=0;					// FPSのカウンター
int		i_fps=0;						// 現在のFPSを表示


// midiサウンド用変数
int midiNo[MIDI_MAX]={0};						// midiファイルの状態を格納する変数 0:未OPEN(or CLOSE 1:OPEN済 2:停止中 3:再生（1回）4:ループ再生

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
// Windows Main 関連
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//-------------------------------------------------------
//WinMain
//-------------------------------------------------------
int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
                   LPSTR lpsCmdLine, int nCmdShow)
{
    char szClassName[] = "ClassName";

	//二重起動の防止
	if(FindWindow(szClassName,WINDOW_TITLE) != NULL)return 0;

	MSG msg;

    
	// RegisterClass
    if (!hPrevInst) {
        if (!InitApp(hCurInst, szClassName))
            return FALSE;
    }
	// CreateWindow
    if (!InitInstance(hCurInst, szClassName, nCmdShow)) {
        return FALSE;
    }
	// マウスカーソルを表示するか？
	if(VIEW_MOUSE == 0) 	ShowCursor(FALSE);

	// フルスクリーンモードの場合
	if(g_screen== FULL_SCREEN)
	{
		//IMEパッドを隠蔽
		ImmAssociateContext(g_hwnd, NULL);
		// フルスクリーンへ
		FullScreenMode();
	}

	//ライブラリ初期化処理
	if(!InitLib())	return false;

	// ゲーム起動前初期化処理
	InitGame();

	//フレームレート用の時間取得
	l_currentTime = timeGetTime();
	l_lastTime = l_currentTime;
	
	//ゲームループ開始
	while(g_fLoop)
	{
		//メッセージ取得
		if(PeekMessage( &msg , NULL , 0 , 0 , PM_REMOVE ))
		{
			if( msg.message == WM_QUIT ) g_fLoop = false;
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		} else {
			static int n=0;
			static UINT m=0;
			//メインループ
			l_currentTime = timeGetTime();	// 現在の経過時間を取得
			//指定FPSの秒数を超えているか？
			if( (l_currentTime - l_lastTime) >= (l_waitTime + l_delayTime) )
			{
				checkFPS(l_currentTime);
				// 実際にフレーム更新に必要とした経過時間を保持（移動計算等の補間に使用することもあり。念のため）
				d_frameTime= ( l_currentTime - l_lastTime ) * 0.001; // ミリ秒単位では値が大きすぎるので秒単位に変換
				l_lastTime = l_currentTime;		//最新時間を保持
					
				// 画面をｸﾘｱ
				SelectObject(g_buffDC,g_bgcolor);
				PatBlt(g_buffDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,PATCOPY);
				//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

				//入力デバイスの状態取得
				GetDeviceState();
				
				//ゲームメイン
				GameMain();

				//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
				// 全体を転送領域とする
				InvalidateRect(g_hwnd,NULL,FALSE);
		//		// バックバッファを転送 (WM_PAINTイベント参照)
				UpdateWindow (g_hwnd);             // 再描画

			}
		}
	}
    return msg.wParam;
}

//-------------------------------------------------------
//InitApp
//-------------------------------------------------------
BOOL InitApp(HINSTANCE hInst, LPCSTR szClassName)
{ 
	// Windowクラスの生成
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;				//ウインドウスタイルを設定 
    wc.lpfnWndProc = WndProc;						// Windowプロシージャのポインタ
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);		//アイコンの指定
//	wc.hIcon = LoadIcon(NULL, "IDI_GAME");
	wc.hCursor	= LoadCursor(NULL , IDC_ARROW);		// マウスカーソルの指定
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//ウインドウの背景色
    wc.lpszMenuName = NULL;							// メニュー名
    wc.lpszClassName = (LPCSTR)szClassName;
    return (RegisterClass(&wc));
}
//<++ 2015/11/20 fhirashima修正
//-------------------------------------------------------
//クライアントサイズ設定
//-------------------------------------------------------
BOOL SetClientSize(HWND hWnd, int width, int height)
{
	RECT rw, rc;
	GetWindowRect(hWnd, &rw);
	GetClientRect(hWnd, &rc);

	int new_width = (rw.right - rw.left) - (rc.right - rc.left) + width;
	int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + height;

	return SetWindowPos(hWnd, NULL, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);
}
//++>
//-------------------------------------------------------
//InitInstance
//-------------------------------------------------------
BOOL InitInstance(HINSTANCE hInst, LPCSTR szClassName, int nCmdShow)
{
    g_hwnd = CreateWindow(szClassName,
            WINDOW_TITLE,			//タイトルバーにこの名前が表示されます　.hを参照
			WS_CAPTION |
			WS_SYSMENU | 
			WS_MINIMIZEBOX,			//ウィンドウの種類
            CW_USEDEFAULT,			//最初にWindowを配置するX座標
            CW_USEDEFAULT,			//最初にWindowを配置するY座標
            SCREEN_WIDTH,			//Windowの幅
            SCREEN_HEIGHT,			//Window高さ
            NULL,					//親ウィンドウのハンドル、親を作るときはNULL
            NULL,					//メニューハンドル、クラスメニューを使うときはNULL
            hInst,					//インスタンスハンドル
            NULL);
    if (!g_hwnd)
        return FALSE;
    SetClientSize(g_hwnd, SCREEN_WIDTH, SCREEN_HEIGHT);
    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);
    return TRUE;
}
//-------------------------------------------------------
//Windowプロシージャ
//-------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
	PAINTSTRUCT ps;

    switch (msg) {
		case WM_KEYDOWN:
            switch( wp ){
                case VK_ESCAPE:
					// プログラムを終了
					PostMessage(g_hwnd, WM_CLOSE, 0, 0);	
					break;
                case VK_F12:
/*					// 調整中
					// ディスプレイモードの切り替えが可能
					if(g_screen == FULL_SCREEN)
					{	// Windowモードへ
						ChangeDisplaySettings(&def_dm, 0);
						g_screen = WINDOW_SCREEN;
					}else if(g_screen == WINDOW_SCREEN){
						//IMEパッドを隠蔽
						ImmAssociateContext (g_hwnd, NULL);
						// フルスクリーンへ
						FullScreenMode();
						g_screen = FULL_SCREEN;
					}
*/	                break;
            }
			break;
		// マウスが移動したさいに発生するイベント
 		case WM_MOUSEMOVE:	
			MOUSE_PX=LOWORD(lp);
			MOUSE_PY=HIWORD(lp);
			break;

		// マウスの左ボタンが押された場合に発生するイベント
		case WM_LBUTTONDOWN:
			break;

		// マウスの左ボタンが離されたときに発生するイベント
		case WM_LBUTTONUP:		
			break;

		// マウスの左ボタンがダブルクリック
		case WM_LBUTTONDBLCLK:
			break;

		// マウスの右ボタンが押された場合に発生するイベント
		case WM_RBUTTONDOWN:
			break;

		// マウスの右ボタンが離されたときに発生するイベント
		case WM_RBUTTONUP:		
			break;

		// マウスの右ボタンがダブルクリック
		case WM_RBUTTONDBLCLK:
			break;

		case MM_MCINOTIFY:	// MCI関連のメッセージの処理

			// 演奏の正常終了以外は何もしない
			if(wp != MCI_NOTIFY_SUCCESSFUL) return 0;
			// ループモードのファイルが存在すれば再生する
			midiLoopCheckPlay();
			break;

		case WM_COMMAND:
			break;

		case WM_PAINT:
				// バックバッファに転送
				g_hdc = BeginPaint (g_hwnd, &ps);
				BitBlt(g_hdc, 0,0,SCREEN_WIDTH,SCREEN_HEIGHT,g_buffDC,0,0,SRCCOPY);
				EndPaint( hwnd , &ps );
				break;

        case WM_CLOSE:
			id = MessageBox(hwnd,
				(LPCTSTR)"終了しますか",
				(LPCTSTR)"終了確認",
				MB_OKCANCEL | MB_ICONQUESTION);
			if (id == IDOK) {
				g_fLoop = false;
				// ゲーム解放処理
				ExitGameMain();
				// ライブラリ終了処理
				ExitLib();
				DestroyWindow(hwnd);
			}
			if (id == IDCANCEL) {
				return FALSE;
			}
            break;
        case WM_DESTROY:

			// midiファイルをオープンしている場合はｸﾛｰｽﾞ
			midiAllClose();
            PostQuitMessage(0);
            break;
        default:
            return (DefWindowProc(hwnd, msg, wp, lp));
    }
    return 0L;
}

//-------------------------------------------------------
//ライブラリ初期化処理
//-------------------------------------------------------
bool InitLib()
{
	// バックバッファの作成
	g_hdc = GetDC(g_hwnd);
	g_buffBMP=CreateCompatibleBitmap(g_hdc, SCREEN_WIDTH, SCREEN_HEIGHT);	// バックバッファ用ビットマップ
	g_buffDC = CreateCompatibleDC(g_hdc);	// バックバッファ用のDC
	SelectObject(g_buffDC,g_buffBMP); //バックバッファビットマップをDCに関連付け	
	ReleaseDC(g_hwnd,g_hdc);

	// デフォルトペンの作成
	g_pen = CreatePen(PS_SOLID, 0, RGB(0,0,0));
	SelectObject(g_buffDC,g_pen);
	// デフォルトブラシの作成
	g_brush = CreateSolidBrush(RGB(0,0,0));
	SelectObject(g_buffDC,g_brush);
	// 背景用ブラシの作成
	g_bgcolor = CreateSolidBrush(BACKGROUND_COLOR);

	//timeGetTime()の精度を1msに設定 これを忘れるとFPSの値が痛い目にあう。
	timeBeginPeriod(1);	
	
	return true;
}
//-------------------------------------------------------
//ライブラリ終了処理
//-------------------------------------------------------
void ExitLib()
{
	timeEndPeriod(1);	

	// ペンの解放
	DeleteObject(g_pen);
	// ブラシの解放
	DeleteObject(g_brush);
	// 背景用ブラシの開放
	DeleteObject(g_bgcolor);
	// バックバッファ用DCの解放
	DeleteDC(g_buffDC);
	// バックバッファ用ビットマップの解放
	DeleteObject(g_buffBMP);
}

//-------------------------------------------------------
//ディスプレイモードの取得
//-------------------------------------------------------
void GetDisplayMode(DEVMODE *dm)
{
/*  この関数は調整中
	HDC	hdc = GetDC(NULL);
	dm->dmSize = sizeof(DEVMODE);
	dm->dmPelsWidth  = GetDeviceCaps(hdc, HORZRES);
	dm->dmPelsHeight = GetDeviceCaps(hdc, VERTRES);
	dm->dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

	// Windows NT/2000用
	if ((GetVersion() & 0x80000000) == 0) {				
		dm->dmFields |= DM_DISPLAYFREQUENCY;
		dm->dmDisplayFrequency = GetDeviceCaps(hdc, VREFRESH);
	}
	ReleaseDC(NULL,hdc);
*/
}
//-------------------------------------------------------
// ディスプレイモードをフルスクリーンに変更
//-------------------------------------------------------
bool FullScreenMode()
{
	// 現在の画面色数を取得する
	HDC hdc ;
	hdc = GetDC(g_hwnd) ;
	int colors = GetDeviceCaps(hdc, BITSPIXEL) ;
	ReleaseDC(g_hwnd, hdc) ;

	// ディスプレイドライバが設定出来る画面設定を見て行き、現在の色数で
	// 640*480の設定を探す
	DEVMODE	dm;

	DWORD modeno ;
	// ディスプレイモードを検索
	for(modeno = 0 ;; modeno ++) {
		if(EnumDisplaySettings(NULL, modeno, &dm) == FALSE) {
		// サーチ終了。つまり変更したいモードには変えられない
			MessageBox(NULL, "このディスプレイカードではフルスクリーンに出来ません。", "Error", MB_OK) ;
			return false ;
		}
		// 変更したいモードか？
		if((dm.dmPelsWidth == SCREEN_WIDTH)
		&& (dm.dmPelsHeight == SCREEN_HEIGHT)
		&& (dm.dmBitsPerPel == (unsigned)colors)) {
		// これを採用
			break ;
		}
	}
	// NT/2000系の場合
/*	if ((GetVersion() & 0x80000000) == 0)
	{				
		dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
		dm.dmDisplayFrequency = GetDeviceCaps(hdc, VREFRESH);
	} */
	if (SetWindowLong(g_hwnd,GWL_STYLE,WS_POPUP | WS_VISIBLE))
		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
			if (SetWindowPos(g_hwnd,HWND_TOPMOST,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,SWP_FRAMECHANGED | SWP_NOSIZE | SWP_DRAWFRAME | SWP_SHOWWINDOW))
				return true;
	return false;
}


//-------------------------------------------------------
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
// 入力デバイス関連
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
/**
 * 入力デバイスの状態取得
 * @return void 
 *
 * history:
 */
void GetDeviceState()
{
	// マウスの状態取得
	GetKey(VK_LBUTTON,		&KEY_LBUTTON);	// マウス左ボタン
	GetKey(VK_RBUTTON,		&KEY_RBUTTON);	// マウス右ボタン

	// キーボードの状態取得
	GetKey(VK_UP,			&KEY_UP);		// ↑キー
	GetKey(VK_DOWN,			&KEY_DOWN);		// ↓キー
	GetKey(VK_LEFT,			&KEY_LEFT);		// ←キー
	GetKey(VK_RIGHT,		&KEY_RIGHT);	// →キー
	GetKey(VK_RETURN,		&KEY_RETURN);	// ENTERキー
	GetKey(VK_SPACE,		&KEY_SPACE);	// スペースキー
	GetKey(VK_SHIFT,		&KEY_SHIFT);	// SHIFTキー
	GetKey(VK_CONTROL,		&KEY_CONTROL);	// CTRLキー
	GetKey(VK_MENU,			&KEY_ALT);		// ALTキー 
	GetKey(VK_ESCAPE,		&KEY_ESCAPE);	// ESCキー
	GetKey(VK_NUMPAD1,		&KEY_NUM1);		// NUM1
	GetKey(VK_NUMPAD2,		&KEY_NUM2);		// NUM2
	GetKey(VK_NUMPAD3,		&KEY_NUM3);		// NUM3
	GetKey(VK_NUMPAD4,		&KEY_NUM4);		// NUM4
	GetKey(VK_NUMPAD5,		&KEY_NUM5);		// NUM5
	GetKey(VK_NUMPAD6,		&KEY_NUM6);		// NUM6
	GetKey(VK_NUMPAD7,		&KEY_NUM7);		// NUM7
	GetKey(VK_NUMPAD8,		&KEY_NUM8);		// NUM8
	GetKey(VK_NUMPAD9,		&KEY_NUM9);		// NUM9
	GetKey(VK_NUMPAD0,		&KEY_NUM0);		// NUM0
	GetKey('1',				&KEY_1);		// 1
	GetKey('2',				&KEY_2);		// 2
	GetKey('3',				&KEY_3);		// 3
	GetKey('4',				&KEY_4);		// 4
	GetKey('5',				&KEY_5);		// 5
	GetKey('6',				&KEY_6);		// 6
	GetKey('7',				&KEY_7);		// 7
	GetKey('8',				&KEY_8);		// 8
	GetKey('9',				&KEY_9);		// 9
	GetKey('0',				&KEY_0);		// 0
	GetKey('Q',				&KEY_Q);		// Q
	GetKey('W',				&KEY_W);		// W
	GetKey('E',				&KEY_E);		// E
	GetKey('R',				&KEY_R);		// R
	GetKey('A',				&KEY_A);		// A
	GetKey('S',				&KEY_S);		// S
	GetKey('D',				&KEY_D);		// D
	GetKey('F',				&KEY_F);		// F
	GetKey('Z',				&KEY_Z);		// Z
	GetKey('X',				&KEY_X);		// X
	GetKey('C',				&KEY_C);		// C
	GetKey('V',				&KEY_V);		// V
}

/**
 * キー状態の取得
 * @param unsigned_int cd アスキーコード or 仮想キーコード ( VK_…… )  
 * @param int* flag キー状態（戻り値の代わり）
 *				FREE_KEY = キーが押されていない状態 
 *				PUSH_KEY = キーが押された瞬間     
 *				PULL_KEY = キーが放された瞬間     
 *				HOLD_KEY = キーが押されている状態 
 * @return void 
 *
 * new	2004/10/15
 * edit	2004/10/15
 * history:
 */
void GetKey(UINT cd,int *flag)
{
	if ((GetKeyState(cd)&0x80)==0x80)
	{
		if (*flag==FREE_KEY)
			*flag=PUSH_KEY;
		else
			*flag=HOLD_KEY;
	}
	else
	{
		if (*flag==PUSH_KEY || *flag==HOLD_KEY)
			*flag=PULL_KEY;
		else
			*flag=FREE_KEY;
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
// グラフィック描画関連
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//----------------------------
/**
 * ペンの色を取得する
 * @return COLORREF
 *
 * history:
 */
COLORREF getPenColor()
{
	GetObject(g_pen,sizeof(LOGPEN),&g_penInfo);
	return g_penInfo.lopnColor;
}

//----------------------------
/**
 * ブラシの色を取得する
 * @return COLORREF
 *
 * history:
 */
COLORREF getBrushColor()
{
	GetObject(g_pen,sizeof(LOGBRUSH),&g_burshInfo);
	return g_burshInfo.lbColor;
}

//----------------------------
/**
 * 座標(x,y)の色情報を取得する
 * @return COLORREF
 *
 * history:
 */
COLORREF getPix(int x, int y)
{
	return GetPixel(g_buffDC,x,y);
}
//----------------------------
/**
 * ペンとブラシの色を変更する
 * @param COLORREF penCol	変更したい色  
 * @param COLORREF brushCol	変更したい色  
 * @return void 
 *
 * history:
 */
void setColor(COLORREF penCol, COLORREF brushCol)
{
	setPen(penCol);
	setBrush(brushCol);
	return;
}

//----------------------------
/**
 * ペンの色を変更する
 * @param COLORREF col	変更したい色  
 * @param int style		スタイル  default:-1 既存のスタイルを使用
 *			PS_SOLID 実線のペン 
 *			PS_DASH 破線のペン 
 *			PS_DOT 点線のペン 
 *			PS_DASHDOT 一点鎖線のペン 
 *			PS_DASHDOTDOT 二点鎖線のペン 
 *			PS_NULL 空のペン　描画無し
 * @param int width	ペンの幅 default:-1 （既存の幅を使用)
 * @return void 
 *
 * history:
 */
void setPen(COLORREF col, int style, int width)
{
	if(style == -1 || width == -1){
		GetObject(g_pen,sizeof(LOGPEN),&g_penInfo);
		if(style == -1) style=g_penInfo.lopnColor;
		if(width == -1) width=g_penInfo.lopnWidth.x;
	}
	if(g_pen) DeleteObject(g_pen);
	//既存のスタイルや幅を使用
	g_pen = CreatePen(style, width, col);
	SelectObject(g_buffDC,g_pen);
}
//----------------------------
/**
 * ペンの色を透明にする
 * @return void 
 *
 * history:
 */
void setNullPen()
{
	SelectObject(g_buffDC,GetStockObject(NULL_PEN));
}
//----------------------------
/**
 * ブラシの色を変更する
 * @param COLORREF col	変更したい色  
 * @return void 
 *
 * history:
 */
void setBrush(COLORREF col)
{
	if(g_brush) DeleteObject(g_brush);
	g_brush = CreateSolidBrush(col);
	SelectObject(g_buffDC,g_brush);
}
//----------------------------
/**
 * ブラシの色を透明にする
 * @return void 
 *
 * history:
 */
void setNullBrush()
{
	SelectObject(g_buffDC,GetStockObject(NULL_BRUSH));
}
//----------------------------
/**
 * 背景色を変更する
 * @param COLORREF col	変更したい色  
 * @return void 
 *
 * history:
 */
void setBgColor(COLORREF col)
{
	if(g_bgcolor) DeleteObject(g_bgcolor);
	g_bgcolor = CreateSolidBrush(col);
	SelectObject(g_buffDC,g_bgcolor);
}

//----------------------------
/**
 * 多角形の塗り潰しモードを変更する
 * @param int mode	変更したいモード
 *					ALTERNATE 交差する内側のみ塗り潰し
 *					WINDING 多角形の内部を全て塗り潰す
 * @return void 
 *
 * history:
 */
void setPolyFillMode(int mode)
{
	SetPolyFillMode(g_buffDC,mode);
}

//----------------------------
/**
 * フォントをセットする
 * @param LOGFONT*	lf	変更したフォント情報をもつ構造体
 *		参照	http://yokohama.cool.ne.jp/chokuto/urawaza/struct/LOGFONT.html
 *				http://msdn.microsoft.com/library/default.asp?url=/library/en-us/gdi/fontext_1wmq.asp
 * @return void 
 *
 * history:
 */
void setFont(LOGFONT *lf)
{
	if(g_font) DeleteObject(g_font);
	g_font = CreateFontIndirect(lf);
	SelectObject(g_buffDC,g_font);

}
//----------------------------
/**
 * 文字間隔を指定する
 * @param int ce 幅
 * @return void 
 *
 * history:
 */
void setCharacterExtra(int ce)
{
	SetTextCharacterExtra(g_buffDC,ce);
}
//----------------------------
/**
 * 点を描画する
 * @param int px		画面X座標  
 * @param int py		画面Y座標  
 * @param COLORREF col	点の色　default:-1 指定しない場合は既存の色を使う
 * @return void 
 *
 * history:
 */
void prtPix( int px, int py, COLORREF col)
{
	//既存の色を使用
	if(col == -1)	SetPixel(g_buffDC, px, py, getPenColor());
	else SetPixel(g_buffDC, px, py, col);
}
//----------------------------
/**
 * Windowsの指定の位置にテキストを描画する
 * @param int px		画面X座標  
 * @param int py		画面Y座標  
 * @param char* str		表示する文字列
 * @param COLORREF col	文字色 default:-1 指定しない場合は既存の色を使う
 * @return void 
 *
 * history:
 */
void prtText( int px, int py, char *str, COLORREF col)
{
	SetBkMode(g_buffDC,TRANSPARENT); //背景は透明
	if(col != -1)	SetTextColor(g_buffDC,col);
	TextOut(g_buffDC, px , py , str , lstrlen(str));
	SetBkMode(g_buffDC,OPAQUE);		// 塗り潰しモードに戻す
}

//----------------------------
/**
 * Windowsの指定の位置にテキストを描画する（オーバロード）
 * @param TRect *rc		テキストを描画したい矩形
 * @param char* str		表示する文字列
 * @param int format	フォーマット　デフォルトはDT_WORDBREAK
 *						詳しくは http://www.microsoft.com/japan/msdn/library/default.asp?url=/japan/msdn/library/ja/jpgdi/html/_win32_drawtext.asp
 * @param COLORREF col	文字色 default:-1 指定しない場合は既存の色を使う
 * @return void 
 *
 * history:
 */
void prtText(TRect *rc, char *str, int format, COLORREF col )
{
	static RECT rect;
	rect.left = rc->px;
	rect.top = rc->py;
	rect.right = rc->px+rc->width;
	rect.bottom = rc->py+rc->height;

	SetBkMode(g_buffDC,TRANSPARENT); //背景は透明
	if(col != -1)	SetTextColor(g_buffDC,col);
	DrawText(g_buffDC, str, -1, &rect, format);
	SetBkMode(g_buffDC,OPAQUE);		// 塗り潰しモードに戻す
}

//----------------------------
/**
 * 線を描画する
 * @param int sx		始点X 
 * @param int sy		始点Y 
 * @param int ex		終点X 
 * @param int ey		終点Y 
 * @return void 
 *
 * history:
 */
void prtLine( int sx, int sy , int ex, int ey)
{
	MoveToEx(g_buffDC,sx,sy,NULL);
	LineTo(g_buffDC,ex,ey);
}

//----------------------------
/**
 * 複数の頂点を持つ線分を描画する
 * @param POINT *p		頂点を格納したPOINT型の配列 
 * @param int size		配列の要素数 
 * @return void 
 *
 * history:
 */
void prtPolyline( POINT *p, int size)
{
	Polyline(g_buffDC,p,size);
}

//----------------------------
/**
 * 三角形を描画する
 * @param int x1		頂点座標x1 
 * @param int y1		頂点座標y1 
 * @param int x2		頂点座標x2 
 * @param int y2		頂点座標y2 
 * @param int x3		頂点座標x3 
 * @param int y3		頂点座標y3 
 * @return void 
 *
 * history:
 */
void prtTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int fill)
{
	POINT p[] = { x1, y1, x2, y2, x3, y3};
	if(fill == 0) //塗りつぶし無
	{
		HGDIOBJ tmp = SelectObject(g_buffDC,GetStockObject(NULL_BRUSH));
		Polygon(g_buffDC,p,3);
		SelectObject(g_buffDC,tmp);
	}else{	// 塗りつぶし有
		Polygon(g_buffDC,p,3);
	}
}

//----------------------------
/**
 * 四角形を描く
 * @param int px		描画X座標 
 * @param int py		描画Y座標 
 * @param int width		四角形の幅  
 * @param int height	四角形の高さ
 * @param int fill		塗りつぶし 
 *						MODE_NOFILL	塗りつぶさない
 *						MODE_FILL	塗りつぶす (デフォルト)
 * @return void 
 *
 * history:
 */
void prtRect( int px, int py , int width, int height, int fill)
{
	if(fill == 0) //塗りつぶし無
	{
		HGDIOBJ tmp = SelectObject(g_buffDC,GetStockObject(NULL_BRUSH));
		Rectangle(g_buffDC,px,py,px+width,py+height);
		SelectObject(g_buffDC,tmp);
	}else{	// 塗りつぶし有
		Rectangle(g_buffDC,px,py,px+width,py+height);	
	}
}

//----------------------------
/**
 * 丸みのある四角形を描く
 * @param int px		描画X座標 
 * @param int py		描画Y座標 
 * @param int width		四角形の幅  
 * @param int height	四角形の高さ
 * @param int rw		角の丸みの幅 
 * @param int rh		角の丸みの高さ
 * @param int fill		塗りつぶし 
 *						MODE_NOFILL	塗りつぶさない
 *						MODE_FILL	塗りつぶす (デフォルト)
 * @return void 
 *
 * history:
 */
void prtRoundRect( int px, int py , int width, int height, int rw, int rh, int fill)
{
	if(fill == 0) //塗りつぶし無
	{
		HGDIOBJ tmp = SelectObject(g_buffDC,GetStockObject(NULL_BRUSH));
		RoundRect(g_buffDC,px,py,px+width,py+height, rw, rh);
		SelectObject(g_buffDC,tmp);
	}else{	// 塗りつぶし有
		RoundRect(g_buffDC,px,py,px+width,py+height, rw, rh);
	}
}
//----------------------------
/**
 * 複数の頂点を結ぶ多角形を描画する
 * @param POINT *p		頂点を格納したPOINT型の配列 
 * @param int size		配列の要素数 
 * @return void 
 *
 * history:
 */
void prtPolygon( POINT *p, int size, int fill)
{
	if(fill == 0) //塗りつぶし無
	{
		HGDIOBJ tmp = SelectObject(g_buffDC,GetStockObject(NULL_BRUSH));
		Polygon(g_buffDC,p,size);
		SelectObject(g_buffDC,tmp);
	}else{	// 塗りつぶし有
		Polygon(g_buffDC,p,size);
	}
}

//----------------------------
/**
 * 円（楕円）を描画する
 * @param int px		四角形の左上X座標（四角形の内接する円のため）
 * @param int py		四角形の左上Y座標（四角形の内接する円のため）
 * @param int width		幅  
 * @param int height	高さ
 * @param int fill		塗りつぶし 
 *						MODE_NOFILL	塗りつぶさない
 *						MODE_FILL	塗りつぶす (デフォルト)
 * @return void 
 *
 * history:
 */
void prtEllipse( int px, int py , int width, int height, int fill)
{
	if(fill == 0) //塗りつぶし無
	{
		HGDIOBJ tmp = SelectObject(g_buffDC,GetStockObject(NULL_BRUSH));
		Ellipse(g_buffDC,px,py,px+width,py+height);
		SelectObject(g_buffDC,tmp);
	}else{	// 塗りつぶし有
		Ellipse(g_buffDC,px,py,px+width,py+height);	
	}
}
//----------------------------
/**
 * 円弧を描画する
 * @param int px		四角形の左上X座標（四角形の内接する円のため）
 * @param int py		四角形の左上Y座標（四角形の内接する円のため）
 * @param int width		幅  
 * @param int height	高さ
 * @param int sx		円弧の開始X座標（中心から開始点までを結んだ線まで描く）
 * @param int sy		円弧の開始Y座標
 * @param int ex		円弧の終了X座標
 * @param int ey		円弧の終了Y座標
 * @param int mode		図形モード
 *						MODE_ARC	円弧（塗りつぶし無し）
 *						MODE_PIE	扇形（塗りつぶし有り）
 *						MODE_CHORD	弓形（塗りつぶし有り）
 * @param int fill		塗りつぶし 
 *						MODE_NOFILL	塗りつぶさない
 *						MODE_FILL	塗りつぶす (デフォルト)
 * @return void 
 *
 * history:
 */
void prtArc( int px, int py , int width, int height, int sx, int sy, int ex, int ey, int mode, int fill)
{
	switch(mode)
	{
		case MODE_ARC:
			Arc(g_buffDC,px, py, px+width, py+height, sx, sy, ex, ey);
			break;
		case MODE_PIE:
			if(fill == 0) //塗りつぶし無
			{
				HGDIOBJ tmp = SelectObject(g_buffDC,GetStockObject(NULL_BRUSH));
				Pie(g_buffDC,px, py, px+width, py+height, sx, sy, ex, ey);
				SelectObject(g_buffDC,tmp);
			}else{	// 塗りつぶし有
				Pie(g_buffDC,px, py, px+width, py+height, sx, sy, ex, ey);
			}
			break;
		case MODE_CHORD:
			if(fill == 0) //塗りつぶし無
			{
				HGDIOBJ tmp = SelectObject(g_buffDC,GetStockObject(NULL_BRUSH));
				Chord(g_buffDC,px, py, px+width, py+height, sx, sy, ex, ey);
				SelectObject(g_buffDC,tmp);
			}else{	// 塗りつぶし有
				Chord(g_buffDC,px, py, px+width, py+height, sx, sy, ex, ey);
			}
			break;
	}
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
// サウンド関連
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// サウンド関連
//----------------------------
/**
 * サウンドを鳴らす（wav形式）　あまり大きなサイズのWavを指定しないこと
 * @param char filename	サウンドファイル名（パス)
 * @return void 
 *
 * history:
 */
void Play(char *filename)
{
	PlaySound(filename,NULL,SND_ASYNC | SND_FILENAME);
}

/**
 * 音を止める
 * @return void 
 *
 * history:
 */
void Stop()										
{
	PlaySound(NULL,NULL,SND_ASYNC | SND_FILENAME);
}

//----------------------------
/**
 * MIDIを読み込む
 * @param char* filname ファイル名(パス）
 * @param int no 番号
 * @return void 
 *
 * history:
 */
bool midiOpen(char *filename, int no)										
{
	// 未OPENなら
	if(midiNo[no] == 0)
	{
		char str[256];
		sprintf_s(str,"open %s alias MUGIC%02d",filename,no);
		mciSendString(str,NULL,0,NULL);
		midiNo[no] = 1;
	}
	return false;
}

//----------------------------
/**
 * 読み込んだMIDIをプレイする
 * @param int no 番号
 * @return void 
 *
 * history:
 */
void midiPlay(int no)										
{
	// OPEN済み or 停止中なら
	if(midiNo[no] == 1 || midiNo[no] == 2)
	{
		char str[256];
		sprintf_s(str,"play MUGIC%02d NOTIFY",no);
		mciSendString(str,NULL,0,g_hwnd);
		midiNo[no] = 3;		// 再生（1回）
	}
}
//----------------------------
/**
 * ループ再生に変更
 * @param int no 番号
 * @return void 
 *
 * history:
 */
void midiSetLoop(int no)										
{
	// OPEN済以上なら
	if(midiNo[no] >= 1)
	{
		midiNo[no] = 4;
	}
}

//----------------------------
/**
 * ループモードで再生する
 * @param int no 番号
 * @return void 
 *
 * history:
 */
void midiLoopPlay(int no)										
{
	// OPEN済み or 停止中なら
	if(midiNo[no] >= 1)
	{
		char str[256];
		sprintf_s(str,"play MUGIC%02d NOTIFY",no);
		mciSendString(str,NULL,0,g_hwnd);
		midiNo[no] = 4;		//　ループ再生
	}
}

//----------------------------
/**
 * MIDIを一時停止
 * @param int no 番号
 * @return void 
 *
 * history:
 */
void midiStop(int no)										
{
	if(midiNo[no] >= 1)
	{
		char str[256];
		sprintf_s(str,"pause MUGIC%02d",no);
		mciSendString(str,NULL,0,NULL);
		midiNo[no] = 2;
	}
}

//----------------------------
/**
 * MIDIをスタート位置に戻す
 * @param int no 番号
 * @return void 
 *
 * history:
 */
void midiSeekStart(int no)										
{
	if(midiNo[no] >= 1)
	{
		char str[256];
		sprintf_s(str,"seek MUGIC%02d to start",no);
		mciSendString(str,NULL,0,NULL);
	}
}

//----------------------------
/**
 * MIDIを閉じる
 * @param int no 番号
 * @return void 
 *
 * history:
 */
void midiClose(int no)										
{
	if(midiNo[no] >= 1)
	{
		char str[256];
		sprintf_s(str,"close MUGIC%02d",no);
		mciSendString(str,NULL,0,NULL);
		midiNo[no] = 0;
	}
}
//----------------------------
/**
 * ループ再生かを確認し、ループならば最初から演奏しなおす
 * ライブラリ用関数のためユーザが使う必要はなし
 * @return void 
 *
 * history:
 */
void midiLoopCheckPlay()										
{
	int i;
	for(i=0; i<MIDI_MAX; i++)
	{
		if(midiNo[i] == 4){
			midiSeekStart(i);
			midiLoopPlay(i);
		}
	}
}
//----------------------------
/**
 * MIDIをすべて閉じる
 * ライブラリ用関数のためユーザが使う必要はなし
 * @return void 
 *
 * history:
 */
void midiAllClose()										
{
	int i;
	for(i=0; i<MIDI_MAX; i++)
	{
		midiClose(i);	
	}
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
// その他関連
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
/**
 * 現在のFPSを取得する　
 * @return int FPS（一秒間に何度画面が更新されているか）
 *
 * history:
 */
int getFPS()
{
	return i_fps;
}
/**
 * FPSの計算を行う　
 * @return void
 *
 * history:
 */
void checkFPS(ULONG ctime)
{
	i_fpsCount++;
	if(ctime - l_fpsTime >= 1000)
	{
		i_fps = i_fpsCount;
		l_fpsTime = ctime;
		i_fpsCount = 0;
	}
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//ゲーム関連
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
/**
 * 矩形の当り判定を計算する　
 * @param TRect* p 矩形１
 * @param TRect* e 矩形２
 * @return bool true: hit false: no hit
 *
 * history:
 * 当たり判定が1ドットずれていたので修正
 */
bool hit(TRect *p, TRect *e)
{
	if(	p->px < e->px+e->width &&	p->px+p->width > e->px &&
		p->py < e->py+e->height &&	p->py+p->height > e->py){
		return true;
	}
	return false;
}
/**
 * 2点間の距離を返す　
 * @param TVec* p ベクトル１
 * @param TVec* e ベクトル２
 * @return double ラジアン
 *
 * history:
 */
double vec_rad(TVec *p, TVec *e)
{
    static double _w;
	static double _h;
	_w = e->vx - p->vx;
	_h = e->vy - p->vy;
	// 0除算を避ける
    if( _h == 0.0 )
    {
        //　横にまっすぐな場合、
		if( _w >= 0.0 )	return RAD(0);		//　真っ直ぐ右
		else			return RAD(180);		//　真っ直ぐ左
    }
    else if( _w == 0.0 )
    {
		//　縦にまっすぐな場合、
		if( _h > 0 )	return RAD(90);		//　真っ直ぐ上
		else			return RAD(270);		//　真っ直ぐ下
    }
	double at = atan(_h/_w);
	// ラジアン値を0～2πの範囲に調整する
	if( _w > 0 && _h < 0 ) return at += (PAI*2); //RAD(360);
	else if( _w < 0 ) at += PAI;//RAD(180);
	return at; 
}
 /**
 * 2点間を結んだ線分の角度を返す-ラジアン（pを中心をした場合に）　
 * @param double _px ベクトル1X
 * @param double _py ベクトル1Y
 * @param double _ex ベクトル2X
 * @param double _ey ベクトル2Y
 * @return double ラジアン
 *
 * history:
 */
double vec_rad(double _px, double _py, double _ex, double _ey)
{
	// →方向が0°となり時計回りに角度が加算される
	// 距離の算出
    static double _w;
	static double _h;
	_w = _ex - _px;
	_h = _ey - _py;
	// 0除算を避ける
    if( _h == 0.0 )
    {
        //　横にまっすぐな場合、
		if( _w >= 0.0 )	return 0.0; //RAD(0); //　真っ直ぐ右
		else			return PAI; //RAD(180);	//　真っ直ぐ左
    }
    else if( _w == 0.0 )
    {
		//　縦にまっすぐな場合、
		if( _h > 0 )	return (PAI/2); //RAD(90);//　真っ直ぐ上
		else			return (PAI+(PAI/2)); //RAD(270);//　真っ直ぐ下
    }
	double at = atan(_h/_w);
	// ラジアン値を0～2πの範囲に調整する
	if( _w > 0 && _h < 0 ) return at += (PAI*2); //RAD(360);
	else if( _w < 0 ) at += PAI;//RAD(180);
	return at; 
}

  /**
 * 2点間の距離を返す　
 * @param TVec* p ベクトル１
 * @param TVec* e ベクトル２
 * @return double
 *
 * history:
 */
double distance(TVec *p, TVec *e)
{
	return sqrt((e->vx - p->vx)*(e->vx - p->vx) + (e->vy - p->vy)*(e->vy - p->vy)); 
}
 /**
 * 2点間の距離を返す　
 * @param double _px ベクトル1X
 * @param double _py ベクトル1Y
 * @param double _ex ベクトル2X
 * @param double _ey ベクトル2Y
 * @return double 距離
 *
 * history:
 */
double distance(double _px, double _py, double _ex, double _ey)
{
	// 距離の算出
	return sqrt((_ex-_px)*(_ex-_px) + (_ey-_py)*(_ey-_py)); 
}


/**
 * 2点間の距離の単位ベクトルを返す　
 * @param TVec* p ベクトル１
 * @param TVec* e ベクトル２
 * @return TVec 単位ベクトル
 *
 * history:
 */
TVec unit_vec(TVec *p, TVec *e)
{
	static TVec vec;
	static double d;
	d = distance(p,e); 
	if(d == 0){
		vec.vx = 0.0;
		vec.vy = 0.0;
		return vec;
	}
	vec.vx = (e->vx - p->vx) / d;
	vec.vy = (e->vy - p->vy) / d;
	return vec;
}

/**
 * 2点間の距離の単位ベクトルを返す　
 * @param double _px ベクトル1X
 * @param double _py ベクトル1Y
 * @param double _ex ベクトル2X
 * @param double _ey ベクトル2Y
 * @return TVec 単位ベクトル
 *
 * history:
 */
TVec unit_vec(double _px, double _py, double _ex, double _ey)
{
	static TVec vec;
	static double d;
	
	// 距離の算出
	d = distance(_px,_py,_ex,_ey); 
	if(d == 0){
		vec.vx = 0.0;
		vec.vy = 0.0;
		return vec;
	}
	vec.vx = (_ex-_px) / d;
	vec.vy = (_ey-_py) / d;
	return vec;
}

/**
 * 回転させたベクトルを返す　
 * 中心点を軸に回転させたベクトルを返す　
 * @param TVec* v ベクトル
 * @param rad	回転させたい角（ラジアン）
 * @return TVec 回転後のベクトル
 * ベクトル回転の公式 (x1, y1) = (x*cos@ - y*sin@, x*sin@ + y*cos@)
 * history:
 */
TVec rotate(TVec *v, double rad)
{
	static TVec vec;
	static double c,s;
	c = cos(rad);
	s = sin(rad);
	vec.vx = v->vx*c - v->vy*s;
	vec.vy = v->vx*s + v->vy*c;
	return vec;
}
/**
 * 中心点を軸に回転させたベクトルを返す（オーバーロード）　
 * @param TVec* v ベクトル
 * @param TVec* cen 回転軸となる中心座標
 * @param rad	回転させたい角（ラジアン）
 * @return TVec 回転後のベクトル
 * (cx,cy)を中心とした回転の公式 (x1, y1) = ((x-cx)*cos@ - (y-cy)*sin@+cx, (x-cx)*sin@ + (y-cy)*cos@+cy)
 * history:
 */
TVec rotate(TVec *v,TVec *cen,  double rad)
{
	static TVec vec;
	static double c,s;
	c = cos(rad);
	s = sin(rad);
	vec.vx = (v->vx-cen->vx)*c - (v->vy-cen->vy)*s + cen->vx;
	vec.vy = (v->vx-cen->vx)*s + (v->vy-cen->vy)*c + cen->vy;
	return vec;
}
/**
 * 回転させたベクトルを返す（オーバーロード）　
 * 中心点を軸に回転させたベクトルを返す　
 * @param _vx ベクトルX
 * @param _vy ベクトルY
 * @param rad	回転させたい角（ラジアン）
 * @return TVec 回転後のベクトル
 * ベクトル回転の公式 (x1, y1) = (x*cos@ - y*sin@, x*sin@ + y*cos@)
 * history:
 */
TVec rotate(double _vx, double _vy, double rad)
{
	static TVec vec;
	static double c,s;
	c = cos(rad);
	s = sin(rad);
	vec.vx = _vx*c - _vy*s;
	vec.vy = _vx*s + _vy*c;
	return vec;
}
/**
 * 中心点を軸に回転させたベクトルを返す（オーバーロード）　
 * @param double _vx
 * @param double _vy
 * @param double _cx
 * @param double _cx
 * @param TVec* cen 回転軸となる中心座標
 * @param rad	回転させたい角（ラジアン）
 * @return TVec 回転後のベクトル
 * (cx,cy)を中心とした回転の公式 (x1, y1) = ((x-cx)*cos@ - (y-cy)*sin@+cx, (x-cx)*sin@ + (y-cy)*cos@+cy)
 * history:
 */
TVec rotate(double _vx, double _vy, double _cx, double _cy,  double rad)
{
	static TVec vec;
	static double c,s;
	c = cos(rad);
	s = sin(rad);
	vec.vx = (_vx-_cx)*c - (_vy-_cy)*s + _cx;
	vec.vy = (_vx-_cx)*s + (_vy-_cy)*c + _cy;
	return vec;
}

