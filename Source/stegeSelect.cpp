/*=====ヘッダコメント=====
*@プログラム名	：stageSelect.h
*@説明・概要	：ステージセレクトついて
*@製作者	：AT_12D_192_51吉田叶聖
*/
#include "stageSelect.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "Texture.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_SELECT_MENU		(3)			// セレクトメニュー数
#define	SELECT_MENU_WIDTH	(360.0f)	// セレクトメニュー幅
#define	SELECT_MENU_HEIGHT	(360.0f)		// セレクトメニュー高さ
#define	SELECT_MENU_POS_X	(-380.0f)		// セレクトメニュー位置(X座標)
#define	SELECT_MENU_POS_Y	(0.0f)	// セレクトメニュー位置(Y座標)
#define	SELECT_MENU_INTERVAL	(400.0f)	// セレクトメニュー間隔
#define GAMEPAD_LEFT_STICK_DEADZONE  (7800)	//左スティックのデッドゾーン
#define UNDER_MOVE_RIMIT_Y	(-10.f)			//ロゴの動く下の上限値
#define TOP_MOVE_RIMIT_Y	(10.f)			//ロゴの動く上の上限値
#define ROGO_SPEED		(0.5f)				//ロゴが動くスピード


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };	// テクスチャへのポインタ

static STAGE_SELECT g_nSelectSMenu = STAGE_1;		// 選択中のメニューNo.
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static bool g_bJoyStick;
static bool flg;
static float g_fPosY;

static LPCWSTR c_aFileNameStageMenu[NUM_SELECT_MENU] =
{
	L"data/texture/stage1.png",	// ステージ1
	L"data/texture/stage2.png",	// ステージ2
	L"data/texture/stage3.png",	// ステージ3
};

//=============================================================================
// ステージセレクト初期化処理
//=============================================================================
HRESULT InitSSelect(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_SELECT_MENU; ++nCntPauseMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,									// デバイスへのポインタ
			c_aFileNameStageMenu[nCntPauseMenu],	// ファイルの名前
			&g_pTextures[nCntPauseMenu]);			// 読み込むメモリー
	}

	g_nSelectSMenu = STAGE_1;
	g_fCurve = 0.0f;
	g_bJoyStick = false;
	flg = true;
	g_fPosY = SELECT_MENU_POS_Y;


	// 効果音初期化
	//g_pSE_SHIZUKU = CreateSound(SE_SHIZUKU_PATH);

	return hr;
}

//=============================================================================
// ステージセレクト終了処理
//=============================================================================
void UninitSSelect(void)
{
	// テクスチャの開放
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_SELECT_MENU; ++nCntPauseMenu) {
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
}

//=============================================================================
// ステージセレクト更新処理
//=============================================================================
void UpdateSSelect(void)
{
	DWORD JoyCount = GetJoyCount();
	LONG JoyX = GetJoyX(JOYSTICKID1);
	if ((JoyX <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyX > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyX = 0l;
	}
	if (JoyCount >= 1) {
		if (JoyX <= -GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false) {
			//	CSound::Play(SE_SHIZUKU);
			g_nSelectSMenu = (STAGE_SELECT)((g_nSelectSMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
			SetStageSelect();
			g_bJoyStick = true;
		}
		if (JoyX >= GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false) {
			//	CSound::Play(SE_SHIZUKU);
			g_nSelectSMenu = (STAGE_SELECT)((g_nSelectSMenu + 1) % NUM_SELECT_MENU);
			SetStageSelect();
			g_bJoyStick = true;
		}
		if (JoyX == 0l)
		{
			g_bJoyStick = false;
		}
	}
	if (GetKeyRepeat(VK_LEFT))
	{
		g_nSelectSMenu = (STAGE_SELECT)((g_nSelectSMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
		SetStageSelect();
	}
	if (GetKeyRepeat(VK_RIGHT))
	{
		g_nSelectSMenu = (STAGE_SELECT)((g_nSelectSMenu + 1) % NUM_SELECT_MENU);
		SetStageSelect();
	}

	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	// 反射光の設定
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

	//ロゴのふわふわ感
	if (flg) {
		g_fPosY = g_fPosY - ROGO_SPEED;
	}
	if (!flg) {
		g_fPosY = g_fPosY + ROGO_SPEED;
	}
	if (g_fPosY <= UNDER_MOVE_RIMIT_Y) flg = false;
	if (g_fPosY >= TOP_MOVE_RIMIT_Y)flg = true;


}

//=============================================================================
// ステージセレクト描画処理
//=============================================================================
void DrawSSelect(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetPolygonTexture(nullptr);

	SetPolygonSize(SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT);
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_SELECT_MENU; ++nCntPauseMenu) {
		SetPolygonPos(SELECT_MENU_POS_X + nCntPauseMenu * SELECT_MENU_INTERVAL, SELECT_MENU_POS_Y);

		if (nCntPauseMenu == g_nSelectSMenu) {
			SetPolygonColor(1.0f, 1.0f, 1.0f);
			SetPolygonPos(SELECT_MENU_POS_X + nCntPauseMenu * SELECT_MENU_INTERVAL, g_fPosY);
		}
		else {
			SetPolygonColor(0.3f, 0.3f, 0.3f);
			SetPolygonPos(SELECT_MENU_POS_X + nCntPauseMenu * SELECT_MENU_INTERVAL, SELECT_MENU_POS_Y);
		}
		// テクスチャの設定
		SetPolygonTexture(g_pTextures[nCntPauseMenu]);
		// ポリゴンの描画
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// ステージセレクトメニューの設定
//=============================================================================
void SetStageSelect(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ステージセレクトメニューの取得
//=============================================================================
STAGE_SELECT GetStageSelect(void)
{
	return g_nSelectSMenu;
}

//=============================================================================
// ステージセレクトメニューのリセット
//=============================================================================
void ResetStageSelect(void)
{
	g_nSelectSMenu = STAGE_1;
	//CSound::Play(SE_SHIZUKU);
	SetStageSelect();
}
