/*=====ヘッダコメント=====
*@プログラム名	：sceneGame.cpp
*@説明・概要	：ゲームシーンについて
*@製作者	：AT_12D_192_51吉田叶聖
*/

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "sceneGame.h"
#include "polygon.h"
#include "debugproc.h"
#include "fade.h"
#include "input.h"
#include "texture.h"
#include "shadow.h"
#include "model.h"
#include "bg.h"
#include "meshfield.h"
#include "meshwall.h"
#include "enemy.h"
#include "object.h"
#include "AssimpModel.h"
#include "pause.h"
#include "sound.h"
#include "effect.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PATH_BGTEXTURE	L"data/texture/setumei.jpg"
#define SOUSA_POS_X		0.0f
#define SOUSA_POS_Y		0.0f
#define SOUSA_WIDTH		700.0f
#define SOUSA_HEIGHT	500.0f
#define MAX_OBJECT			(64)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//*****************************************************************************
// グローバル宣言
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static int g_nBlink;
static int g_nStart;
static bool g_bPause;	//一時停止中
static bool g_bSoundPause;
static int g_nSound;
static float	g_fAlf;			//アルファ値



//=============================================================================
// ゲームシーンの初期化処理
//=============================================================================
HRESULT InitGame() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// テクスチャ読込
		hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
		if (FAILED(hr)) {
			return hr;
		}
	
	
	// 丸影初期化
	hr = InitShadow();
	if (FAILED(hr))
		return hr;
	// モデル表示初期化
	hr = InitModel();
	if (FAILED(hr))
		return hr;

	// 敵機初期化
	hr = InitEnemy();
	if (FAILED(hr))
		return hr;

	// フィールド初期化
	hr = InitMeshField(16, 16, 80.0f, 80.0f);
	if (FAILED(hr))
		return hr;

	// 壁初期化
	hr = InitMeshWall();
	if (FAILED(hr))
		return hr;
	SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	// 背景初期化
	hr = InitBG();
	if (FAILED(hr))
		return hr;
	// 木の初期化
	InitObject();

	//一時停止初期化
	hr = InitPause();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("ポーズ初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	//エフェクト初期化
	hr = InitEffect();
	if (FAILED(hr))
		return hr;

	//スコア初期化
	hr = InitScore();
	if (FAILED(hr))
		return hr;

	g_bPause = false;
	g_bSoundPause = true;
	// 変数初期化
	g_nBlink = 0;
	g_nStart = 0;
	g_nSound = 0;
	g_fAlf = 0.0f;

	// BGM再生
	CSound::SetVolume(BGM_GAME, 0.05f);
	CSound::Play(BGM_GAME);

	return hr;
}
//=============================================================================
// ゲームシーンの終了処理
//=============================================================================
void UninitGame() {
	// 音停止
	CSound::Stop(BGM_GAME);
	// テクスチャ解放
	// 操作説明解放
	SAFE_RELEASE(g_pTexture);
	//ポーズ終了
	UninitPause();

	// 木の終了処理
	UninitObject();

	// 背景終了処理
	UninitBG();

	// フィールド終了処理
	UninitMeshField();

	//壁終了処理
	UninitMeshWall();

	// 敵機終了処理
	UninitEnemy();

	// モデル表示終了処理
	UninitModel();

	// 丸影終了処理
	UninitShadow();

	//エフェクト終了処理
	UninitEffect();

	//すこあ終了処理
	UninitScore();

}
//=============================================================================
// ゲームシーンの更新処理
//=============================================================================
void UpdateGame() {
	//一時停止中？
	if (g_bPause) {
		//一時停止更新
		UpdatePause();

	}
	else {
		
		// モデル更新
		UpdateModel();

		// 敵機更新
		UpdateEnemy();

		// 背景更新
		UpdateBG();

		// フィールド更新
		UpdateMeshField();

		//壁更新
		UpdateMeshWall();

		// 丸影更新
		UpdateShadow();

		// カメラ更新
		CCamera::Get()->Update();

		// 木の更新
		UpdateObject();

		//エフェクト更新
		UpdateEffect();

		// スコアの更新
		UpdateScore();

	}
	if (GetKeyTrigger(VK_SPACE) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON2) && g_fAlf == 1.0f)//B押したら操作画面を消す
	{
		g_fAlf = 0.0f;
	}
	CSound::SetVolume(SE_SHIZUKU, 0.2f);
	CSound::SetVolume(SE_CANCEL, 0.2f);
	//一時停止ON/OFF
	if (GetKeyTrigger(VK_P) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON8)) {
		//フェードイン/アウト中でなければ
		if (GetFade() == FADE_NONE) {
			g_bPause = !g_bPause;
			if (g_bPause) {
				//一時停止開始
				CSound::Pause();
				CSound::Play(SE_SHIZUKU);
				if (g_bSoundPause == true)	//押したら一回だけ再生
				{
					g_bSoundPause = false;
				}
			}
			else
			{
				//一時停止解除
				g_fAlf = 0.0f;
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				if (g_bSoundPause == false)  //押したら一回だけ再生
				{
					g_bSoundPause = true;
				}

			}
		}
	}
	//一時停止メニューの選択
	if (g_bPause&&GetFade() == FADE_NONE) {
		//[Enter]が押された？
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {
			//選択中のメニュー項目により分岐
			switch (GetPauseMenu())
			{
			case PAUSE_MENU_CONTINUE:
				StartFadeOut(SCENE_GAME);
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				break;
			case PAUSE_MENU_SELECT:
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				StartFadeOut(SCENE_SELECT);
				break;
			case PAUSE_MENU_RULE:
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				g_fAlf = 1.0f;

				break;
			}
		}
	}
#ifdef _DEBUG
	PrintDebugProc("ﾘｻﾞﾙﾄ:1\n");
	PrintDebugProc("ﾙｰﾙ  :2\n");
	PrintDebugProc("ｾﾚｸﾄ :3\n");
	PrintDebugProc("ﾀｲﾄﾙ :4\n");
	PrintDebugProc("ｹﾞｰﾑ :5\n");
#endif // _DEBUG
}
//=============================================================================
// ゲームシーンの描画処理
//=============================================================================
void DrawGame() {
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	// 背景描画
	DrawBG();

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);

	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効

	//// フィールド描画
	DrawMeshField();

	////壁描画
	DrawMeshWall();

	// 敵機描画
	DrawEnemy();

	// モデル描画
	DrawModel();

	// 丸影描画
	DrawShadow();

	// 木の描画
	DrawObject();

	//エフェクト描画
	DrawEffect();

	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	// スコア描画
	DrawScore();

	//一時停止描画
	if (g_bPause) {
		DrawPause();
	}

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonAlpha(g_fAlf);
	SetPolygonSize(SOUSA_WIDTH, SOUSA_HEIGHT);
	SetPolygonPos(SOUSA_POS_X, SOUSA_POS_Y);
	SetPolygonTexture(g_pTexture);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);

}

