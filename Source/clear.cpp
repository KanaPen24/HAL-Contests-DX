/*=====ヘッダコメント=====
*@プログラム名	：Clear.cpp
*@説明・概要	：クリア画面などについて
*@製作者	：AT_12D_192_51吉田叶聖
*@更新者	：AT_12D_192_51吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/12/20
*/

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Clear.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"
#include "stageSelect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_CLEAR_MENU		(3)			// クリアメニュー数
#define	CLEAR_MENU_WIDTH	(320.0f)	// クリアメニュー幅
#define	CLEAR_MENU_HEIGHT	(60.0f)		// クリアメニュー高さ
#define	CLEAR_MENU_POS_X	(-380.0f)	// クリアメニュー位置(X座標)
#define	CLEAR_MENU_POS_Y	(-250.0f)	// クリアメニュー位置(Y座標)
#define	CLEAR_MENU_INTERVAL	(400.0f)	// クリアメニュー間隔
#define	PLATE_WIDTH			(SCREEN_WIDTH)	// プレートの幅
#define	PLATE_HEIGHT		(60.0f)		// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(CLEAR_MENU_POS_Y)		// プレートの位置(Y座標)
#define GAMEPAD_LEFT_STICK_DEADZONE  (7800)	//左スティックのデッドゾーン
#define MAX_TEXTURE	(4)
#define POS_X_BG		(0.0f)			//背景のX座標
#define POS_Y_BG		(0.0f)			//背景のY座標
#define WIDTH_BG		(SCREEN_WIDTH)	//横幅
#define HEIGHT_BG		(SCREEN_HEIGHT)	//立幅
#define TEX_BG			(0)				//背景の画像数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[4] = { nullptr };	// テクスチャへのポインタ

static CLEAR_MENU g_nSelectCMenu =CLEAR_MENU_SELECT;		// 選択中のメニューNo.
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static int g_nTexture;
static bool g_bJoyStick;
static bool		g_bSound1;		// 一回だけ流す
static bool		g_bSound2;		// 一回だけ流す
static bool		g_bSound3;		// 一回だけ流す
static bool		g_bClear;		//クリアフラグ

static LPCWSTR c_aFileNameClearMenu[MAX_TEXTURE] =
{
	L"data/texture/stageclear.jpg",	// 背景
	L"data/texture/select.png",	// セレクト
	L"data/texture/retry.png",	// リトライ
	L"data/texture/Gtitle.png"
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitClear(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntClearMenu = 0; nCntClearMenu < MAX_TEXTURE; ++nCntClearMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,			// デバイスへのポインタ
			c_aFileNameClearMenu[nCntClearMenu],	// ファイルの名前
			&g_pTextures[nCntClearMenu]);			// 読み込むメモリー
	}

	g_nSelectCMenu = CLEAR_MENU_SELECT;
	g_fCurve = 0.0f;
	g_nTexture = 0;
	g_bJoyStick = false;
	g_bClear = false;

	// 効果音初期化
	// BGM再生
	CSound::SetVolume(BGM_TITLE, 0.04f);
	CSound::Play(BGM_TITLE);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitClear(void)
{
	// 音停止
	CSound::Stop(BGM_TITLE);
	CSound::Stop(SE_SELECT);

	// テクスチャの開放
	for (int nCntClearMenu = 0; nCntClearMenu < NUM_CLEAR_MENU; ++nCntClearMenu) {
		SAFE_RELEASE(g_pTextures[nCntClearMenu]);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateClear(void)
{
	DWORD JoyCount = GetJoyCount();
	LONG JoyX = GetJoyX(JOYSTICKID1);
	CSound::SetVolume(SE_PI, 0.2f);
	//デッドゾーン以下ならすべて０にする
	if ((JoyX <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyX > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyX = 0l;
	}

	if (JoyCount >= 1) {
		//左スティックが左に押された
		if (JoyX <= -GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false)
		{
			CSound::Play(SE_PI);
			g_nSelectCMenu = (CLEAR_MENU)((g_nSelectCMenu + NUM_CLEAR_MENU - 1) % NUM_CLEAR_MENU);
			SetClearMenu();
			g_bJoyStick = true;
		}
		//左スティックが右に押された
		if (JoyX >= GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false)
		{
			CSound::Play(SE_PI);
			g_nSelectCMenu = (CLEAR_MENU)((g_nSelectCMenu + 1) % NUM_CLEAR_MENU);
			SetClearMenu();
			g_bJoyStick = true;
		}
		if (JoyX == 0l)
		{
			g_bJoyStick = false;
		}
	}
	if (GetKeyRepeat(VK_LEFT)) {
		CSound::Play(SE_PI);
		g_nSelectCMenu = (CLEAR_MENU)((g_nSelectCMenu + NUM_CLEAR_MENU - 1) % NUM_CLEAR_MENU);
		SetClearMenu();
	}
	else if (GetKeyRepeat(VK_RIGHT)) {
		CSound::Play(SE_PI);
		g_nSelectCMenu = (CLEAR_MENU)((g_nSelectCMenu + 1) % NUM_CLEAR_MENU);
		SetClearMenu();
	}

	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}
	//ゲームクリアメニューの選択
	if (g_bClear&&GetFade() == FADE_NONE) {
		//[Enter]が押された？
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {
			//選択中のメニュー項目により分岐
			switch (GetClearMenu())
			{
			case CLEAR_MENU_SELECT:
				StartFadeOut(SCENE_SELECT);
				CSound::SetVolume(SE_SELECT, 0.2f);
				CSound::Play(SE_SELECT);
				break;
			case CLEAR_MENU_RETRY:
				StartFadeOut(SCENE_GAME);
				CSound::SetVolume(SE_SELECT, 0.2f);
				CSound::Play(SE_SELECT);
				break;
			case CLEAR_MENU_TITLE:
				StartFadeOut(SCENE_TITLE);
				CSound::SetVolume(SE_SELECT, 0.2f);
				CSound::Play(SE_SELECT);
				break;
			}
		}
		switch (GetClearMenu())
		{
		case CLEAR_MENU_SELECT:
			g_bSound2 = true;
			g_bSound3 = true;

			if (g_bSound1 == true)
			{
				CSound::SetVolume(SE_PI, 0.2f);
				CSound::Play(SE_PI);
				g_bSound1 = false;
			}
			break;

		case CLEAR_MENU_RETRY:
			g_bSound1 = true;
			g_bSound3 = true;

			if (g_bSound2 == true)
			{
				CSound::SetVolume(SE_PI, 0.2f);
				CSound::Play(SE_PI);
				g_bSound2 = false;
			}
			break;

		case CLEAR_MENU_TITLE:
			g_bSound1 = true;
			g_bSound2 = true;

			if (g_bSound3 == true)
			{
				CSound::SetVolume(SE_PI, 0.2f);
				CSound::Play(SE_PI);
				g_bSound3 = false;
			}
			break;
		}
	}
	// 反射光の設定
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

}


//=============================================================================
// 描画処理
//=============================================================================
void DrawClear(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetPolygonTexture(nullptr);
	SetPolygonSize(WIDTH_BG, HEIGHT_BG);
	SetPolygonPos(POS_X_BG, POS_Y_BG);
	SetPolygonTexture(g_pTextures[0]);
	DrawPolygon(pDeviceContext);

	
	SetPolygonSize(CLEAR_MENU_WIDTH, CLEAR_MENU_HEIGHT);
	for (int nCntClearMenu = 0; nCntClearMenu < NUM_CLEAR_MENU; ++nCntClearMenu) {
		SetPolygonPos(CLEAR_MENU_POS_X + nCntClearMenu * CLEAR_MENU_INTERVAL, CLEAR_MENU_POS_Y);

		if (nCntClearMenu == g_nSelectCMenu) {
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else {
			//選ばれてないほう
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		if (nCntClearMenu == 1)	SetPolygonSize(CLEAR_MENU_WIDTH, CLEAR_MENU_HEIGHT + 120.0f);

		// テクスチャの設定
		SetPolygonTexture(g_pTextures[nCntClearMenu+1]);
		// ポリゴンの描画
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// クリアメニューの設定
//=============================================================================
void SetClearMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// クリアメニューの取得
//=============================================================================
CLEAR_MENU GetClearMenu(void)
{
	return g_nSelectCMenu;
}

//=============================================================================
// クリアメニューのリセット
//=============================================================================
void ResetClearMenu(void)
{
	g_nSelectCMenu = CLEAR_MENU_SELECT;
	SetClearMenu();
}
//=============================================================================
// クリア画面表示フラグ
// @param	[bool]	Clearflg	クリアしたかどうか
// @return	[bool]	g_bClear	クリア情報
//=============================================================================
bool Clearflg(bool Clearflg)
{
	g_bClear = Clearflg;
	return g_bClear;
}
