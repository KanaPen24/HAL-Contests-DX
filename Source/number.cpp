/*=====�w�b�_�R�����g=====
*@�v���O������	�Fnumber.cpp
*@�����E�T�v	�F�����̓ǂݍ��݂�\���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "number.h"
#include "Texture.h"
#include "polygon.h"
// �}�N����`
#define PATH_TEXTURE_NUMBER L"data/texture/number004.png"
#define COUNT_X_NUMBER 10
#define COUNT_Y_NUMBER 1
// �O���[�o���ϐ�
static ID3D11ShaderResourceView* g_pTexture;
// ������
HRESULT InitNumber() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_NUMBER, &g_pTexture);
	return hr;
}
// �I������
void UninitNumber() {
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}
// �`��
void DrawNumber(XMFLOAT2 vPos, // �\���ʒu(����)
	unsigned uNumber, // �\�����l
	int nWidth, // �\������
	float fSizeX, // 1�����̕�
	float fSizeY
) // 1�����̍���
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
	// ���ɖ߂�
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
}