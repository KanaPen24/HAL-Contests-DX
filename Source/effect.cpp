/*=====ヘッダコメント=====
*@プログラム名	：effect.cpp
*@説明・概要	：エフェクトについて
*@製作者	：吉田叶聖
*@更新内容	：2022/06/24
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "effect.h"
#include "debugproc.h"
#include "Camera.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_EFFECT			L"data/texture/effect.png"
#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER			(1.0f)
#define M_AMBIENT		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_EFFECT		100		// 弾最大数

#define Effect_FRAME_X		8
#define Effect_FRANE_Y		1
#define Effect_ANIM_COUNT	8


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TEffect {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT2	m_size;		// サイズ
	bool		use;		//使用しているかどうか
	int			m_nanim;	//アニメーション再生位置
	int			m_ncount;	//アニメーションフレーム数
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH g_mesh;			//メッシュ情報
static MATERIAL g_material;	//マテリアル
static TEffect	g_Effect[MAX_EFFECT];	// 爆発情報

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
static HRESULT MakeVertexEffect(ID3D11Device* pDevise);

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	//頂点情報の作成
	hr = MakeVertexEffect(pDevice);
	if (FAILED(hr)) {
		return hr;
	}
	//マテリアルの設定
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;
	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice, MODEL_EFFECT, &g_mesh.pTexture);
	if (FAILED(hr)) return hr;
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	//ワールドマトリックス初期化
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	//爆発情報初期化
	for (int i = 0; i < MAX_EFFECT; ++i) {
		g_Effect[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Effect[i].m_size = XMFLOAT2(10.0f, 10.0f);
		g_Effect[i].use = false;
		g_Effect[i].m_nanim = 0;
		g_Effect[i].m_ncount = Effect_ANIM_COUNT;
	}

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	// モデルの解放
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	TEffect* pEffect = g_Effect;
	for (int i = 0; i < MAX_EFFECT; ++i, ++pEffect) {
		//未使用ならスキップ
		if (!pEffect->use) {
			continue;
		}
		//カウンタ更新
		--pEffect->m_ncount;
		if (pEffect->m_ncount <= 0) {
			//アニメーションの更新
			++pEffect->m_nanim;
			if (pEffect->m_nanim >= Effect_FRAME_X * Effect_FRANE_Y) {
				pEffect->use = false;	//消滅
				continue;
			}
			//カウンタの初期化
			pEffect->m_ncount = Effect_ANIM_COUNT;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	//光源無効
	//SetBlendState(BS_ALPHABLEND);	//aブレンディング有効
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	TEffect* pEffect = g_Effect;
	for (int i = 0; i < MAX_EFFECT; i++, pEffect++) {
		//未使用ならスキップ
		if (!pEffect->use) {
			continue;
		}
		//テクスチャマトリックスの更新
		XMMATRIX mtxTex = XMMatrixScaling(1.0f / Effect_FRAME_X, 1.0f / Effect_FRANE_Y, 1.0f);
		int u = pEffect->m_nanim % Effect_FRAME_X;
		int v = pEffect->m_nanim / Effect_FRAME_X;
		mtxTex = XMMatrixMultiply(mtxTex, XMMatrixTranslation((float)u / Effect_FRAME_X, (float)v / Effect_FRANE_Y, 0.0f));
		XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTex);
		//ビュー行列の回転成分の転置行列を設定
		g_mesh.mtxWorld._11 = mView._11 * pEffect->m_size.x;
		g_mesh.mtxWorld._12 = mView._21 * pEffect->m_size.x;
		g_mesh.mtxWorld._13 = mView._31 * pEffect->m_size.x;
		g_mesh.mtxWorld._14 = 0.0f;
		g_mesh.mtxWorld._21 = mView._12 * pEffect->m_size.y;
		g_mesh.mtxWorld._22 = mView._22 * pEffect->m_size.y;
		g_mesh.mtxWorld._23 = mView._32 * pEffect->m_size.y;
		g_mesh.mtxWorld._24 = 0.0f;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;
		g_mesh.mtxWorld._34 = 0.0f;
		//位置を反映
		g_mesh.mtxWorld._41 = pEffect->m_pos.x;
		g_mesh.mtxWorld._42 = pEffect->m_pos.y;
		g_mesh.mtxWorld._43 = pEffect->m_pos.z;
		g_mesh.mtxWorld._44 = 1.0f;
		//描画
		DrawMesh(pDC, &g_mesh);
	}
	//SetBlendState(BS_NONE);		//aブレンディング無効
	CLight::Get()->SetEnable();	//光源有効
}

//=============================================================================
// 爆発開始
//=============================================================================
int StartEffect(XMFLOAT3 pos, XMFLOAT2 size)
{
	pos.x = pos.x + 10.0f;
	TEffect*pEffect = g_Effect;
	for (int i = 0; i < MAX_EFFECT; i++, pEffect++) {
		//使用中ならスキップ
		if (pEffect->use) {
			continue;
		}
		pEffect->m_pos = pos;
		pEffect->m_size = size;
		pEffect->m_nanim = 0;
		pEffect->m_ncount = Effect_ANIM_COUNT;
		pEffect->use = true;
		return i;	//爆発できた(0～:弾番号)
	}
	return -1;		//爆発できなかった
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexEffect(ID3D11Device* pDevice)
{
	//一時的な頂点配列を生成
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];
	//頂点配列の中身を埋める
	pVertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);
	//一時的なインデックス配列を生成
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[4];

	//インデックスの配列を埋める
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	//頂点バッファ/インデックスバッファ生成
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	//一時的な頂点配列/インデックス配列を開放
	delete[] pIndexWk;
	delete[] pVertexWk;
	return hr;
}
