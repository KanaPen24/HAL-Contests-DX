/*=====�w�b�_�R�����g=====
*@�v���O������	�Fgameover.cpp
*@�����E�T�v	�F�Q�[���I�[�o�[��ʂɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "gameover.h"
#include "Texture.h"
#include "polygon.h"
#include "input.h"
#include "scene.h"
#include "Sound.h"
#include "fade.h"
#include "Time.h"

// �}�N����`
#define PATH_BGTEXTURE L"data/texture/�Q�[���I�[�o�[�f�b�h�G���h��.png"

#define BG_POS_X 0.0f
#define BG_POS_Y 0.0f
#define BG_WIDTH SCREEN_WIDTH
#define BG_HEIGHT SCREEN_HEIGHT
#define GAMEOVER_TIMER 250
// �O���[�o���ϐ�
static ID3D11ShaderResourceView* g_pTexture;
// ������
HRESULT InitGameOver() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
	if
		(FAILED(hr)) {
		return hr;
	}
	// BGM�Đ�
	//CSound::Play(SE_GAMEOVER);
	return hr;
}
// �I������
void UninitGameOver() {
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);

}
// �X�V
void UpdateGameOver() {
	// [Enter]�܂��̓X�y�[�X�L�[����
		// [Enter]�܂��̓X�y�[�X�L�[����
	if (GetKeyRelease(VK_RETURN) || GetKeyRelease(VK_SPACE)||GetJoyButton(JOYSTICKID1,JOYBUTTON1)) {
		// �^�C�g����ʂ�
		StartFadeOut(SCENE_SELECT);
		return;
	}
}
// �`��
void DrawGameOver() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);
}