/*=====ヘッダコメント=====
*@プログラム名	：score.cpp
*@説明・概要	：スコア表示について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*///*****************************************************************************
// インクルード部
//*****************************************************************************
#include "score.h"
#include "number.h"
#include "Texture.h"
#include "polygon.h"
#include "Time.h"
#include "object.h"
#include "debugproc.h"
#include "fade.h"
#include "scene.h"
#include "clear.h"

// マクロ定義
#define CHRCNT_SCORE 2
#define POS_X_SCORE 0.0f
#define POS_Y_SCORE ((SCREEN_HEIGHT-80.0f)*0.5f+16.0f)
#define MAX_SCORE GetCnt()
// グローバル変数
static ID3D11ShaderResourceView* g_pTexture;
static int g_nScore;
// 初期化
HRESULT InitScore()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// 変数初期化
	g_nScore = MAX_SCORE;
	return hr;
}
// 終了処理
void UninitScore()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}
// 更新
void UpdateScore()
{
	if (g_nScore == 0)
	{
		StartCutOut(SCENE_CLEAR);
		Clearflg(true);
	}
}
// 描画
void DrawScore()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	// 明るい緑
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	// スコア表示
	DrawNumber(XMFLOAT2(POS_X_SCORE, POS_Y_SCORE),(unsigned)g_nScore, CHRCNT_SCORE);
	// 元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);
}
//// スコア加算
void AddScore(int nScore)
{
	g_nScore += nScore;
}
// スコア取得
int GetScore() {
	return g_nScore;
}