/*=====ヘッダコメント=====
*@プログラム名	：stageSelect.h
*@説明・概要	：ステージセレクトついて
*@製作者	：AT_12D_192_51吉田叶聖
*/
#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "main.h"

// ポーズメニュー
typedef enum {
	GAME = 0,		// ゲームスタート
	END,			// 終了
	TITLE_MAX
} TITLE_SELECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTSelect(void);
void UninitTSelect(void);
void UpdateTSelect(void);
void DrawTSelect(void);

void SetTitleSelect(void);
TITLE_SELECT GetTitleSelect(void);
void ResetTitleSelect(void);
