/*=====ヘッダコメント=====
*@プログラム名	：number.h
*@説明・概要	：数字の読み込みや表示について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "main.h"
// マクロ定義
#define NUMBER_SIZE_X 35.0f // 文字幅
#define NUMBER_SIZE_Y 50.0f // 文字高さ
// 関数プロトタイプ
HRESULT InitNumber(); // 初期化
void UninitNumber(); // 終了処理
void DrawNumber( // 描画
	XMFLOAT2 vPos,		// 表示位置(左上)
	unsigned uNumber,	// 表示数値
	int nWidth,			// 表示桁数
	float fSizeX = NUMBER_SIZE_X,	// 1文字の幅
	float fSizeY = NUMBER_SIZE_Y);	// 1文字の高さ