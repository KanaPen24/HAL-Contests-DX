/*=====ヘッダコメント=====
*@プログラム名	：sceneRule.cpp
*@説明・概要	：ルールシーンについて
*@製作者	：AT_12D_192_51吉田叶聖
*@更新者	：AT_12D_192_51吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "sceneRule.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "fade.h"
#include "Sound.h"
#include "scene.h"
#include "debugproc.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Rule_TEXTURENAME		L"data/texture/setumei.jpg"			// 読み込むテクスチャファイル名

#define Rule_POS_X			(0)			// ポリゴンの表示位置
#define Rule_POS_Y			(0)			// ポリゴンの表示位置
#define Rule_SIZE_X			(1280)		// ポリゴンのサイズ
#define Rule_SIZE_Y			(720)		// ポリゴンのサイズ

#define	START_POS_X			(0)			// スタートボタンの位置(X座標)
#define	START_POS_Y			(-220)		// スタートボタンの位置(Y座標)
#define	START_WIDTH			(480)		// スタートボタンの幅
#define	START_HEIGHT		(120)		// スタートボタンの高さ

#define	COUNT_APPEAR		(60)		// スタートボタン出現までの時間
#define	INTERVAL_DISP		(60)		// スタートボタン点滅の時間


#define BLINK_TIMER		(30)			//点滅時間で使用
#define BLINK_START_TIMER	(5)			//点滅開始時間

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[1];			// テクスチャへのポインタ
static int							g_nCountAppear = 0;		//
static int							g_nCountDisp = 0;		//
static bool							g_bDispStart = false;	//
static bool							g_bSkip = false;		// スタートボタン押下
static int g_nBlink;
static int g_nStart;

//=============================================================================
// ルール画面の初期化処理
//=============================================================================
HRESULT InitRule(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// テクスチャの読み込み
	LPCWSTR pszFName[] = {
		Rule_TEXTURENAME,
	};
	for (int i = 0; i < _countof(g_pTextures); ++i) {
		hr = CreateTextureFromFile(pDevice, pszFName[i], &g_pTextures[i]);
		if (FAILED(hr)) {
			MessageBoxW(GetMainWnd(), pszFName[i], L"テクスチャ読込エラー", MB_OK);
			return hr;
		}
	}

	// 変数初期化
	g_nCountAppear = COUNT_APPEAR;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_bSkip = false;

	// BGM再生開始
	//CSound::Play(BGM_TITLE);

	return hr;
}

//=============================================================================
// ルール画面の終了処理
//=============================================================================
void UninitRule(void)
{
	// BGM再生停止
	//CSound::Stop(BGM_TITLE);

	// テクスチャ解放
	for (int i = 0; i < _countof(g_pTextures); ++i) {
		SAFE_RELEASE(g_pTextures[i]);
	}
	CSound::Stop(SE_SELECT);
}

//=============================================================================
// ルール画面の更新処理
//=============================================================================
void UpdateRule(void)
{
	--g_nCountAppear;
	if (g_nCountAppear <= 0) {
		g_nCountDisp = (g_nCountDisp + 1) % 80;
		g_bDispStart = (g_nCountDisp <= INTERVAL_DISP);
	}

	if (g_bSkip) {
		g_bDispStart = g_nCountAppear & 2;
	}

	// [Enter]またはスペースキーで開始
	//if (!g_bSkip && (GetKeyRelease(VK_RETURN) || GetKeyRelease(VK_SPACE) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1))) {
	//	// 選択音
	//	//CSound::Play(SE_SELECT);
	//	// フェードアウト後シーン遷移
	//	StartFadeOut(SCENE_TITLE);	//タイトルに移動

	//	g_bSkip = true;
	//}
	// タイトル画面へ
	if (GetKeyRelease(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//エンター入力
		CSound::SetVolume(SE_SELECT, 0.3f);
		CSound::Play(SE_SELECT);	//効果音
		StartFadeOut(SCENE_SELECT);//セレクトに移動
		g_nStart = 1; // 開始フラグ
		return;
	}
#ifdef _DEBUG
	PrintDebugProc("ﾘｻﾞﾙﾄ:1\n");
	PrintDebugProc("ﾙｰﾙ  :2\n");
	PrintDebugProc("ｾﾚｸﾄ :3\n");
	PrintDebugProc("ﾀｲﾄﾙ :4\n");
	PrintDebugProc("ｹﾞｰﾑ :5\n");
	PrintDebugProc("ｹﾞｰﾑｵｰﾊﾞｰ :6\n");
#endif // _DEBUG
}

//=============================================================================
// チュートリアル画面描画
//=============================================================================
void DrawRule(void)
{
	SetZBuffer(false);
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetPolygonTexture(g_pTextures[0]);
	SetPolygonSize(Rule_SIZE_X, Rule_SIZE_Y);
	SetPolygonPos(Rule_POS_X, Rule_POS_Y);
	DrawPolygon(pDeviceContext);

	SetZBuffer(true);
}
