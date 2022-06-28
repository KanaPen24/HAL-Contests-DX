/*=====ヘッダコメント=====
*@プログラム名	：pause.cpp
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
	PAUSE_MENU_CONTINUE = 0,	// コンティニュー
	PAUSE_MENU_SELECT,			// ステージセレクト
	PAUSE_MENU_RULE,			// ルール
	PAUSE_MENU_MAX
} PAUSE_MENU;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

void SetPauseMenu(void);
PAUSE_MENU GetPauseMenu(void);
void ResetPauseMenu(void);
