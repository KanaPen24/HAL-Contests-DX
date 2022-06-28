/*=====ヘッダコメント=====
*@プログラム名	：clear.h
*@説明・概要	：ポーズ画面などについて
*@製作者	：AT_12D_192_51吉田叶聖
*@更新者	：AT_12D_192_51吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/12/20
*/
#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "main.h"

// ポーズメニュー
typedef enum {
	CLEAR_MENU_SELECT=0,	// ステージセレクト
	CLEAR_MENU_RETRY,		// リトライ
	CLEAR_MENU_TITLE,
	CLEAR_MENU_MAX
} CLEAR_MENU;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitClear(void);
void UninitClear(void);
void UpdateClear(void);
void DrawClear(void);

void SetClearMenu(void);
CLEAR_MENU GetClearMenu(void);
void ResetClearMenu(void);
bool Clearflg(bool Clearflg);
