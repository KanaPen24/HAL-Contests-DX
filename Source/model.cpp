/*=====�w�b�_�R�����g=====
*@�v���O������	�Fmodel.cpp
*@�����E�T�v	�F�v���C���[�̃��f���y�ы����ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*///*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "model.h"
#include "main.h"
#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "explosion.h"
#include "effect.h"
#include "collision.h"
#include "object.h"
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ANIM_IDLE			(0)
#define ANIM_WALK			(1)

#define	VALUE_MOVE_MODEL	(0.55f)		// �ړ����x
#define GRAVITY_MOVE		(0.1f)		//�������x
#define TURN_MOVE_MODEL		(0.50f)		// ���񑬓x
#define	RATE_MOVE_MODEL		(0.25f)		// �ړ������W��
#define	VALUE_ROTATE_MODEL	(9.0f)		// ��]���x
#define	RATE_ROTATE_MODEL	(0.20f)		// ��]�����W��
#define	PLAYER_RADIUS		(10.0f)		// ���E�����a

#define GAMEPAD_LEFT_STICK_DEADZONE  (7800)	//���X�e�B�b�N�̃f�b�h�]�[��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAssimpModel	g_model;		// ���f��

static XMFLOAT3		g_posModel;		// ���݂̈ʒu
static XMFLOAT3		g_rotModel;		// ���݂̌���
static XMFLOAT3		g_rotDestModel;	// �ړI�̌���
static XMFLOAT3		g_moveModel;	// �ړ���
static XMFLOAT3		g_sclModel;		// �X�P�[��
static XMFLOAT3		g_posEffect;
static XMFLOAT2		g_sizeEffect;
static LPCSTR		g_pszModelPath = "data/model/Player.fbx";

static XMFLOAT4X4	g_mtxWorld;		// ���[���h�}�g���b�N�X

static int			g_nShadow;		// �ۉe�ԍ�

static bool			g_bAngleFlg[4];	// �p�x�ύX�p(0:�� 1:�E 2:�� 3:��)
static bool			g_bSAngleFlg[4];// �p�x�ύX�p(0:�� 1:�E 2:�� 3:��)�X�e�B�b�N�p
static int g_nAnimStack;
static double g_dAnimTime;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitModel(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_posModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_sclModel = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_posEffect = g_posModel;
	g_sizeEffect = XMFLOAT2(10.0f, 10.0f);
	for (int i = 0; i < 4; i++) {
		g_bAngleFlg[i] = false;
		g_bSAngleFlg[i] = false;
	}
	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, g_pszModelPath)) {
		hr = E_FAIL;
		MessageBoxA(GetMainWnd(), "���f���f�[�^�Ǎ��G���[", "InitModel", MB_OK | MB_ICONEXCLAMATION);
	}


	// �ۉe�̐���
	g_nShadow = CreateShadow(g_posModel, 12.0f);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitModel(void)
{
	// �ۉe�̉��
	ReleaseShadow(g_nShadow);

	// ���f���̉��
	g_model.Release();

	
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateModel(void)
{
	DWORD JoyCount = GetJoyCount();
	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();
	int g_rot = 1;
	g_posEffect = g_posModel;
	*GetJoyState(JOYSTICKID1);
	LONG JoyX = GetJoyX(JOYSTICKID1);
	LONG JoyY = GetJoyY(JOYSTICKID1);
	//�R���g���[���[����
	if ((JoyX <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyX > -GAMEPAD_LEFT_STICK_DEADZONE) &&
		(JoyY<  GAMEPAD_LEFT_STICK_DEADZONE &&
			JoyY > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyX = 0;
		JoyY = 0;
	}

	/*��������R���g���[���[����̓���*/
	if (JoyCount >= 1) {

		if (JoyX <= -GAMEPAD_LEFT_STICK_DEADZONE) {
			if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE) {
				// ���O�ړ�
				g_moveModel.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_MODEL;
				g_moveModel.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_MODEL;

				g_rotDestModel.y = rotCamera.y + 135.0f;
			}
			else if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE) {
				// ����ړ�
				g_moveModel.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_MODEL;
				g_moveModel.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_MODEL;

				g_rotDestModel.y = rotCamera.y + 45.0f;
			}
			else {
				// ���ړ�
				g_moveModel.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_MODEL;
				g_moveModel.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_MODEL;

				g_rotDestModel.y = rotCamera.y + 90.0f;
			}
		}
		else if (JoyX >= GAMEPAD_LEFT_STICK_DEADZONE) {
			if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE) {
				// �E�O�ړ�
				g_moveModel.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_MODEL;
				g_moveModel.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_MODEL;

				g_rotDestModel.y = rotCamera.y - 135.0f;
			}
			else if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE) {
				// �E��ړ�
				g_moveModel.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_MODEL;
				g_moveModel.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_MODEL;

				g_rotDestModel.y = rotCamera.y - 45.0f;
			}
			else {
				// �E�ړ�
				g_moveModel.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_MODEL;
				g_moveModel.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_MODEL;

				g_rotDestModel.y = rotCamera.y - 90.0f;
			}

		}
		else if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE) {
			// �O�ړ�
			g_moveModel.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = 180.0f + rotCamera.y;
		}
		else if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE) {
			// ��ړ�
			g_moveModel.x -= SinDeg(rotCamera.y) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y;
		}
	}
	//�L�[�{�[�h����
	if (GetKeyPress(VK_LEFT)) {
		if (GetKeyPress(VK_UP)) {
			// ���O�ړ�
			g_moveModel.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y + 135.0f;
		} else if (GetKeyPress(VK_DOWN)) {
			// ����ړ�
			g_moveModel.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y + 45.0f;
		} else {
			// ���ړ�
			g_moveModel.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y + 90.0f;
		}
	} else if (GetKeyPress(VK_RIGHT)) {
		if (GetKeyPress(VK_UP)) {
			// �E�O�ړ�
			g_moveModel.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y - 135.0f;
		} else if (GetKeyPress(VK_DOWN)) {
			// �E��ړ�
			g_moveModel.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y - 45.0f;
		} else {
			// �E�ړ�
			g_moveModel.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y - 90.0f;
		}

	} else if (GetKeyPress(VK_UP)) {
		// �O�ړ�
		g_moveModel.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_MODEL;
		g_moveModel.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_MODEL;

		g_rotDestModel.y = 180.0f + rotCamera.y;
	} else if (GetKeyPress(VK_DOWN)) {
		// ��ړ�
		g_moveModel.x -= SinDeg(rotCamera.y) * VALUE_MOVE_MODEL;
		g_moveModel.z -= CosDeg(rotCamera.y) * VALUE_MOVE_MODEL;

		g_rotDestModel.y = rotCamera.y;
	}

	if (GetKeyPress(VK_LSHIFT)) {
		// ����]
		g_rotDestModel.y -= VALUE_ROTATE_MODEL;
		if (g_rotDestModel.y < -180.0f) {
			g_rotDestModel.y += 360.0f;
		}
	}
	if (GetKeyPress(VK_RSHIFT)) {
		// �E��]
		g_rotDestModel.y += VALUE_ROTATE_MODEL;
		if (g_rotDestModel.y >= 180.0f) {
			g_rotDestModel.y -= 360.0f;
		}
	}

	// �ړI�̊p�x�܂ł̍���
	float fDiffRotY = g_rotDestModel.y - g_rotModel.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	g_rotModel.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (g_rotModel.y >= 180.0f) {
		g_rotModel.y -= 360.0f;
	}
	if (g_rotModel.y < -180.0f) {
		g_rotModel.y += 360.0f;
	}

	// �ʒu�ړ�
	g_posModel.x += g_moveModel.x;
	g_posModel.y += g_moveModel.y;
	g_posModel.z += g_moveModel.z;

	// �ړ��ʂɊ�����������
	g_moveModel.x += (0.0f - g_moveModel.x) * RATE_MOVE_MODEL;
	g_moveModel.y += (0.0f - g_moveModel.y) * RATE_MOVE_MODEL;
	g_moveModel.z += (0.0f - g_moveModel.z) * RATE_MOVE_MODEL;

	// �A�j���[�V�����X�V
	float fSpeed = sqrtf(g_moveModel.x * g_moveModel.x + g_moveModel.z * g_moveModel.z);
	PrintDebugProc("Speed=%f\n\n", fSpeed);
	PrintDebugProc("Animation=%d\n\n", g_nAnimStack);
	int nStack;
	if (fSpeed < 0.1f)
		nStack = ANIM_IDLE;
	if (fSpeed > 0.1f)
		nStack = ANIM_WALK;
	g_nAnimStack = nStack;
	static double dLastTime = clock() / double(CLOCKS_PER_SEC);
	double dNowTime = clock() / double(CLOCKS_PER_SEC);
	double dSlice = dNowTime - dLastTime;
	dLastTime = dNowTime;
	g_dAnimTime += dSlice;
	if (g_dAnimTime >= g_model.GetAnimDuration(g_nAnimStack)) {
		g_dAnimTime = 0.0;
	}


	//�ړ��ʌ��E�l
	if (g_posModel.x < -620.0f) {
		g_posModel.x = -620.0f;
	}
	if (g_posModel.x > 620.0f) {
		g_posModel.x = 620.0f;
	}
	if (g_posModel.z < -620.0f) {
		g_posModel.z = -620.0f;
	}
	if (g_posModel.z > 620.0f) {
		g_posModel.z = 620.0f;
	}
	if (g_posModel.y < 0.0f) {
		g_posModel.y = 0.0f;
	}
	if (g_posModel.y > 0.0f) {
		g_posModel.y = 0.0f;
	}

	if (GetKeyPress(VK_RETURN)) {
		// ���Z�b�g
		g_posModel = XMFLOAT3(0.0f, 20.0f, 0.0f);
		g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}




	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScl;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotModel.x),
		XMConvertToRadians(g_rotModel.y), XMConvertToRadians(g_rotModel.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	mtxScl = XMMatrixScaling(g_sclModel.x, g_sclModel.y, g_sclModel.z);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_posModel.x, g_posModel.y, g_posModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// �ۉe�̈ړ�
	MoveShadow(g_nShadow, g_posModel);



#ifdef _DEBUG

	PrintDebugProc("[˺�� �� : (%f : %f : %f)]\n", g_posModel.x, g_posModel.y, g_posModel.z);
	PrintDebugProc("[˺�� ѷ : (%f) <Ӹ÷ ��:(%f)>]\n", g_rotModel.y, g_rotDestModel.y);
	PrintDebugProc("\n%d",JoyCount);
	PrintDebugProc("\n%d", JoyX);
	PrintDebugProc("\n%d", JoyY);

	PrintDebugProc("\n");

	PrintDebugProc("*** ˺�� ��� ***\n");
	PrintDebugProc("ϴ   ��޳ : \x1e\n");//��
	PrintDebugProc("���  ��޳ : \x1f\n");//��
	PrintDebugProc("���� ��޳ : \x1d\n");//��
	PrintDebugProc("з�  ��޳ : \x1c\n");//��
	PrintDebugProc("�ޮ����   : I\n");
	PrintDebugProc("���       : K\n");
	PrintDebugProc("���� �ݶ� : LSHIFT\n");
	PrintDebugProc("з�  �ݶ� : RSHIFT\n");
	PrintDebugProc("��   ʯ�� : SPACE\n");
	PrintDebugProc("\n");
#endif // DEBUG
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawModel(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	g_model.Draw(pDC, g_mtxWorld, eOpacityOnly);

	// ������������`��
	//SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	// ���f���\��
	g_model.SetAnimIndex(g_nAnimStack);
	g_model.SetAnimTime(g_dAnimTime);
	g_model.Draw(pDC, g_mtxWorld);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	//SetBlendState(BS_NONE);			// �A���t�@�u�����h����
}

//=============================================================================
// �ʒu�擾
//=============================================================================
XMFLOAT3& GetModelPos()
{
	return g_posModel;
}

//=============================================================================
// �Փ˔���
//=============================================================================
bool CollisionPlayer(XMFLOAT3 pos, XMFLOAT3 scale)
{
	bool hit = CollisionAABB(g_posModel,g_sclModel , pos, scale);
	if (hit) {
		PrintDebugProc("����\n");
	}
	return hit;
}
