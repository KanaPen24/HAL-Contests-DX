/*=====�w�b�_�R�����g=====
*@�v���O������	�FsceneTitle.cpp
*@�����E�T�v	�F�^�C�g���V�[���ɂ���
*@�����	�FAT_12D_192_51�g�c����
*@�X�V��	�FAT_12D_192_51�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/11/28
*/

//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "sceneTitle.h"
#include "texture.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "debugproc.h"
#include "sound.h"
#include "titleSelect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POS_X_BG		(0.0f)			//�w�i��X���W
#define POS_Y_BG		(0.0f)			//�w�i��Y���W
#define WIDTH_BG		(SCREEN_WIDTH)	//����
#define HEIGHT_BG		(SCREEN_HEIGHT)	//����
#define WIDTH_ROGO		(1000)			//���S����
#define HEIGHT_ROGO		(200)			//���S����
#define POS_X_ENTER		(0.0f)			//�G���^�[X���W
#define POS_Y_ENTER		(120.0f)	//�G���^�[Y���W
#define TEX_BG			(0)				//�w�i�̉摜��
#define TEX_ENTER		(1)				//�G���^�[�摜��
#define MAX_TEXTURE		(1)				//�g���摜��

//*****************************************************************************
// �O���[�o���錾
//*****************************************************************************
static LPCWSTR g_pszTexture[] = {
	L"data/texture/title.png",
};
static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];
static int g_nBlink;
static int g_nStart;
static bool bSound1;	//��񂾂�����
static bool bSound2;	//��񂾂�����
static bool bSound3;	//��񂾂�����

//=============================================================================
// �^�C�g���V�[���̏���������
//=============================================================================
HRESULT InitTitle() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// �e�N�X�`���Ǎ�
	for(int i = 0; i < MAX_TEXTURE; ++i) {
		hr = CreateTextureFromFile(pDevice, g_pszTexture[i], &g_pTexture[i]);
		if(FAILED(hr)) {
			return hr;
		}
	}
	// �ϐ�������
	g_nBlink = 0;
	g_nStart = 0;
	// BGM�Đ�
	CSound::SetVolume(BGM_TITLE, 0.05f);
	CSound::Play(BGM_TITLE);
	//�^�C�g���Z���N�g������
	hr = InitTSelect();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�^�C�g���Z���N�g���������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}
	return hr;
}
//=============================================================================
// �^�C�g���V�[���̏I������
//=============================================================================
void UninitTitle() {
	// ����~
	CSound::Stop(BGM_TITLE);
	CSound::Stop(SE_SELECT);
	
	// �e�N�X�`�����
	for(int i = 0; i < MAX_TEXTURE; ++i) {
		SAFE_RELEASE(g_pTexture[i]);
	}
	//�^�C�g���Z���N�g�I��
	UninitTSelect();
}
//=============================================================================
// �^�C�g���V�[���̍X�V����
//=============================================================================
void UpdateTitle() {
	//�^�C�g���Z���N�g�X�V
	UpdateTSelect();
	CSound::SetVolume(SE_SELECT, 0.1f);
	switch (GetTitleSelect())
	{
	case GAME:
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
			CSound::Play(SE_SELECT);	//���ʉ�
			StartFadeOut(SCENE_SELECT);	//�Q�[��1��ʂ�
			g_nStart = 1; // �J�n�t���O			return;
		}
		break;
	case END:

		bSound1 = true;
		bSound3 = true;
		if (bSound2 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound2 = false;
		}
		if (GetKeyRelease(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//�G���^�[����
			CSound::Play(SE_SELECT);	//���ʉ�
			PostQuitMessage(0);				// �V�X�e���ɃX���b�h�̏I����v��
		}
		break;
	}

	//// �J�n
	//if (GetKeyRelease(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//�G���^�[����
	//	CSound::Play(SE_SELECT);	//���ʉ�
	//	StartFadeOut(SCENE_SELECT);	//�Q�[����ʂ�
	//	g_nStart = 1; // �J�n�t���O
	//	return;
	//}
	//if (GetKeyRelease(VK_SPACE) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON2)) {//�X�y�[�X���͂�
	//	StartFadeOut(SCENE_RULE);	//�`���[�g���A����ʂ�
	//	g_nStart = 1; // �J�n�t���O
	//	return;
	//}

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
// �^�C�g���V�[���̕`�揈��
//=============================================================================
void DrawTitle() {

	SetZBuffer(false);
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(WIDTH_BG, HEIGHT_BG);
	SetPolygonPos(POS_X_BG, POS_Y_BG);
	SetPolygonTexture(g_pTexture[TEX_BG]);
	DrawPolygon(pDC);
	
	//�^�C�g���Z���N�g�`��
	DrawTSelect();

	SetZBuffer(true);
}