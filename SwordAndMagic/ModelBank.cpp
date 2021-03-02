#include "ModelBank.h"
#include "FbxLight.h"
#include "SceneController.h"
#include "GraphicsManager.h"

/**
* @brief �R���X�g���N�^�B
*/
ModelBank::ModelBank() {

}

/**
* @brief �f�X�g���N�^�B���f���f�[�^�̈ꊇ���
*/
ModelBank::~ModelBank() {
	modelCashe.clear();
}

/**
* @brief ���f���f�[�^�̃L���b�V���B���ɓǂݍ���ł���ꍇ�͓ǂݍ��ݍς݂̃f�[�^��Ԃ��B
* @param[in] name ���f���̃t�@�C����
* @param[in] vtxShader ���f���̒��_�V�F�[�_�̃t�@�C����(�f�t�H���g����NULL)
* @param[in] pixShader ���f���̃s�N�Z���V�F�[�_�̃t�@�C����(�f�t�H���g����NULL)
* @return �ǂݍ��܂ꂽ�f�[�^
*/
C_FbxModel* ModelBank::ModelCashe(LPCSTR name, LPCSTR vtxShader, LPCSTR pixShader) {
	std::string str(name);
	if (modelCashe[str]) return modelCashe[str].get();
	modelCashe[str] = std::make_unique<C_FbxModel>(name, vtxShader, pixShader);
	modelCashe[str]->SetLight(SceneController::GetInstance().GetCurrentScene()->GetLight());
	return modelCashe[str].get();
}

