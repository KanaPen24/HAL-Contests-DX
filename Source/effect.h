/*=====�w�b�_�R�����g=====
*@�v���O������	�Feffect.h
*@�����E�T�v	�F�G�t�F�N�g�ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/#pragma once
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "main.h"

// �֐��v���g�^�C�v
HRESULT InitEffect();	// ������
void UninitEffect();	// �I������
void UpdateEffect();	// �X�V
void DrawEffect();		// �`��

int StartEffect(XMFLOAT3 pos,XMFLOAT2 size);
