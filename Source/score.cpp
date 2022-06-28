/*=====�w�b�_�R�����g=====
*@�v���O������	�Fscore.cpp
*@�����E�T�v	�F�X�R�A�\���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*///*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "score.h"
#include "number.h"
#include "Texture.h"
#include "polygon.h"
#include "Time.h"
#include "object.h"
#include "debugproc.h"
#include "fade.h"
#include "scene.h"
#include "clear.h"

// �}�N����`
#define CHRCNT_SCORE 2
#define POS_X_SCORE 0.0f
#define POS_Y_SCORE ((SCREEN_HEIGHT-80.0f)*0.5f+16.0f)
#define MAX_SCORE GetCnt()
// �O���[�o���ϐ�
static ID3D11ShaderResourceView* g_pTexture;
static int g_nScore;
// ������
HRESULT InitScore()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// �ϐ�������
	g_nScore = MAX_SCORE;
	return hr;
}
// �I������
void UninitScore()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}
// �X�V
void UpdateScore()
{
	if (g_nScore == 0)
	{
		StartCutOut(SCENE_CLEAR);
		Clearflg(true);
	}
}
// �`��
void DrawScore()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	// ���邢��
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	// �X�R�A�\��
	DrawNumber(XMFLOAT2(POS_X_SCORE, POS_Y_SCORE),(unsigned)g_nScore, CHRCNT_SCORE);
	// ���ɖ߂�
	SetPolygonColor(1.0f, 1.0f, 1.0f);
}
//// �X�R�A���Z
void AddScore(int nScore)
{
	g_nScore += nScore;
}
// �X�R�A�擾
int GetScore() {
	return g_nScore;
}