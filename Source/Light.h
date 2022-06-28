/*=====ヘッダコメント=====
*@プログラム名	：light.h
*@説明・概要	：光源や照明について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "main.h"

class CLight
{
private:
	XMFLOAT4 m_diffuse;
	XMFLOAT4 m_ambient;
	XMFLOAT4 m_specular;
	XMFLOAT3 m_direction;
	bool m_bEnable;			// ライティング有効/無効

public:
	CLight();

	void Init();
	XMFLOAT4& GetDiffuse() { return m_diffuse; }
	XMFLOAT4& GetAmbient() { return m_ambient; }
	XMFLOAT4& GetSpecular() { return m_specular; }
	XMFLOAT3& GetDir();
	void SetEnable(bool bEnable = true) { m_bEnable = bEnable; }
	void SetDisable(bool bDisable = true) { m_bEnable = !bDisable; }

	static CLight* Get();
};
