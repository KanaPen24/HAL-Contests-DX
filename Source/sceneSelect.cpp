/*=====ヘッダコメント=====
*@プログラム名	：sceneSelect.cpp
*@説明・概要	：タイトルシーンについて
*@製作者	：AT_12D_192_51吉田叶聖
*@更新者	：AT_12D_192_51吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "sceneSelect.h"
#include "texture.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "debugproc.h"
#include "stageSelect.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POS_X_BG		(0.0f)			//背景のX座標
#define POS_Y_BG		(0.0f)			//背景のY座標
#define WIDTH_BG		(SCREEN_WIDTH)	//横幅
#define HEIGHT_BG		(SCREEN_HEIGHT)	//立幅
#define TEX_BG			(0)				//背景の画像数
#define TEX_ENTER		(1)				//エンター画像数
#define MAX_TEXTURE		(1)				//使う画像数
#define BLINK_TIMER		(30)			//点滅時間で使用
#define BLINK_START_TIMER	(5)			//点滅開始時間

//*****************************************************************************
// グローバル宣言
//*****************************************************************************
static LPCWSTR g_pszTexture[] = {
	L"data/texture/rule.jpg",
};
static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];
static int g_nBlink;
static int g_nStart;

static bool bSound1;	//一回だけ流す
static bool bSound2;	//一回だけ流す
static bool bSound3;	//一回だけ流す

//=============================================================================
// セレクトシーンの初期化処理
//=============================================================================
HRESULT InitSelect() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// テクスチャ読込
	for (int i = 0; i < MAX_TEXTURE; ++i) {
		hr = CreateTextureFromFile(pDevice, g_pszTexture[i], &g_pTexture[i]);
		if (FAILED(hr)) {
			return hr;
		}
	}
	// 変数初期化
	g_nBlink = 0;
	g_nStart = 0;
	//一回だけ再生初期化
	bSound1 = false;
	bSound2 = false;
	bSound3 = false;
	// BGM再生
	CSound::SetVolume(BGM_TITLE, 0.1f);
	CSound::Play(BGM_TITLE);
	//一時停止初期化
	hr = InitSSelect();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("ステージセレクト初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}
	return hr;
}
//=============================================================================
// セレクトシーンの終了処理
//=============================================================================
void UninitSelect() {
	// 音停止
	CSound::Stop(BGM_TITLE);
	CSound::Stop(SE_SELECT);
	// テクスチャ解放
	for (int i = 0; i < MAX_TEXTURE; ++i) {
		SAFE_RELEASE(g_pTexture[i]);
	}
	//ステージセレクト終了
	UninitSSelect();
}
//=============================================================================
// セレクトシーンの更新処理
//=============================================================================
void UpdateSelect() {
	//ステージセレクト更新
	UpdateSSelect();
	
	switch (GetStageSelect())
	{
	case STAGE_1:
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
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//効果音
			StartFadeOut(SCENE_GAME);	//ゲーム1画面へ
			g_nStart = 1; // 開始フラグ
			g_nBlink = BLINK_START_TIMER;
			return;
		}
		break;
	case STAGE_2:
		
		bSound1 = true;
		bSound3 = true;
		if (bSound2 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound2 = false;
		}
		if (GetKeyRelease(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//エンター入力
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//効果音
			StartFadeOut(SCENE_GAME);	//ゲーム画面へ
			g_nStart = 1; // 開始フラグ
			g_nBlink = BLINK_START_TIMER;
			return;
		}		
		break;
	case STAGE_3:
		bSound1 = true;
		bSound2 = true;
		if (bSound3 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound3 = false;
		}
		if (GetKeyRelease(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//エンター入力
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//効果音
			StartFadeOut(SCENE_GAME);	//ゲーム画面へ
			g_nStart = 1; // 開始フラグ
			g_nBlink = BLINK_START_TIMER;
			return;
		}		
		break;
	}
	//操作説明画面
	if (GetKeyPress(VK_X)||GetJoyTrigger(JOYSTICKID1, JOYBUTTON4)) //Y押したら
	{
		StartFadeOut(SCENE_RULE);	//ルール画面へ
		g_nStart = 1; // 開始フラグ
		g_nBlink = BLINK_START_TIMER;
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
// セレクトシーンの描画処理
//=============================================================================
void DrawSelect() {

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetZBuffer(false);
	SetPolygonSize(WIDTH_BG, HEIGHT_BG);
	SetPolygonPos(POS_X_BG, POS_Y_BG);
	SetPolygonTexture(g_pTexture[TEX_BG]);
	DrawPolygon(pDC);
	DrawPolygon(pDC);
	

	//ステージセレクト描画
	DrawSSelect();
	SetZBuffer(true);

}