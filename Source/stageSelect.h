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
	STAGE_1 = 0,		// �X�e�[�W1
	STAGE_2,			// �X�e�[�W2
	STAGE_3,			// �X�e�[�W3
	STAGE_MAX
} STAGE_SELECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSSelect(void);
void UninitSSelect(void);
void UpdateSSelect(void);
void DrawSSelect(void);

void SetStageSelect(void);
STAGE_SELECT GetStageSelect(void);
void ResetStageSelect(void);
