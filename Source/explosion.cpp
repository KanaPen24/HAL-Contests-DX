/*=====ヘッダコメント=====
*@プログラム名	：
*@説明・概要	：
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "explosion.h"
#include "debugproc.h"
#include "Camera.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_EXPLOSION			L"data/texture/explosion000.png"
#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER			(1.0f)
#define M_AMBIENT		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_EXPLOSION		100		// 弾最大数

#define EXPLOSION_FRAME_X		8
#define EXPLOSION_FRANE_Y		1
#define EXPLOSION_ANIM_COUNT	4


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TExplosion {
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
static TExplosion	g_explosion[MAX_EXPLOSION];	// 爆発情報

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
static HRESULT MakeVertexExplosion(ID3D11Device* pDevise);

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitExplosion(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	//頂点情報の作成
	hr = MakeVertexExplosion(pDevice);
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
	hr = CreateTextureFromFile(pDevice, MODEL_EXPLOSION, &g_mesh.pTexture);
	if (FAILED(hr)) return hr;
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	//ワールドマトリックス初期化
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	//爆発情報初期化
	for (int i = 0; i < MAX_EXPLOSION; ++i) {
		g_explosion[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_explosion[i].m_size = XMFLOAT2(10.0f, 10.0f);
		g_explosion[i].use = false;
		g_explosion[i].m_nanim = 0;
		g_explosion[i].m_ncount = EXPLOSION_ANIM_COUNT;
	}

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitExplosion(void)
{
	// モデルの解放
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateExplosion(void)
{
	TExplosion* pExplosion = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExplosion) {
		//未使用ならスキップ
		if (!pExplosion->use) {
			continue;
		}
		//カウンタ更新
		--pExplosion->m_ncount;
		if (pExplosion->m_ncount <= 0) {
			//アニメーションの更新
			++pExplosion->m_nanim;
			if (pExplosion->m_nanim >= EXPLOSION_FRAME_X * EXPLOSION_FRANE_Y) {
				pExplosion->use = false;	//消滅
				continue;
			}
			//カウンタの初期化
			pExplosion->m_ncount = EXPLOSION_ANIM_COUNT;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawExplosion(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	//光源無効
	SetBlendState(BS_ALPHABLEND);	//aブレンディング有効
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	TExplosion* pExplosion = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; i++, pExplosion++) {
		//未使用ならスキップ
		if (!pExplosion->use) {
			continue;
		}
		//テクスチャマトリックスの更新
		XMMATRIX mtxTex = XMMatrixScaling(1.0f / EXPLOSION_FRAME_X, 1.0f / EXPLOSION_FRANE_Y, 1.0f);
		int u = pExplosion->m_nanim % EXPLOSION_FRAME_X;
		int v = pExplosion->m_nanim / EXPLOSION_FRAME_X;
		mtxTex = XMMatrixMultiply(mtxTex, XMMatrixTranslation((float)u / EXPLOSION_FRAME_X, (float)v / EXPLOSION_FRANE_Y, 0.0f));
		XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTex);
		//ビュー行列の回転成分の転置行列を設定
		g_mesh.mtxWorld._11 = mView._11 * pExplosion->m_size.x;
		g_mesh.mtxWorld._12 = mView._21 * pExplosion->m_size.x;
		g_mesh.mtxWorld._13 = mView._31 * pExplosion->m_size.x;
		g_mesh.mtxWorld._14 = 0.0f;
		g_mesh.mtxWorld._21 = mView._12 * pExplosion->m_size.y;
		g_mesh.mtxWorld._22 = mView._22 * pExplosion->m_size.y;
		g_mesh.mtxWorld._23 = mView._32 * pExplosion->m_size.y;
		g_mesh.mtxWorld._24 = 0.0f;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;
		g_mesh.mtxWorld._34 = 0.0f;
		//位置を反映
		g_mesh.mtxWorld._41 = pExplosion->m_pos.x;
		g_mesh.mtxWorld._42 = pExplosion->m_pos.y;
		g_mesh.mtxWorld._43 = pExplosion->m_pos.z;
		g_mesh.mtxWorld._44 = 1.0f;
		//描画
		DrawMesh(pDC, &g_mesh);
	}
	SetBlendState(BS_NONE);		//aブレンディング無効
	CLight::Get()->SetEnable();	//光源有効
}

//=============================================================================
// 爆発開始
//=============================================================================
int StartExplosion(XMFLOAT3 pos, XMFLOAT2 size)
{
	TExplosion*pExplosion = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; i++, pExplosion++) {
		//使用中ならスキップ
		if (pExplosion->use) {
			continue;
		}
		pExplosion->m_pos = pos;
		pExplosion->m_size = size;
		pExplosion->m_nanim = 0;
		pExplosion->m_ncount = EXPLOSION_ANIM_COUNT;
		pExplosion->use = true;
		return i;	//爆発できた(0～:弾番号)
	}
	return -1;		//爆発できなかった
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexExplosion(ID3D11Device* pDevice)
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
