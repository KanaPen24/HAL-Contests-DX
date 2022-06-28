/*=====ヘッダコメント=====
*@プログラム名	：model.cpp
*@説明・概要	：プレイヤーのモデル及び挙動について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*///*****************************************************************************
// インクルード部
//*****************************************************************************
#include "model.h"
#include "main.h"
#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "explosion.h"
#include "effect.h"
#include "collision.h"
#include "object.h"
#include <time.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ANIM_IDLE			(0)
#define ANIM_WALK			(1)

#define	VALUE_MOVE_MODEL	(0.55f)		// 移動速度
#define GRAVITY_MOVE		(0.1f)		//落下速度
#define TURN_MOVE_MODEL		(0.50f)		// 旋回速度
#define	RATE_MOVE_MODEL		(0.25f)		// 移動慣性係数
#define	VALUE_ROTATE_MODEL	(9.0f)		// 回転速度
#define	RATE_ROTATE_MODEL	(0.20f)		// 回転慣性係数
#define	PLAYER_RADIUS		(10.0f)		// 境界球半径

#define GAMEPAD_LEFT_STICK_DEADZONE  (7800)	//左スティックのデッドゾーン

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel	g_model;		// モデル

static XMFLOAT3		g_posModel;		// 現在の位置
static XMFLOAT3		g_rotModel;		// 現在の向き
static XMFLOAT3		g_rotDestModel;	// 目的の向き
static XMFLOAT3		g_moveModel;	// 移動量
static XMFLOAT3		g_sclModel;		// スケール
static XMFLOAT3		g_posEffect;
static XMFLOAT2		g_sizeEffect;
static LPCSTR		g_pszModelPath = "data/model/Player.fbx";

static XMFLOAT4X4	g_mtxWorld;		// ワールドマトリックス

static int			g_nShadow;		// 丸影番号

static bool			g_bAngleFlg[4];	// 角度変更用(0:左 1:右 2:上 3:下)
static bool			g_bSAngleFlg[4];// 角度変更用(0:左 1:右 2:上 3:下)スティック用
static int g_nAnimStack;
static double g_dAnimTime;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitModel(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// 位置・回転・スケールの初期設定
	g_posModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_sclModel = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_posEffect = g_posModel;
	g_sizeEffect = XMFLOAT2(10.0f, 10.0f);
	for (int i = 0; i < 4; i++) {
		g_bAngleFlg[i] = false;
		g_bSAngleFlg[i] = false;
	}
	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, g_pszModelPath)) {
		hr = E_FAIL;
		MessageBoxA(GetMainWnd(), "モデルデータ読込エラー", "InitModel", MB_OK | MB_ICONEXCLAMATION);
	}


	// 丸影の生成
	g_nShadow = CreateShadow(g_posModel, 12.0f);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
	// 丸影の解放
	ReleaseShadow(g_nShadow);

	// モデルの解放
	g_model.Release();

	
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{
	DWORD JoyCount = GetJoyCount();
	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();
	int g_rot = 1;
	g_posEffect = g_posModel;
	*GetJoyState(JOYSTICKID1);
	LONG JoyX = GetJoyX(JOYSTICKID1);
	LONG JoyY = GetJoyY(JOYSTICKID1);
	//コントローラー操作
	if ((JoyX <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyX > -GAMEPAD_LEFT_STICK_DEADZONE) &&
		(JoyY<  GAMEPAD_LEFT_STICK_DEADZONE &&
			JoyY > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyX = 0;
		JoyY = 0;
	}

	/*ここからコントローラー操作の入力*/
	if (JoyCount >= 1) {

		if (JoyX <= -GAMEPAD_LEFT_STICK_DEADZONE) {
			if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE) {
				// 左前移動
				g_moveModel.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_MODEL;
				g_moveModel.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_MODEL;

				g_rotDestModel.y = rotCamera.y + 135.0f;
			}
			else if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE) {
				// 左後移動
				g_moveModel.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_MODEL;
				g_moveModel.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_MODEL;

				g_rotDestModel.y = rotCamera.y + 45.0f;
			}
			else {
				// 左移動
				g_moveModel.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_MODEL;
				g_moveModel.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_MODEL;

				g_rotDestModel.y = rotCamera.y + 90.0f;
			}
		}
		else if (JoyX >= GAMEPAD_LEFT_STICK_DEADZONE) {
			if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE) {
				// 右前移動
				g_moveModel.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_MODEL;
				g_moveModel.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_MODEL;

				g_rotDestModel.y = rotCamera.y - 135.0f;
			}
			else if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE) {
				// 右後移動
				g_moveModel.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_MODEL;
				g_moveModel.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_MODEL;

				g_rotDestModel.y = rotCamera.y - 45.0f;
			}
			else {
				// 右移動
				g_moveModel.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_MODEL;
				g_moveModel.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_MODEL;

				g_rotDestModel.y = rotCamera.y - 90.0f;
			}

		}
		else if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE) {
			// 前移動
			g_moveModel.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = 180.0f + rotCamera.y;
		}
		else if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE) {
			// 後移動
			g_moveModel.x -= SinDeg(rotCamera.y) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y;
		}
	}
	//キーボード入力
	if (GetKeyPress(VK_LEFT)) {
		if (GetKeyPress(VK_UP)) {
			// 左前移動
			g_moveModel.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y + 135.0f;
		} else if (GetKeyPress(VK_DOWN)) {
			// 左後移動
			g_moveModel.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y + 45.0f;
		} else {
			// 左移動
			g_moveModel.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y + 90.0f;
		}
	} else if (GetKeyPress(VK_RIGHT)) {
		if (GetKeyPress(VK_UP)) {
			// 右前移動
			g_moveModel.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y - 135.0f;
		} else if (GetKeyPress(VK_DOWN)) {
			// 右後移動
			g_moveModel.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y - 45.0f;
		} else {
			// 右移動
			g_moveModel.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y - 90.0f;
		}

	} else if (GetKeyPress(VK_UP)) {
		// 前移動
		g_moveModel.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_MODEL;
		g_moveModel.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_MODEL;

		g_rotDestModel.y = 180.0f + rotCamera.y;
	} else if (GetKeyPress(VK_DOWN)) {
		// 後移動
		g_moveModel.x -= SinDeg(rotCamera.y) * VALUE_MOVE_MODEL;
		g_moveModel.z -= CosDeg(rotCamera.y) * VALUE_MOVE_MODEL;

		g_rotDestModel.y = rotCamera.y;
	}

	if (GetKeyPress(VK_LSHIFT)) {
		// 左回転
		g_rotDestModel.y -= VALUE_ROTATE_MODEL;
		if (g_rotDestModel.y < -180.0f) {
			g_rotDestModel.y += 360.0f;
		}
	}
	if (GetKeyPress(VK_RSHIFT)) {
		// 右回転
		g_rotDestModel.y += VALUE_ROTATE_MODEL;
		if (g_rotDestModel.y >= 180.0f) {
			g_rotDestModel.y -= 360.0f;
		}
	}

	// 目的の角度までの差分
	float fDiffRotY = g_rotDestModel.y - g_rotModel.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// 目的の角度まで慣性をかける
	g_rotModel.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (g_rotModel.y >= 180.0f) {
		g_rotModel.y -= 360.0f;
	}
	if (g_rotModel.y < -180.0f) {
		g_rotModel.y += 360.0f;
	}

	// 位置移動
	g_posModel.x += g_moveModel.x;
	g_posModel.y += g_moveModel.y;
	g_posModel.z += g_moveModel.z;

	// 移動量に慣性をかける
	g_moveModel.x += (0.0f - g_moveModel.x) * RATE_MOVE_MODEL;
	g_moveModel.y += (0.0f - g_moveModel.y) * RATE_MOVE_MODEL;
	g_moveModel.z += (0.0f - g_moveModel.z) * RATE_MOVE_MODEL;

	// アニメーション更新
	float fSpeed = sqrtf(g_moveModel.x * g_moveModel.x + g_moveModel.z * g_moveModel.z);
	PrintDebugProc("Speed=%f\n\n", fSpeed);
	PrintDebugProc("Animation=%d\n\n", g_nAnimStack);
	int nStack;
	if (fSpeed < 0.1f)
		nStack = ANIM_IDLE;
	if (fSpeed > 0.1f)
		nStack = ANIM_WALK;
	g_nAnimStack = nStack;
	static double dLastTime = clock() / double(CLOCKS_PER_SEC);
	double dNowTime = clock() / double(CLOCKS_PER_SEC);
	double dSlice = dNowTime - dLastTime;
	dLastTime = dNowTime;
	g_dAnimTime += dSlice;
	if (g_dAnimTime >= g_model.GetAnimDuration(g_nAnimStack)) {
		g_dAnimTime = 0.0;
	}


	//移動量限界値
	if (g_posModel.x < -620.0f) {
		g_posModel.x = -620.0f;
	}
	if (g_posModel.x > 620.0f) {
		g_posModel.x = 620.0f;
	}
	if (g_posModel.z < -620.0f) {
		g_posModel.z = -620.0f;
	}
	if (g_posModel.z > 620.0f) {
		g_posModel.z = 620.0f;
	}
	if (g_posModel.y < 0.0f) {
		g_posModel.y = 0.0f;
	}
	if (g_posModel.y > 0.0f) {
		g_posModel.y = 0.0f;
	}

	if (GetKeyPress(VK_RETURN)) {
		// リセット
		g_posModel = XMFLOAT3(0.0f, 20.0f, 0.0f);
		g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}




	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScl;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotModel.x),
		XMConvertToRadians(g_rotModel.y), XMConvertToRadians(g_rotModel.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	mtxScl = XMMatrixScaling(g_sclModel.x, g_sclModel.y, g_sclModel.z);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_posModel.x, g_posModel.y, g_posModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// 丸影の移動
	MoveShadow(g_nShadow, g_posModel);



#ifdef _DEBUG

	PrintDebugProc("[ﾋｺｳｷ ｲﾁ : (%f : %f : %f)]\n", g_posModel.x, g_posModel.y, g_posModel.z);
	PrintDebugProc("[ﾋｺｳｷ ﾑｷ : (%f) <ﾓｸﾃｷ ｲﾁ:(%f)>]\n", g_rotModel.y, g_rotDestModel.y);
	PrintDebugProc("\n%d",JoyCount);
	PrintDebugProc("\n%d", JoyX);
	PrintDebugProc("\n%d", JoyY);

	PrintDebugProc("\n");

	PrintDebugProc("*** ﾋｺｳｷ ｿｳｻ ***\n");
	PrintDebugProc("ﾏｴ   ｲﾄﾞｳ : \x1e\n");//↑
	PrintDebugProc("ｳｼﾛ  ｲﾄﾞｳ : \x1f\n");//↓
	PrintDebugProc("ﾋﾀﾞﾘ ｲﾄﾞｳ : \x1d\n");//←
	PrintDebugProc("ﾐｷﾞ  ｲﾄﾞｳ : \x1c\n");//→
	PrintDebugProc("ｼﾞｮｳｼｮｳ   : I\n");
	PrintDebugProc("ｶｺｳ       : K\n");
	PrintDebugProc("ﾋﾀﾞﾘ ｾﾝｶｲ : LSHIFT\n");
	PrintDebugProc("ﾐｷﾞ  ｾﾝｶｲ : RSHIFT\n");
	PrintDebugProc("ﾀﾏ   ﾊｯｼｬ : SPACE\n");
	PrintDebugProc("\n");
#endif // DEBUG
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	g_model.Draw(pDC, g_mtxWorld, eOpacityOnly);

	// 半透明部分を描画
	//SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない
	// モデル表示
	g_model.SetAnimIndex(g_nAnimStack);
	g_model.SetAnimTime(g_dAnimTime);
	g_model.Draw(pDC, g_mtxWorld);
	SetZWrite(true);				// Zバッファ更新する
	//SetBlendState(BS_NONE);			// アルファブレンド無効
}

//=============================================================================
// 位置取得
//=============================================================================
XMFLOAT3& GetModelPos()
{
	return g_posModel;
}

//=============================================================================
// 衝突判定
//=============================================================================
bool CollisionPlayer(XMFLOAT3 pos, XMFLOAT3 scale)
{
	bool hit = CollisionAABB(g_posModel,g_sclModel , pos, scale);
	if (hit) {
		PrintDebugProc("ｱﾀｯﾀ\n");
	}
	return hit;
}
