/*=====ヘッダコメント=====
*@プログラム名	：scene.cpp
*@説明・概要	：画面変異について
*@製作者	：AT_12D_192_51吉田叶聖
*@更新者	：AT_12D_192_51吉田叶聖
*@更新内容	：ゲームオーバー追加
*@更新履歴	：2021/12/11
*/
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "scene.h"
#include "sceneTitle.h"
#include "scenegame.h"
#include "fade.h"
#include "gameover.h"
#include "sceneRule.h"
#include "sceneSelect.h"
#include "clear.h"
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static EScene g_eScene = SCENE_NONE;
//=============================================================================
// シーンの初期化処理
//=============================================================================
HRESULT InitScene() {
	HRESULT hr = S_OK;
	SetScene(SCENE_TITLE); // 最初はタイトル画面へ
	InitFade(); // フェード初期化
	return hr;
}
//=============================================================================
// シーンの終了処理
//=============================================================================
void UninitScene() {
	SetScene(SCENE_NONE); // 現在のシーンを終了
	UninitFade(); // フェード終了処理
}
//=============================================================================
// シーンの更新処理
//=============================================================================
void UpdateScene() {
	switch (g_eScene) {
	case SCENE_TITLE: // タイトル画面
		UpdateTitle();
		break;
	case SCENE_RULE: // チュートリアル画面
		UpdateRule();
		break;
	case SCENE_SELECT: // セレクト画面
		UpdateSelect();
		break;
	case SCENE_GAME: // ゲーム1画面
		UpdateGame();
		break;
	case SCENE_CLEAR: // リザルト画面
		UpdateClear();
		break;
	case SCENE_GAMEOVER:
		UpdateGameOver();
		break;
	}
	UpdateFade(); // フェードイン/アウト更新
}
//=============================================================================
// シーンの描画処理
//=============================================================================
void DrawScene() {
	switch (g_eScene) {
	case SCENE_TITLE: // タイトル画面
		DrawTitle();
		break;
	case SCENE_RULE: // チュートリアル画面
		DrawRule();
		break;
	case SCENE_SELECT: // セレクト画面
		DrawSelect();
		break;
	case SCENE_GAME: // ゲーム1画面
		DrawGame();
		break;
	case SCENE_CLEAR: // リザルト画面
		DrawClear();
		break;
	case SCENE_GAMEOVER:
		DrawGameOver();
		break;
	}
	DrawFade(); // フェードイン/アウト描画
}
//=============================================================================
// シーンの切替
//=============================================================================
void SetScene(EScene eScene) {
	// 現在のシーンを終了
	switch (g_eScene) {
	case SCENE_TITLE: // タイトル画面
		UninitTitle();
		break;
	case SCENE_RULE: // チュートリアル画面
		UninitRule();
		break;
	case SCENE_SELECT: // セレクト画面
		UninitSelect();
		break;
	case SCENE_GAME: // ゲーム1画面
		UninitGame();
		break;
	case SCENE_CLEAR: // リザルト画面
		UninitClear();
		break;
	case SCENE_GAMEOVER:
		UninitGameOver();
		break;
	}
	// シーン切替
	g_eScene = eScene;
	// 次のシーンを初期化
	switch (g_eScene) {
	case SCENE_TITLE: // タイトル画面
		InitTitle();
		break;
	case SCENE_RULE: // チュートリアル画面
		InitRule();
		break;
	case SCENE_SELECT: // セレクト画面
		InitSelect();
		break;
	case SCENE_GAME: // ゲーム1画面
		InitGame();
		break;
	case SCENE_CLEAR: // リザルト画面
		InitClear();
		break;
	case SCENE_GAMEOVER:
		InitGameOver();
		break;
	}
}