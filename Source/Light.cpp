/*=====ヘッダコメント=====
*@プログラム名	：Light.cpp
*@説明・概要	：光源やライトについて
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*///*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Light.h"

using namespace DirectX;

// グローバル変数
namespace {
	const XMFLOAT3 LIGHT0_DIRECTION = XMFLOAT3(0.0f, -1.0f, 0.0f);//方向
	const XMFLOAT4 LIGHT0_DIFFUSE = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);//拡散
	const XMFLOAT4 LIGHT0_AMBIENT = XMFLOAT4(0.02f, 0.02f, 0.02f, 1.0f);//周囲
	const XMFLOAT4 LIGHT0_SPECULAR = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);//鏡面反射

	CLight g_light;
};

// コンストラクタ
CLight::CLight()
{
	Init();
}

// 初期化
void CLight::Init()
{
	XMFLOAT3 vDir = LIGHT0_DIRECTION;
	XMStoreFloat3(&m_direction, XMVector3Normalize(XMLoadFloat3(&vDir)));
	m_diffuse = LIGHT0_DIFFUSE;
	m_ambient = LIGHT0_AMBIENT;
	m_specular = LIGHT0_SPECULAR;
	m_bEnable = true;
}

// 光源方向取得
XMFLOAT3& CLight::GetDir()
{
	if (m_bEnable) return m_direction;
	static XMFLOAT3 off(0.0f, 0.0f, 0.0f);
	return off;
}

// 光源インスタンス取得
CLight* CLight::Get()
{
	return &g_light;
}
