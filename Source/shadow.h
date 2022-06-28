/*=====ヘッダコメント=====
*@プログラム名	：shadow.h
*@説明・概要	：影について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int CreateShadow(XMFLOAT3 pos, float radius);
void MoveShadow(int nShadow, XMFLOAT3 pos);
void ReleaseShadow(int nShadow);
