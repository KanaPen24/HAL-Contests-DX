/*=====ヘッダコメント=====
*@プログラム名	：stageSelect.cpp
*@説明・概要	：ステージセレクトついて
*@製作者	：AT_12D_192_51吉田叶聖
*/
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "titleSelect.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "Texture.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_SELECT_MENU		(2)			// セレクトメニュー数
#define	SELECT_MENU_WIDTH	(500.0f)	// セレクトメニュー幅
#define	SELECT_MENU_HEIGHT	(100.0f)	// セレクトメニュー高さ
#define	SELECT_MENU_POS_X	(25.0f)		// セレクトメニュー位置(X座標)
#define	SELECT_MENU_POS_Y	(-100.0f)	// セレクトメニュー位置(Y座標)
#define	SELECT_MENU_INTERVAL	(150.0f)	// セレクトメニュー間隔
#define GAMEPAD_LEFT_STICK_DEADZONE  (7800)	//左スティックのデッドゾーン
#define MAX_TEXTURE	(2)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[2] = { nullptr };	// テクスチャへのポインタ

static TITLE_SELECT g_nSelectTMenu = GAME;		// 選択中のメニューNo.
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int g_texture = 0;
static bool g_bJoyStick;


static LPCWSTR c_aFileNameTitleMenu[MAX_TEXTURE] =
{
	L"data/texture/start.png",	// game
	L"data/texture/end.png",	// end
};

//=============================================================================
// ステージセレクト初期化処理
//=============================================================================
HRESULT InitTSelect(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntPauseMenu = 0; nCntPauseMenu < MAX_TEXTURE; ++nCntPauseMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,			// デバイスへのポインタ
			c_aFileNameTitleMenu[nCntPauseMenu],	// ファイルの名前
			&g_pTextures[nCntPauseMenu]);			// 読み込むメモリー
	}

	g_nSelectTMenu = GAME;
	g_fCurve = 0.0f;
	g_bJoyStick = false;

	// 効果音初期化
	//g_pSE_SHIZUKU = CreateSound(SE_SHIZUKU_PATH);
	//一時停止初期化
	return hr;
}

//=============================================================================
// ステージセレクト終了処理
//=============================================================================
void UninitTSelect(void)
{
	// テクスチャの開放
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_SELECT_MENU; ++nCntPauseMenu) {
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
}

//=============================================================================
// ステージセレクト更新処理
//=============================================================================
void UpdateTSelect(void)
{
	DWORD JoyCount = GetJoyCount();
	LONG JoyY = GetJoyY(JOYSTICKID1);
	//デッドゾーン以下ならすべて０にする
	if ((JoyY <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyY > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyY = 0;
	}
	if (JoyCount >= 1) {
		//ジョイスティックがうえ
		if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false){
			//	CSound::Play(SE_SHIZUKU);
			g_nSelectTMenu = (TITLE_SELECT)((g_nSelectTMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
			SetTitleSelect();
			g_bJoyStick = true;
		}
		//ジョイスティックが下
		if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false){
			//	CSound::Play(SE_SHIZUKU);
			g_nSelectTMenu = (TITLE_SELECT)((g_nSelectTMenu + 1) % NUM_SELECT_MENU);
			SetTitleSelect();
			g_bJoyStick = true;
		}
		if (JoyY == 0l)
		{
			g_bJoyStick = false;
		}
	}
	if (GetKeyRepeat(VK_UP))
	{
		g_nSelectTMenu = (TITLE_SELECT)((g_nSelectTMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
		SetTitleSelect();
	}
	if (GetKeyRepeat(VK_DOWN))
	{
		g_nSelectTMenu = (TITLE_SELECT)((g_nSelectTMenu + 1) % NUM_SELECT_MENU);
		SetTitleSelect();
	}

	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	// 反射光の設定
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

}

//=============================================================================
// ステージセレクト描画処理
//=============================================================================
void DrawTSelect(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetPolygonTexture(nullptr);
	SetPolygonSize(SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT);
	for (int nCntTitleMenu = 0; nCntTitleMenu < NUM_SELECT_MENU; ++nCntTitleMenu) {
		SetPolygonPos(SELECT_MENU_POS_X, SELECT_MENU_POS_Y - nCntTitleMenu * SELECT_MENU_INTERVAL);

		if (nCntTitleMenu == g_nSelectTMenu) {
			//選んでいるメニューの方
			SetPolygonColor(1.0f, 1.0f, 1.0f);
			SetPolygonAlpha(1.f);
			SetPolygonSize(SELECT_MENU_WIDTH + 50.f, SELECT_MENU_HEIGHT + 50.f);
		}
		else {
			//選ばれてないほう
			SetPolygonColor(0.3f, 0.3f, 0.3f);
			SetPolygonAlpha(0.5f);
			SetPolygonSize(SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT);
		}
		// テクスチャの設定
		SetPolygonTexture(g_pTextures[nCntTitleMenu]);

		// ポリゴンの描画
		DrawPolygon(pDeviceContext);
	}
	SetPolygonColor(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// ステージセレクトメニューの設定
//=============================================================================
void SetTitleSelect(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ステージセレクトメニューの取得
//=============================================================================
TITLE_SELECT GetTitleSelect(void)
{
	return g_nSelectTMenu;
}

//=============================================================================
// ステージセレクトメニューのリセット
//=============================================================================
void ResetTitleSelect(void)
{
	g_nSelectTMenu = GAME;
	//CSound::Play(SE_SHIZUKU);
	SetTitleSelect();
}
