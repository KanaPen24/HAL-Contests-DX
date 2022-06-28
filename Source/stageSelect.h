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
	STAGE_1 = 0,		// ステージ1
	STAGE_2,			// ステージ2
	STAGE_3,			// ステージ3
	STAGE_MAX
} STAGE_SELECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSSelect(void);
void UninitSSelect(void);
void UpdateSSelect(void);
void DrawSSelect(void);

void SetStageSelect(void);
STAGE_SELECT GetStageSelect(void);
void ResetStageSelect(void);
