/*=====�w�b�_�R�����g=====
*@�v���O������	�Fcollision.h
*@�����E�T�v	�F�����蔻��ɂ���
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

bool CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br);
bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize);
