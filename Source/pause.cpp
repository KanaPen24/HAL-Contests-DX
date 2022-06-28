/*=====�w�b�_�R�����g=====
*@�v���O������	�Fpause.cpp
*@�����E�T�v	�F�|�[�Y��ʂȂǂɂ���
*@�����	�FAT_12D_192_51�g�c����
*@�X�V��	�FAT_12D_192_51�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/12/20
*/
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_PAUSE_MENU		(3)			// �|�[�Y���j���[��
#define	PAUSE_MENU_WIDTH	(320.0f)	// �|�[�Y���j���[��
#define	PAUSE_MENU_HEIGHT	(60.0f)		// �|�[�Y���j���[����
#define	PAUSE_MENU_POS_X	(0.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	PAUSE_MENU_POS_Y	(PAUSE_MENU_INTERVAL)	// �|�[�Y���j���[�ʒu(Y���W)
#define	PAUSE_MENU_INTERVAL	(100.0f)	// �|�[�Y���j���[�Ԋu
#define	PLATE_WIDTH			(360.0f)	// �v���[�g�̕�
#define	PLATE_HEIGHT		(340.0f)	// �v���[�g�̕�
#define	PLATE_POS_X			(0.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(0.0f)		// �v���[�g�̈ʒu(Y���W)
#define GAMEPAD_LEFT_STICK_DEADZONE  (7800)	//���X�e�B�b�N�̃f�b�h�]�[��
#define MAX_TEXTURE	(3)


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[6] = {nullptr};	// �e�N�X�`���ւ̃|�C���^

static PAUSE_MENU g_nSelectPMenu = PAUSE_MENU_CONTINUE;		// �I�𒆂̃��j���[No.
static float g_fCurve;
static float g_fCol = 0.0f;
static int g_nTexture;
static bool g_bJoyStick;

static LPCWSTR c_aFileNamePauseMenu[MAX_TEXTURE] =
{
	L"data/texture/retry.png",	// ���g���C
	L"data/texture/select.png",	// �Z���N�g
	L"data/texture/rule.png",	// ���[��
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPause(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntPauseMenu = 0; nCntPauseMenu < MAX_TEXTURE; ++nCntPauseMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNamePauseMenu[nCntPauseMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntPauseMenu]);			// �ǂݍ��ރ������[
	}

	g_nSelectPMenu = PAUSE_MENU_CONTINUE;
	g_fCurve = 0.0f;
	g_nTexture = 0;
	g_bJoyStick = false;

	// ���ʉ�������
	//g_pSE_SHIZUKU = CreateSound(SE_SHIZUKU_PATH);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPause(void)
{
	// �e�N�X�`���̊J��
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePause(void)
{
	DWORD JoyCount = GetJoyCount();
	LONG JoyY = GetJoyY(JOYSTICKID1);
	CSound::SetVolume(SE_PI, 0.2f);
	//�f�b�h�]�[���ȉ��Ȃ炷�ׂĂO�ɂ���
	if ((JoyY <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyY > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyY = 0;
	}
	if (JoyCount >= 1) {
		//���X�e�B�b�N����ɉ����ꂽ
		if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false)
		{
			CSound::Play(SE_PI);
			g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
			SetPauseMenu();
			g_bJoyStick = true;
		}
		//���X�e�B�b�N�����ɉ����ꂽ
		if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false)
		{
			CSound::Play(SE_PI);
			g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + 1) % NUM_PAUSE_MENU);
			SetPauseMenu();
			g_bJoyStick = true;
		}
		if (JoyY == 0l)
		{
			g_bJoyStick = false;
		}
	}
	if (GetKeyRepeat(VK_UP)) {
		CSound::Play(SE_PI);
		g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}
	else if ( GetKeyRepeat(VK_DOWN)) {
		CSound::Play(SE_PI);
		g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}

	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	// ���ˌ��̐ݒ�
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawPause(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetPolygonTexture(nullptr);
	SetPolygonSize(PLATE_WIDTH, PLATE_HEIGHT);
	SetPolygonPos(PLATE_POS_X, PLATE_POS_Y);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(g_fCol, g_fCol, g_fCol);
	SetPolygonAlpha(1.0f);
	DrawPolygon(pDeviceContext);

	SetPolygonSize(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		SetPolygonPos(PAUSE_MENU_POS_X, PAUSE_MENU_POS_Y - nCntPauseMenu * PAUSE_MENU_INTERVAL);

		if (nCntPauseMenu == g_nSelectPMenu) {
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else {
			//�I�΂�ĂȂ��ق�
			SetPolygonColor(0.0f, 0.0f, 0.0f);
		}

		// �e�N�X�`���̐ݒ�
		SetPolygonTexture(g_pTextures[nCntPauseMenu]);
		// �|���S���̕`��
		DrawPolygon(pDeviceContext);
	}

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetPauseMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
PAUSE_MENU GetPauseMenu(void)
{
	return g_nSelectPMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetPauseMenu(void)
{
	g_nSelectPMenu = PAUSE_MENU_CONTINUE;
	SetPauseMenu();
}
