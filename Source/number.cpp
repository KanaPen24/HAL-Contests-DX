/*=====ヘッダコメント=====
*@プログラム名	：number.cpp
*@説明・概要	：数字の読み込みや表示について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "number.h"
#include "Texture.h"
#include "polygon.h"
// マクロ定義
#define PATH_TEXTURE_NUMBER L"data/texture/number004.png"
#define COUNT_X_NUMBER 10
#define COUNT_Y_NUMBER 1
// グローバル変数
static ID3D11ShaderResourceView* g_pTexture;
// 初期化
HRESULT InitNumber() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_NUMBER, &g_pTexture);
	return hr;
}
// 終了処理
void UninitNumber() {
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}
// 描画
void DrawNumber(XMFLOAT2 vPos, // 表示位置(左上)
	unsigned uNumber, // 表示数値
	int nWidth, // 表示桁数
	float fSizeX, // 1文字の幅
	float fSizeY
) // 1文字の高さ
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(fSizeX, fSizeY);
	SetPolygonTexture(g_pTexture);
	SetPolygonFrameSize(1.0f / COUNT_X_NUMBER,1.0f / COUNT_Y_NUMBER);
	vPos.y -= fSizeY * 0.5f;
	vPos.x += fSizeX * (nWidth - 0.5f);
	for (; nWidth > 0; --nWidth, vPos.x -= fSizeX) {
		unsigned n = uNumber % 10;
		uNumber /= 10;
		SetPolygonPos(vPos.x, vPos.y);
		SetPolygonUV((n % COUNT_X_NUMBER) / (float)COUNT_X_NUMBER, (n / COUNT_X_NUMBER) / (float)COUNT_Y_NUMBER);
		DrawPolygon(pDC);
	}
	// 元に戻す
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
}