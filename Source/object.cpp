/*=====�w�b�_�R�����g=====
*@�v���O������	�FObject.cpp
*@�����E�T�v	�F�I�u�W�F�N�g�ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "object.h"
#include "Camera.h"
#include "shadow.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"
#include "model.h"
#include "debugproc.h"
#include "score.h"
#include "effect.h"
#include "stageSelect.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_OBJECT	L"data/texture/book.gif"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BOOK_WIDTH		(30.0f)						// �؂̕�
#define	BOOK_HEIGHT		(50.0f)						// �؂̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexObject(ID3D11Device* pDevice);
void SetVertexObject(int idxObject, float width, float height);
void SetColorObject(int idxObject, XMFLOAT4 col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH				g_mesh;				// ���b�V�����
static MATERIAL			g_material;			// �}�e���A��
static TObject			g_object[30];		// �I�u�W�F�N�g���[�N
static int g_cntObject;
static int g_MaxObject;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitObject(void)
{
	ID3D11Device* pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexObject(pDevice);

	// �e�N�X�`���̓ǂݍ���
	CreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
						  TEXTURE_OBJECT,			// �t�@�C���̖��O
						  &g_mesh.pTexture);	// �ǂݍ��ރ�����
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	g_material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_material.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_material.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_material.Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_material.Power = 0.0f;
	g_mesh.pMaterial = &g_material;
	switch (GetStageSelect())
	{
	case STAGE_1:
		g_MaxObject = 10;
		break;
	case STAGE_2:
		g_MaxObject = 20;
		break;
	case STAGE_3:
		g_MaxObject = 30;
		break;
	}
	for (g_cntObject = 0; g_cntObject < g_MaxObject; ++g_cntObject) {
		g_object[g_cntObject].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_object[g_cntObject].col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		g_object[g_cntObject].rot = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
		g_object[g_cntObject].sclModel = XMFLOAT3(20.0f, 20.0f, 20.0f);
		g_object[g_cntObject].width = BOOK_WIDTH;
		g_object[g_cntObject].height = BOOK_HEIGHT;
		g_object[g_cntObject].use = false;
	}
	for (int nCntObject = 0; nCntObject < g_MaxObject; ++nCntObject) {
		float fPosX = (rand() % 8200) / 10.0f - 310.0f;
		float fPosY = 0.0f;
		float fPosZ = (rand() % 8200) / 10.0f - 310.0f;
		SetObject(XMFLOAT3(fPosX, fPosY, fPosZ), 30.0f, 40.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitObject(void)
{
	for (g_cntObject = 0; g_cntObject < g_MaxObject; ++g_cntObject) {
		if (g_object[g_cntObject].use) {
			ReleaseShadow(g_object[g_cntObject].idxShadow);
			g_object[g_cntObject].idxShadow = -1;
			g_object[g_cntObject].use = false;
		}
	}
	// ���b�V���̊J��
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateObject(void)
{
	for (int cntObject = 0; cntObject < g_MaxObject; ++cntObject) {
		// ���g�p�Ȃ�X�L�b�v
		if (!g_object[cntObject].use) {
			continue;
		}
		// �e�̈ʒu�ݒ�
		MoveShadow(g_object[cntObject].idxShadow, XMFLOAT3(g_object[cntObject].pos.x, 0.1f, g_object[cntObject].pos.z));
	}
	for (g_cntObject = 0; g_cntObject < g_MaxObject; ++g_cntObject) {
		//�v���C���[�̓����蔻��
		if (CollisionPlayer(g_object[g_cntObject].pos, g_object[g_cntObject].sclModel)) {
			//�I�u�W�F�N�g����
			StartEffect(g_object[g_cntObject].pos, XMFLOAT2(BOOK_WIDTH, BOOK_HEIGHT));
			g_object[g_cntObject].pos.z = 50000.0f;
			CSound::SetVolume(SE_CRASH, 0.2f);
			CSound::Play(SE_CRASH);	//���ʉ�
			AddScore(-1);
			return;
		}
	}
	

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawObject(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate, mtxScl;

	CLight::Get()->SetDisable();	// ��������
	//SetBlendState(BS_ALPHABLEND);	// ���u�����f�B���O�L��

	// �r���[�}�g���b�N�X���擾
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	for (g_cntObject = 0; g_cntObject < g_MaxObject; ++g_cntObject) {
		// ���g�p�Ȃ�X�L�b�v
		if (!g_object[g_cntObject].use) {
			continue;
		}
		// ���[���h �}�g���b�N�X������
		mtxWorld = XMMatrixIdentity();
		XMStoreFloat4x4(&g_mesh.mtxWorld, mtxWorld);

		// ��]�𔽉f
		g_mesh.mtxWorld._11 = mtxView._11;
		g_mesh.mtxWorld._12 = mtxView._21;
		g_mesh.mtxWorld._13 = mtxView._31;
		g_mesh.mtxWorld._21 = mtxView._12;
		g_mesh.mtxWorld._22 = mtxView._22;
		g_mesh.mtxWorld._23 = mtxView._32;
		g_mesh.mtxWorld._31 = mtxView._13;
		g_mesh.mtxWorld._32 = mtxView._23;
		g_mesh.mtxWorld._33 = mtxView._33;
		mtxWorld = XMLoadFloat4x4(&g_mesh.mtxWorld);

		mtxScl = XMMatrixScaling(g_object[g_cntObject].sclModel.x, g_object[g_cntObject].sclModel.y, g_object[g_cntObject].sclModel.z);

		// �X�P�[���𔽉f (��]����ɔ��f)
		mtxScale = XMMatrixScaling(g_object[g_cntObject].width, g_object[g_cntObject].height, 1.0f);
		mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_object[g_cntObject].pos.x, g_object[g_cntObject].pos.y, g_object[g_cntObject].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		XMStoreFloat4x4(&g_mesh.mtxWorld, mtxWorld);

		// �F�̐ݒ�
		g_material.Diffuse = g_object[g_cntObject].col;

		// �|���S���̕`��
		DrawMesh(pDeviceContext, &g_mesh);
	}

	//SetBlendState(BS_NONE);		// ���u�����f�B���O����
	CLight::Get()->SetEnable();	// �����L��
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexObject(ID3D11Device* pDevice)
{
	// �ꎞ�I�Ȓ��_�z��𐶐�
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// ���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	// ���_���W�̐ݒ�
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2.0f, 0.0f, 0.0f);
	pVtx[1].vtx = XMFLOAT3(-1.0f / 2.0f, 1.0f, 0.0f);
	pVtx[2].vtx = XMFLOAT3( 1.0f / 2.0f, 0.0f, 0.0f);
	pVtx[3].vtx = XMFLOAT3( 1.0f / 2.0f, 1.0f, 0.0f);

	// �@���̐ݒ�
	pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[g_mesh.nNumIndex];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexObject(int idxObject, float width, float height)
{
	if (idxObject >= 0 && idxObject < g_MaxObject) {
		g_object[idxObject].width = width;
		g_object[idxObject].height = height;
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorObject(int idxObject, XMFLOAT4 col)
{
	if (idxObject >= 0 && idxObject < g_MaxObject) {
		g_object[idxObject].col = col;
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetObject(XMFLOAT3 pos, float width, float height, XMFLOAT4 col)
{
	int idxObject = -1;

	for (int cntObject = 0; cntObject < g_MaxObject; ++cntObject) {
		// �g�p���Ȃ�X�L�b�v
		if (g_object[cntObject].use) {
			continue;
		}
		g_object[cntObject].use = true;
		g_object[cntObject].pos = pos;

		// ���_���W�̐ݒ�
		SetVertexObject(cntObject, width, height);

		// ���_�J���[�̐ݒ�
		SetColorObject(cntObject, col);

		// �e�̐ݒ�
		g_object[cntObject].idxShadow = CreateShadow(g_object[cntObject].pos, g_object[cntObject].width * 0.5f);

		idxObject = cntObject;
		break;
	}

	return idxObject;
}

int GetCnt()
{
	return g_cntObject;
}