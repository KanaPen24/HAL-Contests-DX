/*=====�w�b�_�R�����g=====
*@�v���O������	�FstageSelect.h
*@�����E�T�v	�F�X�e�[�W�Z���N�g����
*@�����	�FAT_12D_192_51�g�c����
*/
#pragma once
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "main.h"

// �|�[�Y���j���[
typedef enum {
	GAME = 0,		// �Q�[���X�^�[�g
	END,			// �I��
	TITLE_MAX
} TITLE_SELECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTSelect(void);
void UninitTSelect(void);
void UpdateTSelect(void);
void DrawTSelect(void);

void SetTitleSelect(void);
TITLE_SELECT GetTitleSelect(void);
void ResetTitleSelect(void);
