/*=====�w�b�_�R�����g=====
*@�v���O������	�Fenemy.cpp
*@�����E�T�v	�F�G�̋����ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/24
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/

//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "enemy.h"
#include "main.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "model.h"
#include "fade.h"
#include "sound.h"
#include "stageSelect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_ENEMY			"data/model/Enemy.fbx"

#define	RATE_MOVE_ENEMY		(0.20f)		// �ړ������W��
#define	VALUE_ROTATE_ENEMY	(7.0f)		// ��]���x
#define	RATE_ROTATE_ENEMY	(0.20f)		// ��]�����W��

#define MAX_ENEMY			(1)		// �G�@�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct TEnemy {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3	m_rotDest;	// �ړI�̌���
	XMFLOAT3	m_move;		// �ړ���
	XMFLOAT3	m_sclModel;			// �X�P�[��

	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X

	int			m_nShadow;	// �ۉe�ԍ�
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAssimpModel	g_model;			// ���f��
static TEnemy		g_enemy[MAX_ENEMY];	// �G�@���
static float g_Speed;	//�ړ����x
//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		return E_FAIL;
	}
	switch (GetStageSelect())
	{
	case STAGE_1:
		g_Speed = 1.0f;
		break;
	case STAGE_2:
		g_Speed = 1.2f;
		break;
	case STAGE_3:
		g_Speed = 1.5f;
		break;
	}

	for (int i = 0; i < MAX_ENEMY; ++i) {
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_enemy[i].m_pos = XMFLOAT3(rand() % 620 - 310.0f,
									0.0f,
									rand() % 620 - 310.0f);
		g_enemy[i].m_rot = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
		g_enemy[i].m_rotDest = g_enemy[i].m_rot;
		g_enemy[i].m_move = XMFLOAT3(
			-SinDeg(g_enemy[i].m_rot.y) * g_Speed,
			0.0f,
			-CosDeg(g_enemy[i].m_rot.y) * g_Speed);
		g_enemy[i].m_sclModel = XMFLOAT3(1.00f, 1.00f, 1.00f);
		// �ۉe�̐���
		switch (GetStageSelect())
		{
		case STAGE_1:
			g_enemy[i].m_nShadow = CreateShadow(g_enemy[i].m_pos, 12.0f);
			break;
		case STAGE_2:
			g_enemy[i].m_nShadow = CreateShadow(g_enemy[i].m_pos, 12.0f);
			break;
		case STAGE_3:
			break;
		}
	}

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; ++i) {
		// �ۉe�̉��
		switch (GetStageSelect())
		{
		case STAGE_1:
			ReleaseShadow(g_enemy[i].m_nShadow);
			break;
		case STAGE_2:
			ReleaseShadow(g_enemy[i].m_nShadow);
			break;
		case STAGE_3:
			break;
		}

	}

	// ���f���̉��
	g_model.Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	XMMATRIX mtxWorld, mtxTranslate, mtxScl;
	static XMFLOAT3 pPos;			// �v���C���[�\���ʒu
	pPos = GetModelPos();		//�v���C���[�̍��W�擾

	for (int i = 0; i < MAX_ENEMY; ++i) {
		// �ړ�
		// �ړI�̊p�x�܂ł̍���
		float fDiffRotY = g_enemy[i].m_rotDest.y - g_enemy[i].m_rot.y;
		// �v���C���[��ǔ�����
		if (pPos.x > g_enemy[i].m_pos.x) {
			g_enemy[i].m_pos.x += g_Speed;
		}
		else 
		{
			g_enemy[i].m_pos.x -= g_Speed;
		}
		if (pPos.z > g_enemy[i].m_pos.z)
		{
			g_enemy[i].m_pos.z += g_Speed;
		}
		else 
		{
			g_enemy[i].m_pos.z -= g_Speed;
		}

		// �ړI�̊p�x�܂Ŋ�����������
		g_enemy[i].m_rot.y += fDiffRotY * RATE_ROTATE_ENEMY;
		if (g_enemy[i].m_rot.y >= 180.0f) {
			g_enemy[i].m_rot.y -= 360.0f;
		}
		if (g_enemy[i].m_rot.y < -180.0f) {
			g_enemy[i].m_rot.y += 360.0f;
		}
		g_enemy[i].m_move = XMFLOAT3(-SinDeg(g_enemy[i].m_rot.y) * g_Speed,
			0.0f,-CosDeg(g_enemy[i].m_rot.y) * g_Speed);
		//�v���C���[�̓����蔻��
		if (CollisionPlayer(g_enemy[i].m_pos, g_enemy[i].m_sclModel)) {
			//�Q�[���I�[�o�[����
			StartCutOut(SCENE_GAMEOVER);
			CSound::SetVolume(SE_DAMAGE, 0.2f);
			CSound::Play(SE_DAMAGE);
		}

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_enemy[i].m_sclModel.x, g_enemy[i].m_sclModel.y, g_enemy[i].m_sclModel.z);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(
			g_enemy[i].m_pos.x,
			g_enemy[i].m_pos.y,
			g_enemy[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemy[i].m_mtxWorld, mtxWorld);

		// �ۉe�̈ړ�
		switch (GetStageSelect())
		{
		case STAGE_1:
			MoveShadow(g_enemy[i].m_nShadow, g_enemy[i].m_pos);
			break;
		case STAGE_2:
			MoveShadow(g_enemy[i].m_nShadow, g_enemy[i].m_pos);
			break;
		case STAGE_3:
			break;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
}
