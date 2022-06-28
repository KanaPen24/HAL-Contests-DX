/*=====ヘッダコメント=====
*@プログラム名	：Object.cpp
*@説明・概要	：オブジェクトについて
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "object.h"
#include "Camera.h"
#include "shadow.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"
#include "model.h"
#include "debugproc.h"
#include "score.h"
#include "effect.h"
#include "stageSelect.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_OBJECT	L"data/texture/book.gif"	// 読み込むテクスチャファイル名
#define	BOOK_WIDTH		(30.0f)						// 木の幅
#define	BOOK_HEIGHT		(50.0f)						// 木の高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexObject(ID3D11Device* pDevice);
void SetVertexObject(int idxObject, float width, float height);
void SetColorObject(int idxObject, XMFLOAT4 col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH				g_mesh;				// メッシュ情報
static MATERIAL			g_material;			// マテリアル
static TObject			g_object[30];		// オブジェクトワーク
static int g_cntObject;
static int g_MaxObject;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitObject(void)
{
	ID3D11Device* pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexObject(pDevice);

	// テクスチャの読み込み
	CreateTextureFromFile(pDevice,				// デバイスへのポインタ
						  TEXTURE_OBJECT,			// ファイルの名前
						  &g_mesh.pTexture);	// 読み込むメモリ
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	g_material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_material.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_material.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_material.Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_material.Power = 0.0f;
	g_mesh.pMaterial = &g_material;
	switch (GetStageSelect())
	{
	case STAGE_1:
		g_MaxObject = 10;
		break;
	case STAGE_2:
		g_MaxObject = 20;
		break;
	case STAGE_3:
		g_MaxObject = 30;
		break;
	}
	for (g_cntObject = 0; g_cntObject < g_MaxObject; ++g_cntObject) {
		g_object[g_cntObject].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_object[g_cntObject].col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		g_object[g_cntObject].rot = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
		g_object[g_cntObject].sclModel = XMFLOAT3(20.0f, 20.0f, 20.0f);
		g_object[g_cntObject].width = BOOK_WIDTH;
		g_object[g_cntObject].height = BOOK_HEIGHT;
		g_object[g_cntObject].use = false;
	}
	for (int nCntObject = 0; nCntObject < g_MaxObject; ++nCntObject) {
		float fPosX = (rand() % 8200) / 10.0f - 310.0f;
		float fPosY = 0.0f;
		float fPosZ = (rand() % 8200) / 10.0f - 310.0f;
		SetObject(XMFLOAT3(fPosX, fPosY, fPosZ), 30.0f, 40.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitObject(void)
{
	for (g_cntObject = 0; g_cntObject < g_MaxObject; ++g_cntObject) {
		if (g_object[g_cntObject].use) {
			ReleaseShadow(g_object[g_cntObject].idxShadow);
			g_object[g_cntObject].idxShadow = -1;
			g_object[g_cntObject].use = false;
		}
	}
	// メッシュの開放
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateObject(void)
{
	for (int cntObject = 0; cntObject < g_MaxObject; ++cntObject) {
		// 未使用ならスキップ
		if (!g_object[cntObject].use) {
			continue;
		}
		// 影の位置設定
		MoveShadow(g_object[cntObject].idxShadow, XMFLOAT3(g_object[cntObject].pos.x, 0.1f, g_object[cntObject].pos.z));
	}
	for (g_cntObject = 0; g_cntObject < g_MaxObject; ++g_cntObject) {
		//プレイヤーの当たり判定
		if (CollisionPlayer(g_object[g_cntObject].pos, g_object[g_cntObject].sclModel)) {
			//オブジェクト消滅
			StartEffect(g_object[g_cntObject].pos, XMFLOAT2(BOOK_WIDTH, BOOK_HEIGHT));
			g_object[g_cntObject].pos.z = 50000.0f;
			CSound::SetVolume(SE_CRASH, 0.2f);
			CSound::Play(SE_CRASH);	//効果音
			AddScore(-1);
			return;
		}
	}
	

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawObject(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate, mtxScl;

	CLight::Get()->SetDisable();	// 光源無効
	//SetBlendState(BS_ALPHABLEND);	// αブレンディング有効

	// ビューマトリックスを取得
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	for (g_cntObject = 0; g_cntObject < g_MaxObject; ++g_cntObject) {
		// 未使用ならスキップ
		if (!g_object[g_cntObject].use) {
			continue;
		}
		// ワールド マトリックス初期化
		mtxWorld = XMMatrixIdentity();
		XMStoreFloat4x4(&g_mesh.mtxWorld, mtxWorld);

		// 回転を反映
		g_mesh.mtxWorld._11 = mtxView._11;
		g_mesh.mtxWorld._12 = mtxView._21;
		g_mesh.mtxWorld._13 = mtxView._31;
		g_mesh.mtxWorld._21 = mtxView._12;
		g_mesh.mtxWorld._22 = mtxView._22;
		g_mesh.mtxWorld._23 = mtxView._32;
		g_mesh.mtxWorld._31 = mtxView._13;
		g_mesh.mtxWorld._32 = mtxView._23;
		g_mesh.mtxWorld._33 = mtxView._33;
		mtxWorld = XMLoadFloat4x4(&g_mesh.mtxWorld);

		mtxScl = XMMatrixScaling(g_object[g_cntObject].sclModel.x, g_object[g_cntObject].sclModel.y, g_object[g_cntObject].sclModel.z);

		// スケールを反映 (回転より先に反映)
		mtxScale = XMMatrixScaling(g_object[g_cntObject].width, g_object[g_cntObject].height, 1.0f);
		mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_object[g_cntObject].pos.x, g_object[g_cntObject].pos.y, g_object[g_cntObject].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		XMStoreFloat4x4(&g_mesh.mtxWorld, mtxWorld);

		// 色の設定
		g_material.Diffuse = g_object[g_cntObject].col;

		// ポリゴンの描画
		DrawMesh(pDeviceContext, &g_mesh);
	}

	//SetBlendState(BS_NONE);		// αブレンディング無効
	CLight::Get()->SetEnable();	// 光源有効
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexObject(ID3D11Device* pDevice)
{
	// 一時的な頂点配列を生成
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// 頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	// 頂点座標の設定
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2.0f, 0.0f, 0.0f);
	pVtx[1].vtx = XMFLOAT3(-1.0f / 2.0f, 1.0f, 0.0f);
	pVtx[2].vtx = XMFLOAT3( 1.0f / 2.0f, 0.0f, 0.0f);
	pVtx[3].vtx = XMFLOAT3( 1.0f / 2.0f, 1.0f, 0.0f);

	// 法線の設定
	pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 反射光の設定
	pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[g_mesh.nNumIndex];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexObject(int idxObject, float width, float height)
{
	if (idxObject >= 0 && idxObject < g_MaxObject) {
		g_object[idxObject].width = width;
		g_object[idxObject].height = height;
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorObject(int idxObject, XMFLOAT4 col)
{
	if (idxObject >= 0 && idxObject < g_MaxObject) {
		g_object[idxObject].col = col;
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetObject(XMFLOAT3 pos, float width, float height, XMFLOAT4 col)
{
	int idxObject = -1;

	for (int cntObject = 0; cntObject < g_MaxObject; ++cntObject) {
		// 使用中ならスキップ
		if (g_object[cntObject].use) {
			continue;
		}
		g_object[cntObject].use = true;
		g_object[cntObject].pos = pos;

		// 頂点座標の設定
		SetVertexObject(cntObject, width, height);

		// 頂点カラーの設定
		SetColorObject(cntObject, col);

		// 影の設定
		g_object[cntObject].idxShadow = CreateShadow(g_object[cntObject].pos, g_object[cntObject].width * 0.5f);

		idxObject = cntObject;
		break;
	}

	return idxObject;
}

int GetCnt()
{
	return g_cntObject;
}