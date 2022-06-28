/*=====�w�b�_�R�����g=====
*@�v���O������	�FsceneGame.cpp
*@�����E�T�v	�F�Q�[���V�[���ɂ���
*@�����	�FAT_12D_192_51�g�c����
*/

//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "sceneGame.h"
#include "polygon.h"
#include "debugproc.h"
#include "fade.h"
#include "input.h"
#include "texture.h"
#include "shadow.h"
#include "model.h"
#include "bg.h"
#include "meshfield.h"
#include "meshwall.h"
#include "enemy.h"
#include "object.h"
#include "AssimpModel.h"
#include "pause.h"
#include "sound.h"
#include "effect.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PATH_BGTEXTURE	L"data/texture/setumei.jpg"
#define SOUSA_POS_X		0.0f
#define SOUSA_POS_Y		0.0f
#define SOUSA_WIDTH		700.0f
#define SOUSA_HEIGHT	500.0f
#define MAX_OBJECT			(64)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//*****************************************************************************
// �O���[�o���錾
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static int g_nBlink;
static int g_nStart;
static bool g_bPause;	//�ꎞ��~��
static bool g_bSoundPause;
static int g_nSound;
static float	g_fAlf;			//�A���t�@�l



//=============================================================================
// �Q�[���V�[���̏���������
//=============================================================================
HRESULT InitGame() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// �e�N�X�`���Ǎ�
		hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
		if (FAILED(hr)) {
			return hr;
		}
	
	
	// �ۉe������
	hr = InitShadow();
	if (FAILED(hr))
		return hr;
	// ���f���\��������
	hr = InitModel();
	if (FAILED(hr))
		return hr;

	// �G�@������
	hr = InitEnemy();
	if (FAILED(hr))
		return hr;

	// �t�B�[���h������
	hr = InitMeshField(16, 16, 80.0f, 80.0f);
	if (FAILED(hr))
		return hr;

	// �Ǐ�����
	hr = InitMeshWall();
	if (FAILED(hr))
		return hr;
	SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	// �w�i������
	hr = InitBG();
	if (FAILED(hr))
		return hr;
	// �؂̏�����
	InitObject();

	//�ꎞ��~������
	hr = InitPause();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�|�[�Y���������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	//�G�t�F�N�g������
	hr = InitEffect();
	if (FAILED(hr))
		return hr;

	//�X�R�A������
	hr = InitScore();
	if (FAILED(hr))
		return hr;

	g_bPause = false;
	g_bSoundPause = true;
	// �ϐ�������
	g_nBlink = 0;
	g_nStart = 0;
	g_nSound = 0;
	g_fAlf = 0.0f;

	// BGM�Đ�
	CSound::SetVolume(BGM_GAME, 0.05f);
	CSound::Play(BGM_GAME);

	return hr;
}
//=============================================================================
// �Q�[���V�[���̏I������
//=============================================================================
void UninitGame() {
	// ����~
	CSound::Stop(BGM_GAME);
	// �e�N�X�`�����
	// ����������
	SAFE_RELEASE(g_pTexture);
	//�|�[�Y�I��
	UninitPause();

	// �؂̏I������
	UninitObject();

	// �w�i�I������
	UninitBG();

	// �t�B�[���h�I������
	UninitMeshField();

	//�ǏI������
	UninitMeshWall();

	// �G�@�I������
	UninitEnemy();

	// ���f���\���I������
	UninitModel();

	// �ۉe�I������
	UninitShadow();

	//�G�t�F�N�g�I������
	UninitEffect();

	//�������I������
	UninitScore();

}
//=============================================================================
// �Q�[���V�[���̍X�V����
//=============================================================================
void UpdateGame() {
	//�ꎞ��~���H
	if (g_bPause) {
		//�ꎞ��~�X�V
		UpdatePause();

	}
	else {
		
		// ���f���X�V
		UpdateModel();

		// �G�@�X�V
		UpdateEnemy();

		// �w�i�X�V
		UpdateBG();

		// �t�B�[���h�X�V
		UpdateMeshField();

		//�ǍX�V
		UpdateMeshWall();

		// �ۉe�X�V
		UpdateShadow();

		// �J�����X�V
		CCamera::Get()->Update();

		// �؂̍X�V
		UpdateObject();

		//�G�t�F�N�g�X�V
		UpdateEffect();

		// �X�R�A�̍X�V
		UpdateScore();

	}
	if (GetKeyTrigger(VK_SPACE) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON2) && g_fAlf == 1.0f)//B�������瑀���ʂ�����
	{
		g_fAlf = 0.0f;
	}
	CSound::SetVolume(SE_SHIZUKU, 0.2f);
	CSound::SetVolume(SE_CANCEL, 0.2f);
	//�ꎞ��~ON/OFF
	if (GetKeyTrigger(VK_P) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON8)) {
		//�t�F�[�h�C��/�A�E�g���łȂ����
		if (GetFade() == FADE_NONE) {
			g_bPause = !g_bPause;
			if (g_bPause) {
				//�ꎞ��~�J�n
				CSound::Pause();
				CSound::Play(SE_SHIZUKU);
				if (g_bSoundPause == true)	//���������񂾂��Đ�
				{
					g_bSoundPause = false;
				}
			}
			else
			{
				//�ꎞ��~����
				g_fAlf = 0.0f;
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				if (g_bSoundPause == false)  //���������񂾂��Đ�
				{
					g_bSoundPause = true;
				}

			}
		}
	}
	//�ꎞ��~���j���[�̑I��
	if (g_bPause&&GetFade() == FADE_NONE) {
		//[Enter]�������ꂽ�H
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {
			//�I�𒆂̃��j���[���ڂɂ�蕪��
			switch (GetPauseMenu())
			{
			case PAUSE_MENU_CONTINUE:
				StartFadeOut(SCENE_GAME);
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				break;
			case PAUSE_MENU_SELECT:
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				StartFadeOut(SCENE_SELECT);
				break;
			case PAUSE_MENU_RULE:
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				g_fAlf = 1.0f;

				break;
			}
		}
	}
#ifdef _DEBUG
	PrintDebugProc("ػ���:1\n");
	PrintDebugProc("ٰ�  :2\n");
	PrintDebugProc("�ڸ� :3\n");
	PrintDebugProc("���� :4\n");
	PrintDebugProc("�ް� :5\n");
#endif // _DEBUG
}
//=============================================================================
// �Q�[���V�[���̕`�揈��
//=============================================================================
void DrawGame() {
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	// �w�i�`��
	DrawBG();

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);

	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��

	//// �t�B�[���h�`��
	DrawMeshField();

	////�Ǖ`��
	DrawMeshWall();

	// �G�@�`��
	DrawEnemy();

	// ���f���`��
	DrawModel();

	// �ۉe�`��
	DrawShadow();

	// �؂̕`��
	DrawObject();

	//�G�t�F�N�g�`��
	DrawEffect();

	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	// �X�R�A�`��
	DrawScore();

	//�ꎞ��~�`��
	if (g_bPause) {
		DrawPause();
	}

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonAlpha(g_fAlf);
	SetPolygonSize(SOUSA_WIDTH, SOUSA_HEIGHT);
	SetPolygonPos(SOUSA_POS_X, SOUSA_POS_Y);
	SetPolygonTexture(g_pTexture);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);

}

