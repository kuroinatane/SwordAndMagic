#include "ModelData.h"
#include "SceneController.h"
#include "GraphicsManager.h"
#include "FbxModel.h"
#include "camera.h"

/**
* @brief �R���X�g���N�^�B���f���ƃV�F�[�_�[�̓ǂݍ��݁B
* @param[in] name ���f���̃t�@�C����
* @param[in] vtxShader ���f���̒��_�V�F�[�_�̃t�@�C����(�f�t�H���g����NULL)
* @param[in] pixShader ���f���̃s�N�Z���V�F�[�_�̃t�@�C����(�f�t�H���g����NULL)
*/
ModelDataComponent::ModelDataComponent(LPCSTR name, C_GameObject* object, LPCSTR vtxShader, LPCSTR pixShader, bool outline) {

	m_pObj = object;
	modelName = name;
	ModelBank::GetInstance().ModelCashe(modelName, vtxShader, pixShader);
	m_pSceneCamera = SceneController::GetInstance().GetCurrentScene()->GetCamera();
	if (outline) {
		ModelBank::GetInstance().ModelCashe(modelName)->SetOutline();
	}
}

/**
* @brief �f�X�g���N�^
*/
ModelDataComponent::~ModelDataComponent() {
	
}

/**
* @brief �A�j���[�^�[��ݒ�
* @param[in] animator �A�j���[�^�[���
*/
void ModelDataComponent::SetAnimator(std::unique_ptr<C_Animator> animator) {
	m_Animator = std::move(animator);
}

/**
* @brief �A�j���[�^�[���擾
* @return �A�j���[�^�[���
*/
C_Animator* ModelDataComponent::GetAnimator() {
	return m_Animator.get();
}

/**
* @brief �`��֐��B�A�j���[�^�[������΂����炩����擾
*/
void ModelDataComponent::Draw() {
	if (m_Animator) {
		C_FbxModel* pModel = ModelBank::GetInstance().ModelCashe(m_Animator->GetCurrentAnimation());
		pModel->SetAnimFrame(m_Animator->GetCurrentAnimFrame());
		pModel->SetCamera(m_pSceneCamera->GetPosition());
		pModel->Render(m_pObj->transform.getWorldMatrix(), eNoAffect);
	}
	else {
		C_FbxModel* pModel = ModelBank::GetInstance().ModelCashe(modelName);
		pModel->SetCamera(m_pSceneCamera->GetPosition());
		pModel->Render(m_pObj->transform.getWorldMatrix(), eNoAffect);
	}
}