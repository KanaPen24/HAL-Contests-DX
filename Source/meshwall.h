/*=====�w�b�_�R�����g=====
*@�v���O������	�Fmeshwall.h
*@�����E�T�v	�F���b�V���𗘗p�����ǂɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/#pragma once
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "main.h"

//**************************************
// �`�敔��
//**************************************
enum EDrawPart {
	DRAWPART_ALL = 0,		// ��ʂȂ��\��
	DRAWPART_OPAQUE,		// �s�����̂ݕ`��
	DRAWPART_TRANSLUCENT,	// �������̂ݕ`��

	DRAWPART_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshWall(void);
HRESULT SetMeshWall(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col,
	int nNumBlockX, int nNumBlockY, XMFLOAT2 sizeBlock);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(EDrawPart dp = DRAWPART_ALL);
