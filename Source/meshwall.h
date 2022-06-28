/*=====ヘッダコメント=====
*@プログラム名	：meshwall.h
*@説明・概要	：メッシュを利用した壁について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "main.h"

//**************************************
// 描画部分
//**************************************
enum EDrawPart {
	DRAWPART_ALL = 0,		// 区別なく表示
	DRAWPART_OPAQUE,		// 不透明のみ描画
	DRAWPART_TRANSLUCENT,	// 半透明のみ描画

	DRAWPART_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshWall(void);
HRESULT SetMeshWall(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col,
	int nNumBlockX, int nNumBlockY, XMFLOAT2 sizeBlock);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(EDrawPart dp = DRAWPART_ALL);
