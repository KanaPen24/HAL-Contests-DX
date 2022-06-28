/*=====�w�b�_�R�����g=====
*@�v���O������	�Fmeshfield.h
*@�����E�T�v	�F���b�V���𗘗p�����t�B�[���h(��)�ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ, float fTexSizeX = 1.0f, float fTexSizeZ = 1.0f);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
