#include "Camera.h"
#include "input.h"


using namespace DirectX;

/**
 * @brief �J�����̈ʒu�A��]�Ȃǂ̏����������B�����Ȃ��o�[�W�����ł͒萔�ŏ���������B
 */
C_Camera::C_Camera() {
	posCameraP = XMFLOAT3(CAM_START_POS.x, CAM_START_POS.y, CAM_START_POS.z);
	posCameraR = XMFLOAT3(CAM_START_POS_R.x, CAM_START_POS_R.y, CAM_START_POS_R.z);
	vecCameraU = XMFLOAT3(0.0f, 1.0f, 0.0f);
	rotCamera = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Set();

}

/**
 * @brief �J�����̈ʒu�A��]�Ȃǂ̏����������B
 * @param[in] pos �J�����̎��_�ʒu
 * @param[in] posr �J�����̏œ_�ʒu
 */
C_Camera::C_Camera(XMFLOAT3 pos, XMFLOAT3 posr) {
	posCameraP = XMFLOAT3(pos.x, pos.y, pos.z);
	posCameraR = XMFLOAT3(posr.x, posr.y, posr.z);
	vecCameraU = XMFLOAT3(0.0f, 1.0f, 0.0f);
	rotCamera = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Set();
}

/**
 * @brief �j������f�[�^�Ȃǂ͓��ɖ������ߌ���͏�������
 */
C_Camera::~C_Camera() {

}

/**
 * @brief �J�����̐ݒ���s���A�U�����͎��t���[���̐U���l�̌v�Z���s��
 */
void C_Camera::Update(void)
{
	Set();
	
	if (fabs(shakePower) < 0.01f) {
		shakePower = 0.0f;
	}
	else {
		shakePower *= -0.999f;
	}
}

/**
 * @brief �X�V��������̌Ăяo���p�B�r���[�}�g���N�X�A�v���W�F�N�V�����}�g���N�X���X�V���ă����o�ϐ��Ɋi�[����B
 */
void C_Camera::Set(void)
{
	//�r���[�}�g���N�X�̍쐬
	XMStoreFloat4x4(&mtxView,
		XMMatrixLookAtLH(
			XMVectorSet(posCameraP.x, posCameraP.y + shakePower, posCameraP.z, 1.f),
			XMVectorSet(posCameraR.x, posCameraR.y + shakePower, posCameraR.z, 1.f),
			XMVectorSet(vecCameraU.x, vecCameraU.y, vecCameraU.z, 0.f)));

	//�v���W�F�N�V�����}�g���N�X�̍쐬
	XMStoreFloat4x4(&mtxProjection,
		XMMatrixPerspectiveFovLH(
			VIEW_ANGLE,
			GetViewAspect(),
			VIEW_NEAR_Z,
			VIEW_FAR_Z));
}

/**
 * @brief �J�����̎��_�ʒu�擾�B�O������̌Ăяo���p�B
 * @return �J�����̎��_�ʒu
 */
XMFLOAT3& C_Camera::GetPosition(void)
{
	return posCameraP;
}

/**
 * @brief �J�����̎��_�ʒu��ύX�B�Ɠ����ɃJ�����̏œ_�ʒu�������������s�ړ�����B
 */
void C_Camera::Move(XMFLOAT3 pos)
{
	posCameraP.x += pos.x;
	posCameraP.y += pos.y;
	posCameraP.z += pos.z;
	posCameraR.x += pos.x;
	posCameraR.y += pos.y;
	posCameraR.z += pos.z;
}

void C_Camera::SetPositionP(DirectX::XMFLOAT3 pos) {
	posCameraP.x = pos.x;
	posCameraP.y = pos.y;
	posCameraP.z = pos.z;
}

void C_Camera::SetPositionR(DirectX::XMFLOAT3 pos) {
	posCameraR.x = pos.x;
	posCameraR.y = pos.y;
	posCameraR.z = pos.z;
}

/**
 * @brief �J�����̃r���[�s����擾����B
 * @return �J�����̃r���[�s��
 */
XMFLOAT4X4& C_Camera::GetViewMatrix(void)
{
	return mtxView;
}

/**
 * @brief �J�����̃v���W�F�N�V�����s����擾����B
 * @return �J�����̃v���W�F�N�V�����s��
 */
XMFLOAT4X4& C_Camera::GetProjMatrix(void)
{
	return mtxProjection;
}

/**
 * @brief �J��������]
 * @param[in] rot ��]�l
 */
void C_Camera::Rotate(XMFLOAT3 rot)
{
	XMFLOAT3 diff;
	diff.x = posCameraR.x - posCameraP.x;
	diff.y = posCameraR.y - posCameraP.y;
	diff.z = posCameraR.z - posCameraP.z;

	DirectX::XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&diff),XMMatrixRotationRollPitchYaw(rot.x,rot.y,rot.z));
	
	XMFLOAT3 res;
	XMStoreFloat3(&res, vec);

	posCameraR.x = posCameraP.x + res.x;
	posCameraR.y = posCameraP.y + res.y;
	posCameraR.z = posCameraP.z + res.z;
}


/**
 * @brief �J�����̎����x�N�g�����擾�B�r���{�[�h�����ȂǂɎg�p�B
 * @return �J�����̎����x�N�g��
 */
XMFLOAT3 C_Camera::GetLookAt() {
	XMFLOAT3 diff;
	diff.x = posCameraR.x - posCameraP.x;
	diff.y = posCameraR.y - posCameraP.y;
	diff.z = posCameraR.z - posCameraP.z;
	XMVECTOR vec = XMVector3Normalize(XMLoadFloat3(&diff));
	XMStoreFloat3(&diff, vec);
	return diff;
}

/**
 * @brief �J�����̐U���̐ݒ�B����͒P������ł����c�ɗh�炷���Ƃɂ��Ă���B
 */
void C_Camera::SetShake(float pow) {
	shakePower = pow;
}


