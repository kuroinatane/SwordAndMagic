#pragma once
/**
 * @file Camera.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */

#include "Main.h"
#include "Utility.h"

constexpr DirectX::XMFLOAT3 CAM_START_POS = DirectX::XMFLOAT3(0.0f,0.0f,-300.0f); //!< @brief �J�����̏������_�ʒu
constexpr DirectX::XMFLOAT3 CAM_START_POS_R = DirectX::XMFLOAT3(0.0f,0.0f,0.0f); //!< @brief �J�����̏����œ_�ʒu
constexpr float VIEW_ANGLE = DirectX::XMConvertToRadians(45.0f); //!< @brief �J�����̉�p
constexpr float VIEW_NEAR_Z = (10.0f);	 //!< @brief �J�����̃j�A�N���b�v
constexpr float VIEW_FAR_Z = (1000.0f);	 //!< @brief �J�����̃t�@�[�N���b�v

 /**
  * @class C_Camera
  * @brief �J�������`����N���X�B
  *		  �V�[�����Ƃɐ������Ďg�����Ƃ�z�肵�Ă���̂ŃV�[���ׂ��͗v�����B
  */
class C_Camera
{
private:
	DirectX::XMFLOAT3		posCameraP;		//!< @brief �J�����̎��_�ʒu
	DirectX::XMFLOAT3		posCameraR;		//!< @brief �J�����̏œ_�ʒu
	DirectX::XMFLOAT3		vecCameraU;		//!< @brief �J�����̃A�b�v�x�N�g��
	DirectX::XMFLOAT3		rotCamera;		//!< @brief �J�����̉�]
	DirectX::XMFLOAT4X4	mtxView;			//!< @brief �J�����̃r���[�s��
	DirectX::XMFLOAT4X4	mtxProjection;		//!< @brief �J�����̃v���W�F�N�V�����s��
	float shakePower = 0;					//!< @brief �J�����̐U���l(�h�炵������)
public:
	C_Camera();
	C_Camera(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 posr);
	~C_Camera();
	void Update();
	void Set();
	DirectX::XMFLOAT3& GetPosition(void);
	void SetPosition(DirectX::XMFLOAT3);
	DirectX::XMFLOAT4X4& GetViewMatrix(void);
	DirectX::XMFLOAT4X4& GetProjMatrix(void);
	DirectX::XMFLOAT3 GetLookAt();
	void SetShake(float pow);

	float GetViewAspect() {
		return ((float)GetScreenSize().x / GetScreenSize().y);
	}
};

