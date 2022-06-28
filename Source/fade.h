/*=====ヘッダコメント=====
*@プログラム名	：fade.h
*@説明・概要	：フェードイン/アウトについて
*@製作者	：AT_12D_192_51吉田叶聖
*@更新者	：AT_12D_192_51吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/
#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "scene.h"
//*****************************************************************************
// 定数定義
//*****************************************************************************
enum EFade
{
	FADE_NONE = 0, // 何もしていない
	FADE_IN, // フェードイン処理中
	FADE_OUT, // フェードアウト処理中
	MAX_FADE
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFade(); // 初期化
void UninitFade();	// 終了処理
void UpdateFade();	// 更新
void DrawFade();	// 描画
void StartFadeOut(EScene eNext);	// フェードアウト開始
void StartCutOut(EScene eNext);		// カットアウト開始
EFade GetFade();	// 状態取得
void SetFadeColor(float fR, float fG, float fB);	// フェードアウト色設定