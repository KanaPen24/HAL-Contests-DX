/*=====�w�b�_�R�����g=====
*@�v���O������	�F
*@�����E�T�v	�F
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "explosion.h"
#include "debugproc.h"
#include "Camera.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_EXPLOSION			L"data/texture/explosion000.png"
#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER			(1.0f)
#define M_AMBIENT		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_EXPLOSION		100		// �e�ő吔

#define EXPLOSION_FRAME_X		8
#define EXPLOSION_FRANE_Y		1
#define EXPLOSION_ANIM_COUNT	4


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct TExplosion {
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
static TExplosion	g_explosion[MAX_EXPLOSION];	// �������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static HRESULT MakeVertexExplosion(ID3D11Device* pDevise);

//=============================================================================
// ����������
//=============================================================================
HRESULT InitExplosion(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	//���_���̍쐬
	hr = MakeVertexExplosion(pDevice);
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
	hr = CreateTextureFromFile(pDevice, MODEL_EXPLOSION, &g_mesh.pTexture);
	if (FAILED(hr)) return hr;
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	//���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	//������񏉊���
	for (int i = 0; i < MAX_EXPLOSION; ++i) {
		g_explosion[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_explosion[i].m_size = XMFLOAT2(10.0f, 10.0f);
		g_explosion[i].use = false;
		g_explosion[i].m_nanim = 0;
		g_explosion[i].m_ncount = EXPLOSION_ANIM_COUNT;
	}

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitExplosion(void)
{
	// ���f���̉��
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateExplosion(void)
{
	TExplosion* pExplosion = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExplosion) {
		//���g�p�Ȃ�X�L�b�v
		if (!pExplosion->use) {
			continue;
		}
		//�J�E���^�X�V
		--pExplosion->m_ncount;
		if (pExplosion->m_ncount <= 0) {
			//�A�j���[�V�����̍X�V
			++pExplosion->m_nanim;
			if (pExplosion->m_nanim >= EXPLOSION_FRAME_X * EXPLOSION_FRANE_Y) {
				pExplosion->use = false;	//����
				continue;
			}
			//�J�E���^�̏�����
			pExplosion->m_ncount = EXPLOSION_ANIM_COUNT;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawExplosion(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	//��������
	SetBlendState(BS_ALPHABLEND);	//a�u�����f�B���O�L��
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	TExplosion* pExplosion = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; i++, pExplosion++) {
		//���g�p�Ȃ�X�L�b�v
		if (!pExplosion->use) {
			continue;
		}
		//�e�N�X�`���}�g���b�N�X�̍X�V
		XMMATRIX mtxTex = XMMatrixScaling(1.0f / EXPLOSION_FRAME_X, 1.0f / EXPLOSION_FRANE_Y, 1.0f);
		int u = pExplosion->m_nanim % EXPLOSION_FRAME_X;
		int v = pExplosion->m_nanim / EXPLOSION_FRAME_X;
		mtxTex = XMMatrixMultiply(mtxTex, XMMatrixTranslation((float)u / EXPLOSION_FRAME_X, (float)v / EXPLOSION_FRANE_Y, 0.0f));
		XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTex);
		//�r���[�s��̉�]�����̓]�u�s���ݒ�
		g_mesh.mtxWorld._11 = mView._11 * pExplosion->m_size.x;
		g_mesh.mtxWorld._12 = mView._21 * pExplosion->m_size.x;
		g_mesh.mtxWorld._13 = mView._31 * pExplosion->m_size.x;
		g_mesh.mtxWorld._14 = 0.0f;
		g_mesh.mtxWorld._21 = mView._12 * pExplosion->m_size.y;
		g_mesh.mtxWorld._22 = mView._22 * pExplosion->m_size.y;
		g_mesh.mtxWorld._23 = mView._32 * pExplosion->m_size.y;
		g_mesh.mtxWorld._24 = 0.0f;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;
		g_mesh.mtxWorld._34 = 0.0f;
		//�ʒu�𔽉f
		g_mesh.mtxWorld._41 = pExplosion->m_pos.x;
		g_mesh.mtxWorld._42 = pExplosion->m_pos.y;
		g_mesh.mtxWorld._43 = pExplosion->m_pos.z;
		g_mesh.mtxWorld._44 = 1.0f;
		//�`��
		DrawMesh(pDC, &g_mesh);
	}
	SetBlendState(BS_NONE);		//a�u�����f�B���O����
	CLight::Get()->SetEnable();	//�����L��
}

//=============================================================================
// �����J�n
//=============================================================================
int StartExplosion(XMFLOAT3 pos, XMFLOAT2 size)
{
	TExplosion*pExplosion = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; i++, pExplosion++) {
		//�g�p���Ȃ�X�L�b�v
		if (pExplosion->use) {
			continue;
		}
		pExplosion->m_pos = pos;
		pExplosion->m_size = size;
		pExplosion->m_nanim = 0;
		pExplosion->m_ncount = EXPLOSION_ANIM_COUNT;
		pExplosion->use = true;
		return i;	//�����ł���(0�`:�e�ԍ�)
	}
	return -1;		//�����ł��Ȃ�����
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexExplosion(ID3D11Device* pDevice)
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
