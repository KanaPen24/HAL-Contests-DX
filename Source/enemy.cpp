/*=====ヘッダコメント=====
*@プログラム名	：enemy.cpp
*@説明・概要	：敵の挙動について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/24
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "enemy.h"
#include "main.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "model.h"
#include "fade.h"
#include "sound.h"
#include "stageSelect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_ENEMY			"data/model/Enemy.fbx"

#define	RATE_MOVE_ENEMY		(0.20f)		// 移動慣性係数
#define	VALUE_ROTATE_ENEMY	(7.0f)		// 回転速度
#define	RATE_ROTATE_ENEMY	(0.20f)		// 回転慣性係数

#define MAX_ENEMY			(1)		// 敵機最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TEnemy {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3	m_rotDest;	// 目的の向き
	XMFLOAT3	m_move;		// 移動量
	XMFLOAT3	m_sclModel;			// スケール

	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス

	int			m_nShadow;	// 丸影番号
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel	g_model;			// モデル
static TEnemy		g_enemy[MAX_ENEMY];	// 敵機情報
static float g_Speed;	//移動速度
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY)) {
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
		return E_FAIL;
	}
	switch (GetStageSelect())
	{
	case STAGE_1:
		g_Speed = 1.0f;
		break;
	case STAGE_2:
		g_Speed = 1.2f;
		break;
	case STAGE_3:
		g_Speed = 1.5f;
		break;
	}

	for (int i = 0; i < MAX_ENEMY; ++i) {
		// 位置・回転・スケールの初期設定
		g_enemy[i].m_pos = XMFLOAT3(rand() % 620 - 310.0f,
									0.0f,
									rand() % 620 - 310.0f);
		g_enemy[i].m_rot = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
		g_enemy[i].m_rotDest = g_enemy[i].m_rot;
		g_enemy[i].m_move = XMFLOAT3(
			-SinDeg(g_enemy[i].m_rot.y) * g_Speed,
			0.0f,
			-CosDeg(g_enemy[i].m_rot.y) * g_Speed);
		g_enemy[i].m_sclModel = XMFLOAT3(1.00f, 1.00f, 1.00f);
		// 丸影の生成
		switch (GetStageSelect())
		{
		case STAGE_1:
			g_enemy[i].m_nShadow = CreateShadow(g_enemy[i].m_pos, 12.0f);
			break;
		case STAGE_2:
			g_enemy[i].m_nShadow = CreateShadow(g_enemy[i].m_pos, 12.0f);
			break;
		case STAGE_3:
			break;
		}
	}

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; ++i) {
		// 丸影の解放
		switch (GetStageSelect())
		{
		case STAGE_1:
			ReleaseShadow(g_enemy[i].m_nShadow);
			break;
		case STAGE_2:
			ReleaseShadow(g_enemy[i].m_nShadow);
			break;
		case STAGE_3:
			break;
		}

	}

	// モデルの解放
	g_model.Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	XMMATRIX mtxWorld, mtxTranslate, mtxScl;
	static XMFLOAT3 pPos;			// プレイヤー表示位置
	pPos = GetModelPos();		//プレイヤーの座標取得

	for (int i = 0; i < MAX_ENEMY; ++i) {
		// 移動
		// 目的の角度までの差分
		float fDiffRotY = g_enemy[i].m_rotDest.y - g_enemy[i].m_rot.y;
		// プレイヤーを追尾する
		if (pPos.x > g_enemy[i].m_pos.x) {
			g_enemy[i].m_pos.x += g_Speed;
		}
		else 
		{
			g_enemy[i].m_pos.x -= g_Speed;
		}
		if (pPos.z > g_enemy[i].m_pos.z)
		{
			g_enemy[i].m_pos.z += g_Speed;
		}
		else 
		{
			g_enemy[i].m_pos.z -= g_Speed;
		}

		// 目的の角度まで慣性をかける
		g_enemy[i].m_rot.y += fDiffRotY * RATE_ROTATE_ENEMY;
		if (g_enemy[i].m_rot.y >= 180.0f) {
			g_enemy[i].m_rot.y -= 360.0f;
		}
		if (g_enemy[i].m_rot.y < -180.0f) {
			g_enemy[i].m_rot.y += 360.0f;
		}
		g_enemy[i].m_move = XMFLOAT3(-SinDeg(g_enemy[i].m_rot.y) * g_Speed,
			0.0f,-CosDeg(g_enemy[i].m_rot.y) * g_Speed);
		//プレイヤーの当たり判定
		if (CollisionPlayer(g_enemy[i].m_pos, g_enemy[i].m_sclModel)) {
			//ゲームオーバー処理
			StartCutOut(SCENE_GAMEOVER);
			CSound::SetVolume(SE_DAMAGE, 0.2f);
			CSound::Play(SE_DAMAGE);
		}

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_enemy[i].m_sclModel.x, g_enemy[i].m_sclModel.y, g_enemy[i].m_sclModel.z);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(
			g_enemy[i].m_pos.x,
			g_enemy[i].m_pos.y,
			g_enemy[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_enemy[i].m_mtxWorld, mtxWorld);

		// 丸影の移動
		switch (GetStageSelect())
		{
		case STAGE_1:
			MoveShadow(g_enemy[i].m_nShadow, g_enemy[i].m_pos);
			break;
		case STAGE_2:
			MoveShadow(g_enemy[i].m_nShadow, g_enemy[i].m_pos);
			break;
		case STAGE_3:
			break;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
}
