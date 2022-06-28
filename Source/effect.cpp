/*=====�w�b�_�R�����g=====
*@�v���O������	�Feffect.cpp
*@�����E�T�v	�F�G�t�F�N�g�ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/24
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/

//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "effect.h"
#include "debugproc.h"
#include "Camera.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_EFFECT			L"data/texture/effect.png"
#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER			(1.0f)
#define M_AMBIENT		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_EFFECT		100		// �e�ő吔

#define Effect_FRAME_X		8
#define Effect_FRANE_Y		1
#define Effect_ANIM_COUNT	8


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct TEffect {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT2	m_size;		// �T�C�Y
	bool		use;		//�g�p���Ă��邩�ǂ���
	int			m_nanim;	//�A�j���[�V�����Đ��ʒu
	int			m_ncount;	//�A�j���[�V�����t���[����
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH g_mesh;			//���b�V�����
static MATERIAL g_material;	//�}�e���A��
static TEffect	g_Effect[MAX_EFFECT];	// �������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static HRESULT MakeVertexEffect(ID3D11Device* pDevise);

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	//���_���̍쐬
	hr = MakeVertexEffect(pDevice);
	if (FAILED(hr)) {
		return hr;
	}
	//�}�e���A���̐ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;
	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, MODEL_EFFECT, &g_mesh.pTexture);
	if (FAILED(hr)) return hr;
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	//���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	//������񏉊���
	for (int i = 0; i < MAX_EFFECT; ++i) {
		g_Effect[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Effect[i].m_size = XMFLOAT2(10.0f, 10.0f);
		g_Effect[i].use = false;
		g_Effect[i].m_nanim = 0;
		g_Effect[i].m_ncount = Effect_ANIM_COUNT;
	}

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	// ���f���̉��
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	TEffect* pEffect = g_Effect;
	for (int i = 0; i < MAX_EFFECT; ++i, ++pEffect) {
		//���g�p�Ȃ�X�L�b�v
		if (!pEffect->use) {
			continue;
		}
		//�J�E���^�X�V
		--pEffect->m_ncount;
		if (pEffect->m_ncount <= 0) {
			//�A�j���[�V�����̍X�V
			++pEffect->m_nanim;
			if (pEffect->m_nanim >= Effect_FRAME_X * Effect_FRANE_Y) {
				pEffect->use = false;	//����
				continue;
			}
			//�J�E���^�̏�����
			pEffect->m_ncount = Effect_ANIM_COUNT;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	//��������
	//SetBlendState(BS_ALPHABLEND);	//a�u�����f�B���O�L��
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	TEffect* pEffect = g_Effect;
	for (int i = 0; i < MAX_EFFECT; i++, pEffect++) {
		//���g�p�Ȃ�X�L�b�v
		if (!pEffect->use) {
			continue;
		}
		//�e�N�X�`���}�g���b�N�X�̍X�V
		XMMATRIX mtxTex = XMMatrixScaling(1.0f / Effect_FRAME_X, 1.0f / Effect_FRANE_Y, 1.0f);
		int u = pEffect->m_nanim % Effect_FRAME_X;
		int v = pEffect->m_nanim / Effect_FRAME_X;
		mtxTex = XMMatrixMultiply(mtxTex, XMMatrixTranslation((float)u / Effect_FRAME_X, (float)v / Effect_FRANE_Y, 0.0f));
		XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTex);
		//�r���[�s��̉�]�����̓]�u�s���ݒ�
		g_mesh.mtxWorld._11 = mView._11 * pEffect->m_size.x;
		g_mesh.mtxWorld._12 = mView._21 * pEffect->m_size.x;
		g_mesh.mtxWorld._13 = mView._31 * pEffect->m_size.x;
		g_mesh.mtxWorld._14 = 0.0f;
		g_mesh.mtxWorld._21 = mView._12 * pEffect->m_size.y;
		g_mesh.mtxWorld._22 = mView._22 * pEffect->m_size.y;
		g_mesh.mtxWorld._23 = mView._32 * pEffect->m_size.y;
		g_mesh.mtxWorld._24 = 0.0f;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;
		g_mesh.mtxWorld._34 = 0.0f;
		//�ʒu�𔽉f
		g_mesh.mtxWorld._41 = pEffect->m_pos.x;
		g_mesh.mtxWorld._42 = pEffect->m_pos.y;
		g_mesh.mtxWorld._43 = pEffect->m_pos.z;
		g_mesh.mtxWorld._44 = 1.0f;
		//�`��
		DrawMesh(pDC, &g_mesh);
	}
	//SetBlendState(BS_NONE);		//a�u�����f�B���O����
	CLight::Get()->SetEnable();	//�����L��
}

//=============================================================================
// �����J�n
//=============================================================================
int StartEffect(XMFLOAT3 pos, XMFLOAT2 size)
{
	pos.x = pos.x + 10.0f;
	TEffect*pEffect = g_Effect;
	for (int i = 0; i < MAX_EFFECT; i++, pEffect++) {
		//�g�p���Ȃ�X�L�b�v
		if (pEffect->use) {
			continue;
		}
		pEffect->m_pos = pos;
		pEffect->m_size = size;
		pEffect->m_nanim = 0;
		pEffect->m_ncount = Effect_ANIM_COUNT;
		pEffect->use = true;
		return i;	//�����ł���(0�`:�e�ԍ�)
	}
	return -1;		//�����ł��Ȃ�����
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexEffect(ID3D11Device* pDevice)
{
	//�ꎞ�I�Ȓ��_�z��𐶐�
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];
	//���_�z��̒��g�𖄂߂�
	pVertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);
	//�ꎞ�I�ȃC���f�b�N�X�z��𐶐�
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[4];

	//�C���f�b�N�X�̔z��𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	//���_�o�b�t�@/�C���f�b�N�X�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	//�ꎞ�I�Ȓ��_�z��/�C���f�b�N�X�z����J��
	delete[] pIndexWk;
	delete[] pVertexWk;
	return hr;
}
