/*=====�w�b�_�R�����g=====
*@�v���O������	�Fnumber.h
*@�����E�T�v	�F�����̓ǂݍ��݂�\���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#pragma once
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "main.h"
// �}�N����`
#define NUMBER_SIZE_X 35.0f // ������
#define NUMBER_SIZE_Y 50.0f // ��������
// �֐��v���g�^�C�v
HRESULT InitNumber(); // ������
void UninitNumber(); // �I������
void DrawNumber( // �`��
	XMFLOAT2 vPos,		// �\���ʒu(����)
	unsigned uNumber,	// �\�����l
	int nWidth,			// �\������
	float fSizeX = NUMBER_SIZE_X,	// 1�����̕�
	float fSizeY = NUMBER_SIZE_Y);	// 1�����̍���