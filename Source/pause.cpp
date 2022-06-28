/*=====ヘッダコメント=====
*@プログラム名	：pause.cpp
*@説明・概要	：ポーズ画面などについて
*@製作者	：AT_12D_192_51吉田叶聖
*@更新者	：AT_12D_192_51吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/12/20
*/
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_PAUSE_MENU		(3)			// ポーズメニュー数
#define	PAUSE_MENU_WIDTH	(320.0f)	// ポーズメニュー幅
#define	PAUSE_MENU_HEIGHT	(60.0f)		// ポーズメニュー高さ
#define	PAUSE_MENU_POS_X	(0.0f)		// ポーズメニュー位置(X座標)
#define	PAUSE_MENU_POS_Y	(PAUSE_MENU_INTERVAL)	// ポーズメニュー位置(Y座標)
#define	PAUSE_MENU_INTERVAL	(100.0f)	// ポーズメニュー間隔
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(340.0f)	// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)
#define GAMEPAD_LEFT_STICK_DEADZONE  (7800)	//左スティックのデッドゾーン
#define MAX_TEXTURE	(3)


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[6] = {nullptr};	// テクスチャへのポインタ

static PAUSE_MENU g_nSelectPMenu = PAUSE_MENU_CONTINUE;		// 選択中のメニューNo.
static float g_fCurve;
static float g_fCol = 0.0f;
static int g_nTexture;
static bool g_bJoyStick;

static LPCWSTR c_aFileNamePauseMenu[MAX_TEXTURE] =
{
	L"data/texture/retry.png",	// リトライ
	L"data/texture/select.png",	// セレクト
	L"data/texture/rule.png",	// ルール
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPause(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntPauseMenu = 0; nCntPauseMenu < MAX_TEXTURE; ++nCntPauseMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,									// デバイスへのポインタ
			c_aFileNamePauseMenu[nCntPauseMenu],	// ファイルの名前
			&g_pTextures[nCntPauseMenu]);			// 読み込むメモリー
	}

	g_nSelectPMenu = PAUSE_MENU_CONTINUE;
	g_fCurve = 0.0f;
	g_nTexture = 0;
	g_bJoyStick = false;

	// 効果音初期化
	//g_pSE_SHIZUKU = CreateSound(SE_SHIZUKU_PATH);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	// テクスチャの開放
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{
	DWORD JoyCount = GetJoyCount();
	LONG JoyY = GetJoyY(JOYSTICKID1);
	CSound::SetVolume(SE_PI, 0.2f);
	//デッドゾーン以下ならすべて０にする
	if ((JoyY <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyY > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyY = 0;
	}
	if (JoyCount >= 1) {
		//左スティックが上に押された
		if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false)
		{
			CSound::Play(SE_PI);
			g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
			SetPauseMenu();
			g_bJoyStick = true;
		}
		//左スティックが下に押された
		if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false)
		{
			CSound::Play(SE_PI);
			g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + 1) % NUM_PAUSE_MENU);
			SetPauseMenu();
			g_bJoyStick = true;
		}
		if (JoyY == 0l)
		{
			g_bJoyStick = false;
		}
	}
	if (GetKeyRepeat(VK_UP)) {
		CSound::Play(SE_PI);
		g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}
	else if ( GetKeyRepeat(VK_DOWN)) {
		CSound::Play(SE_PI);
		g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}

	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	// 反射光の設定
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

}


//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetPolygonTexture(nullptr);
	SetPolygonSize(PLATE_WIDTH, PLATE_HEIGHT);
	SetPolygonPos(PLATE_POS_X, PLATE_POS_Y);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(g_fCol, g_fCol, g_fCol);
	SetPolygonAlpha(1.0f);
	DrawPolygon(pDeviceContext);

	SetPolygonSize(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		SetPolygonPos(PAUSE_MENU_POS_X, PAUSE_MENU_POS_Y - nCntPauseMenu * PAUSE_MENU_INTERVAL);

		if (nCntPauseMenu == g_nSelectPMenu) {
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else {
			//選ばれてないほう
			SetPolygonColor(0.0f, 0.0f, 0.0f);
		}

		// テクスチャの設定
		SetPolygonTexture(g_pTextures[nCntPauseMenu]);
		// ポリゴンの描画
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetPauseMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
PAUSE_MENU GetPauseMenu(void)
{
	return g_nSelectPMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetPauseMenu(void)
{
	g_nSelectPMenu = PAUSE_MENU_CONTINUE;
	SetPauseMenu();
}
