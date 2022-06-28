/*=====�w�b�_�R�����g=====
*@�v���O������	�Fpause.cpp
*@�����E�T�v	�F�|�[�Y��ʂȂǂɂ���
*@�����	�FAT_12D_192_51�g�c����
*@�X�V��	�FAT_12D_192_51�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/12/20
*/
#pragma once
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "main.h"

// �|�[�Y���j���[
typedef enum {
	PAUSE_MENU_CONTINUE = 0,	// �R���e�B�j���[
	PAUSE_MENU_SELECT,			// �X�e�[�W�Z���N�g
	PAUSE_MENU_RULE,			// ���[��
	PAUSE_MENU_MAX
} PAUSE_MENU;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

void SetPauseMenu(void);
PAUSE_MENU GetPauseMenu(void);
void ResetPauseMenu(void);
