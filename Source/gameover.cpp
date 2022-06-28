/*=====ヘッダコメント=====
*@プログラム名	：gameover.cpp
*@説明・概要	：ゲームオーバー画面について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "gameover.h"
#include "Texture.h"
#include "polygon.h"
#include "input.h"
#include "scene.h"
#include "Sound.h"
#include "fade.h"
#include "Time.h"

// マクロ定義
#define PATH_BGTEXTURE L"data/texture/ゲームオーバーデッドエンド赤.png"

#define BG_POS_X 0.0f
#define BG_POS_Y 0.0f
#define BG_WIDTH SCREEN_WIDTH
#define BG_HEIGHT SCREEN_HEIGHT
#define GAMEOVER_TIMER 250
// グローバル変数
static ID3D11ShaderResourceView* g_pTexture;
// 初期化
HRESULT InitGameOver() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
	if
		(FAILED(hr)) {
		return hr;
	}
	// BGM再生
	//CSound::Play(SE_GAMEOVER);
	return hr;
}
// 終了処理
void UninitGameOver() {
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);

}
// 更新
void UpdateGameOver() {
	// [Enter]またはスペースキー押下
		// [Enter]またはスペースキー押下
	if (GetKeyRelease(VK_RETURN) || GetKeyRelease(VK_SPACE)||GetJoyButton(JOYSTICKID1,JOYBUTTON1)) {
		// タイトル画面へ
		StartFadeOut(SCENE_SELECT);
		return;
	}
}
// 描画
void DrawGameOver() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);
}