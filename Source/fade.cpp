/*=====ヘッダコメント=====
*@プログラム名	：fade.h
*@説明・概要	：フェードイン/アウトについて
*@製作者	：AT_12D_192_51吉田叶聖
*@更新者	：AT_12D_192_51吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "fade.h"
#include "polygon.h"
#include "Sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_RATE (0.02f) // フェードイン/アウトの刻み
//*****************************************************************************
// グローバル宣言
//*****************************************************************************
static float g_fRed = 0.0f; // フェードアウト色(R)
static float g_fGreen = 0.0f;//(G)
static float g_fBlue = 0.0f;//(B)
static float g_fAlpha = 1.0f; // 不透明度
static EFade g_eFade = FADE_IN; // 状態
static EScene g_eNext = SCENE_TITLE; // 次のシーン
//=============================================================================
// フェードの初期化処理
//=============================================================================
HRESULT InitFade() {
	g_fAlpha = 1.0f;
	g_eFade = FADE_IN;
	g_eNext = SCENE_TITLE;
	return S_OK;
}
//=============================================================================
// フェードの終了処理
//=============================================================================
void UninitFade() {
	g_fAlpha = 0.0f;
	g_eFade = FADE_NONE;
}
//=============================================================================
// フェードの更新処理
//=============================================================================
void UpdateFade() {
	switch (g_eFade) {
	case FADE_NONE:
		break;
	case FADE_OUT:
		g_fAlpha += FADE_RATE; // 不透明度を増す
		if (g_fAlpha >= 1.0f) {
			// フェードイン処理に切り替え
			g_fAlpha = 1.0f;
			g_eFade = FADE_IN;
			// シーン切替
			SetScene(g_eNext);
		}
		// ボリュームもフェードアウト
		CSound::SetVolume(1.0f - g_fAlpha);
		break;
	case FADE_IN:
		g_fAlpha -= FADE_RATE; // 透明度を増す
		if (g_fAlpha <= 0.0f) {
			// フェードイン終了
			g_fAlpha = 0.0f;
			g_eFade = FADE_NONE;
		}
		// ボリュームもフェードイン
		CSound::SetVolume(1.0f - g_fAlpha);
		break;
	}
}
//=============================================================================
// フェードの描画処理
//=============================================================================
void DrawFade() {
	// 画面全体に半透明ポリゴンを描画
	SetPolygonPos(0.0f, 0.0f);
	SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetPolygonTexture(nullptr);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(g_fRed, g_fGreen, g_fBlue);
	SetPolygonAlpha(g_fAlpha);
	DrawPolygon(GetDeviceContext());
	// 元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);
}
//=============================================================================
// フェードアウト開始
//=============================================================================
void StartFadeOut(EScene eNext)
{
	if (g_eFade != FADE_OUT) {
		g_eFade = FADE_OUT;
		g_fAlpha = 0.0f;
		g_eNext = eNext;
	}
}

//=============================================================================
// カットアウト処理
//=============================================================================
void StartCutOut(EScene eNext)
{
	g_eNext = eNext;
	SetScene(g_eNext);
}
//=============================================================================
// 状態処理
//=============================================================================
EFade GetFade() {
	return g_eFade;
}
//=============================================================================
//　フェードアウト色設定
//=============================================================================
void SetFadeColor(float fR, float fG, float fB
)
{
	g_fRed = fR;
	g_fGreen = fG;
	g_fBlue = fB;
}