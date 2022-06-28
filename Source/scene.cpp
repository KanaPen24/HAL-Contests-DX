/*=====�w�b�_�R�����g=====
*@�v���O������	�Fscene.cpp
*@�����E�T�v	�F��ʕψقɂ���
*@�����	�FAT_12D_192_51�g�c����
*@�X�V��	�FAT_12D_192_51�g�c����
*@�X�V���e	�F�Q�[���I�[�o�[�ǉ�
*@�X�V����	�F2021/12/11
*/
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "scene.h"
#include "sceneTitle.h"
#include "scenegame.h"
#include "fade.h"
#include "gameover.h"
#include "sceneRule.h"
#include "sceneSelect.h"
#include "clear.h"
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static EScene g_eScene = SCENE_NONE;
//=============================================================================
// �V�[���̏���������
//=============================================================================
HRESULT InitScene() {
	HRESULT hr = S_OK;
	SetScene(SCENE_TITLE); // �ŏ��̓^�C�g����ʂ�
	InitFade(); // �t�F�[�h������
	return hr;
}
//=============================================================================
// �V�[���̏I������
//=============================================================================
void UninitScene() {
	SetScene(SCENE_NONE); // ���݂̃V�[�����I��
	UninitFade(); // �t�F�[�h�I������
}
//=============================================================================
// �V�[���̍X�V����
//=============================================================================
void UpdateScene() {
	switch (g_eScene) {
	case SCENE_TITLE: // �^�C�g�����
		UpdateTitle();
		break;
	case SCENE_RULE: // �`���[�g���A�����
		UpdateRule();
		break;
	case SCENE_SELECT: // �Z���N�g���
		UpdateSelect();
		break;
	case SCENE_GAME: // �Q�[��1���
		UpdateGame();
		break;
	case SCENE_CLEAR: // ���U���g���
		UpdateClear();
		break;
	case SCENE_GAMEOVER:
		UpdateGameOver();
		break;
	}
	UpdateFade(); // �t�F�[�h�C��/�A�E�g�X�V
}
//=============================================================================
// �V�[���̕`�揈��
//=============================================================================
void DrawScene() {
	switch (g_eScene) {
	case SCENE_TITLE: // �^�C�g�����
		DrawTitle();
		break;
	case SCENE_RULE: // �`���[�g���A�����
		DrawRule();
		break;
	case SCENE_SELECT: // �Z���N�g���
		DrawSelect();
		break;
	case SCENE_GAME: // �Q�[��1���
		DrawGame();
		break;
	case SCENE_CLEAR: // ���U���g���
		DrawClear();
		break;
	case SCENE_GAMEOVER:
		DrawGameOver();
		break;
	}
	DrawFade(); // �t�F�[�h�C��/�A�E�g�`��
}
//=============================================================================
// �V�[���̐ؑ�
//=============================================================================
void SetScene(EScene eScene) {
	// ���݂̃V�[�����I��
	switch (g_eScene) {
	case SCENE_TITLE: // �^�C�g�����
		UninitTitle();
		break;
	case SCENE_RULE: // �`���[�g���A�����
		UninitRule();
		break;
	case SCENE_SELECT: // �Z���N�g���
		UninitSelect();
		break;
	case SCENE_GAME: // �Q�[��1���
		UninitGame();
		break;
	case SCENE_CLEAR: // ���U���g���
		UninitClear();
		break;
	case SCENE_GAMEOVER:
		UninitGameOver();
		break;
	}
	// �V�[���ؑ�
	g_eScene = eScene;
	// ���̃V�[����������
	switch (g_eScene) {
	case SCENE_TITLE: // �^�C�g�����
		InitTitle();
		break;
	case SCENE_RULE: // �`���[�g���A�����
		InitRule();
		break;
	case SCENE_SELECT: // �Z���N�g���
		InitSelect();
		break;
	case SCENE_GAME: // �Q�[��1���
		InitGame();
		break;
	case SCENE_CLEAR: // ���U���g���
		InitClear();
		break;
	case SCENE_GAMEOVER:
		InitGameOver();
		break;
	}
}