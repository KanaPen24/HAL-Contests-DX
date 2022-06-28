/*=====ヘッダコメント=====
*@プログラム名	：sceneTitle.cpp
*@説明・概要	：タイトルシーンについて
*@製作者	：AT_12D_192_51吉田叶聖
*@更新者	：AT_12D_192_51吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "sceneTitle.h"
#include "texture.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "debugproc.h"
#include "sound.h"
#include "titleSelect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POS_X_BG		(0.0f)			//背景のX座標
#define POS_Y_BG		(0.0f)			//背景のY座標
#define WIDTH_BG		(SCREEN_WIDTH)	//横幅
#define HEIGHT_BG		(SCREEN_HEIGHT)	//立幅
#define WIDTH_ROGO		(1000)			//ロゴ横幅
#define HEIGHT_ROGO		(200)			//ロゴ立幅
#define POS_X_ENTER		(0.0f)			//エンターX座標
#define POS_Y_ENTER		(120.0f)	//エンターY座標
#define TEX_BG			(0)				//背景の画像数
#define TEX_ENTER		(1)				//エンター画像数
#define MAX_TEXTURE		(1)				//使う画像数

//*****************************************************************************
// グローバル宣言
//*****************************************************************************
static LPCWSTR g_pszTexture[] = {
	L"data/texture/title.png",
};
static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];
static int g_nBlink;
static int g_nStart;
static bool bSound1;	//一回だけ流す
static bool bSound2;	//一回だけ流す
static bool bSound3;	//一回だけ流す

//=============================================================================
// タイトルシーンの初期化処理
//=============================================================================
HRESULT InitTitle() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// テクスチャ読込
	for(int i = 0; i < MAX_TEXTURE; ++i) {
		hr = CreateTextureFromFile(pDevice, g_pszTexture[i], &g_pTexture[i]);
		if(FAILED(hr)) {
			return hr;
		}
	}
	// 変数初期化
	g_nBlink = 0;
	g_nStart = 0;
	// BGM再生
	CSound::SetVolume(BGM_TITLE, 0.05f);
	CSound::Play(BGM_TITLE);
	//タイトルセレクト初期化
	hr = InitTSelect();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("タイトルセレクト初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}
	return hr;
}
//=============================================================================
// タイトルシーンの終了処理
//=============================================================================
void UninitTitle() {
	// 音停止
	CSound::Stop(BGM_TITLE);
	CSound::Stop(SE_SELECT);
	
	// テクスチャ解放
	for(int i = 0; i < MAX_TEXTURE; ++i) {
		SAFE_RELEASE(g_pTexture[i]);
	}
	//タイトルセレクト終了
	UninitTSelect();
}
//=============================================================================
// タイトルシーンの更新処理
//=============================================================================
void UpdateTitle() {
	//タイトルセレクト更新
	UpdateTSelect();
	CSound::SetVolume(SE_SELECT, 0.1f);
	switch (GetTitleSelect())
	{
	case GAME:
		bSound2 = true;
		bSound3 = true;
		//CSound::Resume();
		if (bSound1 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound1 = false;
		}
		// 開始
		if (GetKeyRelease(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//エンター入力
			CSound::Play(SE_SELECT);	//効果音
			StartFadeOut(SCENE_SELECT);	//ゲーム1画面へ
			g_nStart = 1; // 開始フラグ			return;
		}
		break;
	case END:

		bSound1 = true;
		bSound3 = true;
		if (bSound2 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound2 = false;
		}
		if (GetKeyRelease(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//エンター入力
			CSound::Play(SE_SELECT);	//効果音
			PostQuitMessage(0);				// システムにスレッドの終了を要求
		}
		break;
	}

	//// 開始
	//if (GetKeyRelease(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//エンター入力
	//	CSound::Play(SE_SELECT);	//効果音
	//	StartFadeOut(SCENE_SELECT);	//ゲーム画面へ
	//	g_nStart = 1; // 開始フラグ
	//	return;
	//}
	//if (GetKeyRelease(VK_SPACE) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON2)) {//スペース入力で
	//	StartFadeOut(SCENE_RULE);	//チュートリアル画面へ
	//	g_nStart = 1; // 開始フラグ
	//	return;
	//}

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
// タイトルシーンの描画処理
//=============================================================================
void DrawTitle() {

	SetZBuffer(false);
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(WIDTH_BG, HEIGHT_BG);
	SetPolygonPos(POS_X_BG, POS_Y_BG);
	SetPolygonTexture(g_pTexture[TEX_BG]);
	DrawPolygon(pDC);
	
	//タイトルセレクト描画
	DrawTSelect();

	SetZBuffer(true);
}