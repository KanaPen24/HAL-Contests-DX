/*=====�w�b�_�R�����g=====
*@�v���O������	�FsceneRule.cpp
*@�����E�T�v	�F���[���V�[���ɂ���
*@�����	�FAT_12D_192_51�g�c����
*@�X�V��	�FAT_12D_192_51�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/11/28
*/

//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "sceneRule.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "fade.h"
#include "Sound.h"
#include "scene.h"
#include "debugproc.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Rule_TEXTURENAME		L"data/texture/setumei.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C����

#define Rule_POS_X			(0)			// �|���S���̕\���ʒu
#define Rule_POS_Y			(0)			// �|���S���̕\���ʒu
#define Rule_SIZE_X			(1280)		// �|���S���̃T�C�Y
#define Rule_SIZE_Y			(720)		// �|���S���̃T�C�Y

#define	START_POS_X			(0)			// �X�^�[�g�{�^���̈ʒu(X���W)
#define	START_POS_Y			(-220)		// �X�^�[�g�{�^���̈ʒu(Y���W)
#define	START_WIDTH			(480)		// �X�^�[�g�{�^���̕�
#define	START_HEIGHT		(120)		// �X�^�[�g�{�^���̍���

#define	COUNT_APPEAR		(60)		// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP		(60)		// �X�^�[�g�{�^���_�ł̎���


#define BLINK_TIMER		(30)			//�_�Ŏ��ԂŎg�p
#define BLINK_START_TIMER	(5)			//�_�ŊJ�n����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[1];			// �e�N�X�`���ւ̃|�C���^
static int							g_nCountAppear = 0;		//
static int							g_nCountDisp = 0;		//
static bool							g_bDispStart = false;	//
static bool							g_bSkip = false;		// �X�^�[�g�{�^������
static int g_nBlink;
static int g_nStart;

//=============================================================================
// ���[����ʂ̏���������
//=============================================================================
HRESULT InitRule(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// �e�N�X�`���̓ǂݍ���
	LPCWSTR pszFName[] = {
		Rule_TEXTURENAME,
	};
	for (int i = 0; i < _countof(g_pTextures); ++i) {
		hr = CreateTextureFromFile(pDevice, pszFName[i], &g_pTextures[i]);
		if (FAILED(hr)) {
			MessageBoxW(GetMainWnd(), pszFName[i], L"�e�N�X�`���Ǎ��G���[", MB_OK);
			return hr;
		}
	}

	// �ϐ�������
	g_nCountAppear = COUNT_APPEAR;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_bSkip = false;

	// BGM�Đ��J�n
	//CSound::Play(BGM_TITLE);

	return hr;
}

//=============================================================================
// ���[����ʂ̏I������
//=============================================================================
void UninitRule(void)
{
	// BGM�Đ���~
	//CSound::Stop(BGM_TITLE);

	// �e�N�X�`�����
	for (int i = 0; i < _countof(g_pTextures); ++i) {
		SAFE_RELEASE(g_pTextures[i]);
	}
	CSound::Stop(SE_SELECT);
}

//=============================================================================
// ���[����ʂ̍X�V����
//=============================================================================
void UpdateRule(void)
{
	--g_nCountAppear;
	if (g_nCountAppear <= 0) {
		g_nCountDisp = (g_nCountDisp + 1) % 80;
		g_bDispStart = (g_nCountDisp <= INTERVAL_DISP);
	}

	if (g_bSkip) {
		g_bDispStart = g_nCountAppear & 2;
	}

	// [Enter]�܂��̓X�y�[�X�L�[�ŊJ�n
	//if (!g_bSkip && (GetKeyRelease(VK_RETURN) || GetKeyRelease(VK_SPACE) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1))) {
	//	// �I����
	//	//CSound::Play(SE_SELECT);
	//	// �t�F�[�h�A�E�g��V�[���J��
	//	StartFadeOut(SCENE_TITLE);	//�^�C�g���Ɉړ�

	//	g_bSkip = true;
	//}
	// �^�C�g����ʂ�
	if (GetKeyRelease(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//�G���^�[����
		CSound::SetVolume(SE_SELECT, 0.3f);
		CSound::Play(SE_SELECT);	//���ʉ�
		StartFadeOut(SCENE_SELECT);//�Z���N�g�Ɉړ�
		g_nStart = 1; // �J�n�t���O
		return;
	}
#ifdef _DEBUG
	PrintDebugProc("ػ���:1\n");
	PrintDebugProc("ٰ�  :2\n");
	PrintDebugProc("�ڸ� :3\n");
	PrintDebugProc("���� :4\n");
	PrintDebugProc("�ް� :5\n");
	PrintDebugProc("�ްѵ��ް :6\n");
#endif // _DEBUG
}

//=============================================================================
// �`���[�g���A����ʕ`��
//=============================================================================
void DrawRule(void)
{
	SetZBuffer(false);
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetPolygonTexture(g_pTextures[0]);
	SetPolygonSize(Rule_SIZE_X, Rule_SIZE_Y);
	SetPolygonPos(Rule_POS_X, Rule_POS_Y);
	DrawPolygon(pDeviceContext);

	SetZBuffer(true);
}
