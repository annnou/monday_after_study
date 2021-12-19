/**
 * ゲームメインループ
 */
#include "Game.h"

/**
 * ゲーム起動前初期化処理
 */
void InitGame()
{
	//------------------------------------------
	//ここにゲームの初期化処理を書く
	//------------------------------------------

	//　弾の初期化
	for (int i = 0; i < BULLET_MAX; i++) {
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].w = BULLET_W;
		bullet[i].h = BULLET_H;
		bullet[i].vx = 0;
		bullet[i].vy = -1;
		bullet[i].speed = BULLET_SPEED;
		bullet[i].type = VLINE;	// 縦線
		bullet[i].out_color = RGB(255, 0, 0);
		bullet[i].in_color = RGB(255, 0, 0);
		bullet[i].move = true;
		bullet[i].view = false;
	}
	bullet_index = 0;			// 弾の配列Index


	for (int i = 0; i < BULLET_MAX; i++) {
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].vx = 0;
		bullet[i].vy = -1;
		bullet[i].speed = BULLET_SPEED;
		bullet[i].view = false;
	}


	// 敵の初期化
	for (int i = 0; i < ENEMY_MAX; i++) {
		enemy[i].x = rand() % SCREEN_W;
		enemy[i].y = rand() % 200 + ENEMY_Y_POS ;
		enemy[i].w = ENEMY_W;
		enemy[i].h = ENEMY_H;
		enemy[i].HP = ENEMY_HP;
		enemy[i].vx = -1;
		enemy[i].vy = 0;
		enemy[i].speed = ENEMY_SPEED ;
		enemy[i].type = CIRCLE;
		enemy[i].out_color = RGB(0, 0, 128);
		enemy[i].in_color = RGB(0, 0, 255);
		enemy[i].move = true;
		enemy[i].view = true;

	}
}

/**
 * ゲームメインループ
 */
void GameMain()
{
	//------------------------------------------
	//ここに↓ゲームのメイン処理を記述
	//------------------------------------------
	// 自機の描画
	draw(&ship);

	// 弾の描画
	for (int i = 0; i < BULLET_MAX; i++) {
		draw(&bullet[i]);
	}
	// 敵の描画
	for (int i = 0; i < ENEMY_MAX; i++) {
		draw(&enemy[i]);

		Time[enemy_num]++;

		if (Time[i] < MAXTIME) {

			// 画面左側
			if (enemy[i].x < enemy[i].w / 2) {
				enemy[i].x = enemy[i].w / 2;
				enemy[i].vx = 1;
			}
			// 画面右側
			if (enemy[i].x > SCREEN_W - enemy[i].w / 2) {
				enemy[i].x = SCREEN_W - enemy[i].w / 2;
				enemy[i].vx = -1;
			}
		}
		else
		{
			enemy[enemy_num].vx = 0;
			enemy[enemy_num].vy = 1;

			if (enemy[enemy_num].y > SCREEN_H) {

				enemy[enemy_num].view = false;
				enemy_num++;
			}
		}
	}

	/*
	// 当たり判定の公式
	// ax ay aw ah と bx by bw bh の矩形同士の当たり判定の公式
	if (ax < bx + bw && ax + aw > bx &&
		ay < by + bh && ay + ah > by) {

		// 当たった！！
	}
	*/

	for (int i = 0; i < ENEMY_MAX; i++) {
		for (int t = 0; t < BULLET_MAX; t++) {
			if (enemy[i].view && bullet[i].view) {
				// 弾と敵の当たり判定
				if (enemy[i].x - enemy[i].w / 2 < bullet[t].x - bullet[t].w / 2 + bullet[t].w &&
					enemy[i].x - enemy[i].w / 2 + enemy[i].w > bullet[t].x - bullet[t].w / 2 &&
					enemy[i].y - enemy[i].h / 2 < bullet[t].y - bullet[t].h / 2 + bullet[t].h &&
					enemy[i].y - enemy[i].h / 2 + enemy[i].h > bullet[t].y - bullet[t].h / 2) {

					enemy[i].HP--;

					int Receive_R_Color = rand() % 255 + 1;
					int Receive_G_Color = rand() % 255 + 1;
					int Receive_B_Color = rand() % 255 + 1;
					enemy[i].in_color = RGB(
						Receive_R_Color,
						Receive_G_Color,
						Receive_B_Color);

					if (enemy[i].HP <= 0)
					{
						enemy[i].view = false;
					}
				}
			}
		}
	}

	for (int i = 0; i < ENEMY_MAX; i++) {
		if (ship.view && enemy[i].view) {
			// 弾と敵の当たり判定
			if (enemy[i].x - enemy[i].w / 2 < ship.x - ship.w / 2 + ship.w &&
				enemy[i].x - enemy[i].w / 2 + enemy[i].w > ship.x - ship.w / 2 &&
				enemy[i].y - enemy[i].h / 2 < ship.y - ship.h / 2 + ship.h &&
				enemy[i].y - enemy[i].h / 2 + enemy[i].h > ship.y - ship.h / 2) {

				player_down = true;
			}
		}
	}

	//-----------------------------
	// 自機の操作

	if (!player_down) {

		//Horizontal
		if (KEY_LEFT == HOLD_KEY) {
			ship.x -= ship.speed;
			if (ship.x < ship.w / 2) {
				ship.x = ship.w / 2;
			}
		}
		else if (KEY_RIGHT == HOLD_KEY) {
			ship.x += ship.speed;
			if (ship.x > SCREEN_W - ship.w / 2) {
				ship.x = SCREEN_W - ship.w / 2;
			}

		}

		//vartical
		if (KEY_UP == HOLD_KEY) {
			ship.y -= ship.speed;
			if (ship.y < ship.h / 2) {
				ship.y = ship.h / 2;
			}
		}
		else if (KEY_DOWN == HOLD_KEY) {
			ship.y += ship.speed;
			if (ship.y > SCREEN_H - ship.h / 2) {
				ship.y = SCREEN_H - ship.h / 2;
			}

		}


		// 弾の発射
		if (KEY_Z == HOLD_KEY) {
			bullet[bullet_index].view = true;
			bullet[bullet_index].x = ship.x;
			bullet[bullet_index].y = ship.y - ship.h / 2;
			bullet_index++;

			int r = rand() % 10 + 265;
			bullet[bullet_index].vx = cos(RAD(r));
			bullet[bullet_index].vy = sin(RAD(r));

			if (bullet_index >= BULLET_MAX) {
				bullet_index = 0;
			}
		}

	}
	else {

		ship.in_color = RGB(255, 0, 0);
	}

	//------------------------------------------
	//ここまで↑
	//------------------------------------------
	//------------------------------------------
	//↓デバッグ用
//	char str[256];
//	sprintf_s(str,"x=%d : y=%d",x,y);
//	prtText(0,0,str,RGB(255,0,0));	

	return;
}

/**
 * ゲーム終了解放処理
 */
void ExitGameMain()
{
	//------------------------------------------
	//ここにゲーム終了後の解放処理を記述
	//------------------------------------------

}

void draw(TObject* o) {
	if (o->view) {
		setPen(o->out_color, PS_SOLID, 3);	// 外色の指定
		setBrush(o->in_color);				// 内色の指定
		switch (o->type) {
		case RECT:	// 四角形
			prtRect(o->x - o->w / 2, o->y - o->h / 2, o->w, o->h, 1);
			break;
		case CIRCLE:// 円形
			prtEllipse(o->x - o->w / 2, o->y - o->h / 2, o->w, o->h, 1);
			break;

		case VLINE:	// 縦線
			prtLine(o->x, o->y - (o->h / 2), o->x, o->y + (o->h / 2));
			break;
		case HLINE:	// 横線 
			prtLine(o->x - (o->w / 2), o->y, o->x + (o->w / 2), o->y);
			break;
		case U_TRI:	// 上三角形
			prtTriangle(o->x, o->y - (o->h / 2),
				o->x - (o->w / 2), o->y + (o->h / 2),
				o->x + (o->w / 2), o->y + (o->h / 2), 1);
			break;
		case D_TRI:	// 下三角形
			prtTriangle(o->x, o->y + (o->h / 2),
				o->x - (o->w / 2), o->y - (o->h / 2),
				o->x + (o->w / 2), o->y - (o->h / 2), 1);
			break;
		}
		// 移動フラグ
		if (o->move) {
			o->x += o->vx * o->speed;
			o->y += o->vy * o->speed;
		}
	}
}


