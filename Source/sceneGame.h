/*=====ヘッダコメント=====
*@プログラム名	：sceneStage1.h
*@説明・概要	：ゲームシーンについて
*@製作者	：AT_12D_192_51吉田叶聖
*@更新者	：AT_12D_192_51吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/
#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "main.h"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGame();	// 初期化
void UninitGame();	// 終了処理
void UpdateGame();	// 更新
void DrawGame();		// 描画

