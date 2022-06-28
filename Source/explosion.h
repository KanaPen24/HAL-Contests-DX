/*=====ヘッダコメント=====
*@プログラム名	：explosion.h
*@説明・概要	：爆発エフェクトについて
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "main.h"

// 関数プロトタイプ
HRESULT InitExplosion();	// 初期化
void UninitExplosion();	// 終了処理
void UpdateExplosion();	// 更新
void DrawExplosion();		// 描画

int StartExplosion(XMFLOAT3 pos, XMFLOAT2 size);
