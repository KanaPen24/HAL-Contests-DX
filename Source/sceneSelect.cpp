/*=====�w�b�_�R�����g=====
*@�v���O������	�FsceneSelect.cpp
*@�����E�T�v	�F�^�C�g���V�[���ɂ���
*@�����	�FAT_12D_192_51�g�c����
*@�X�V��	�FAT_12D_192_51�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/11/28
*/

//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "sceneSelect.h"
#include "texture.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "debugproc.h"
#include "stageSelect.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POS_X_BG		(0.0f)			//�w�i��X���W
#define POS_Y_BG		(0.0f)			//�w�i��Y���W
#define WIDTH_BG		(SCREEN_WIDTH)	//����
#define HEIGHT_BG		(SCREEN_HEIGHT)	//����
#define TEX_BG			(0)				//�w�i�̉摜��
#define TEX_ENTER		(1)				//�G���^�[�摜��
#define MAX_TEXTURE		(1)				//�g���摜��
#define BLINK_TIMER		(30)			//�_�Ŏ��ԂŎg�p
#define BLINK_START_TIMER	(5)			//�_�ŊJ�n����

//*****************************************************************************
// �O���[�o���錾
//*****************************************************************************
static LPCWSTR g_pszTexture[] = {
	L"data/texture/rule.jpg",
};
static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];
static int g_nBlink;
static int g_nStart;

static bool bSound1;	//��񂾂�����
static bool bSound2;	//��񂾂�����
static bool bSound3;	//��񂾂�����

//=============================================================================
// �Z���N�g�V�[���̏���������
//=============================================================================
HRESULT InitSelect() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// �e�N�X�`���Ǎ�
	for (int i = 0; i < MAX_TEXTURE; ++i) {
		hr = CreateTextureFromFile(pDevice, g_pszTexture[i], &g_pTexture[i]);
		if (FAILED(hr)) {
			return hr;
		}
	}
	// �ϐ�������
	g_nBlink = 0;
	g_nStart = 0;
	//��񂾂��Đ�������
	bSound1 = false;
	bSound2 = false;
	bSound3 = false;
	// BGM�Đ�
	CSound::SetVolume(BGM_TITLE, 0.1f);
	CSound::Play(BGM_TITLE);
	//�ꎞ��~������
	hr = InitSSelect();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�X�e�[�W�Z���N�g���������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}
	return hr;
}
//=============================================================================
// �Z���N�g�V�[���̏I������
//=============================================================================
void UninitSelect() {
	// ����~
	CSound::Stop(BGM_TITLE);
	CSound::Stop(SE_SELECT);
	// �e�N�X�`�����
	for (int i = 0; i < MAX_TEXTURE; ++i) {
		SAFE_RELEASE(g_pTexture[i]);
	}
	//�X�e�[�W�Z���N�g�I��
	UninitSSelect();
}
//=============================================================================
// �Z���N�g�V�[���̍X�V����
//=============================================================================
void UpdateSelect() {
	//�X�e�[�W�Z���N�g�X�V
	UpdateSSelect();
	
	switch (GetStageSelect())
	{
	case STAGE_1:
		bSound2 = true;
		bSound3 = true;
		//CSound::Resume();
		if (bSound1 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound1 = false;
		}
		// �J�n
		if (GetKeyRelease(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//�G���^�[����
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//���ʉ�
			StartFadeOut(SCENE_GAME);	//�Q�[��1��ʂ�
			g_nStart = 1; // �J�n�t���O
			g_nBlink = BLINK_START_TIMER;
			return;
		}
		break;
	case STAGE_2:
		
		bSound1 = true;
		bSound3 = true;
		if (bSound2 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound2 = false;
		}
		if (GetKeyRelease(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//�G���^�[����
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//���ʉ�
			StartFadeOut(SCENE_GAME);	//�Q�[����ʂ�
			g_nStart = 1; // �J�n�t���O
			g_nBlink = BLINK_START_TIMER;
			return;
		}		
		break;
	case STAGE_3:
		bSound1 = true;
		bSound2 = true;
		if (bSound3 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound3 = false;
		}
		if (GetKeyRelease(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//�G���^�[����
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//���ʉ�
			StartFadeOut(SCENE_GAME);	//�Q�[����ʂ�
			g_nStart = 1; // �J�n�t���O
			g_nBlink = BLINK_START_TIMER;
			return;
		}		
		break;
	}
	//����������
	if (GetKeyPress(VK_X)||GetJoyTrigger(JOYSTICKID1, JOYBUTTON4)) //Y��������
	{
		StartFadeOut(SCENE_RULE);	//���[����ʂ�
		g_nStart = 1; // �J�n�t���O
		g_nBlink = BLINK_START_TIMER;
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
// �Z���N�g�V�[���̕`�揈��
//=============================================================================
void DrawSelect() {

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetZBuffer(false);
	SetPolygonSize(WIDTH_BG, HEIGHT_BG);
	SetPolygonPos(POS_X_BG, POS_Y_BG);
	SetPolygonTexture(g_pTexture[TEX_BG]);
	DrawPolygon(pDC);
	DrawPolygon(pDC);
	

	//�X�e�[�W�Z���N�g�`��
	DrawSSelect();
	SetZBuffer(true);

}