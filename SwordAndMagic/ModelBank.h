/**
 * @file ModelBank.h
 * @brief fbx�̃f�[�^��ǂݍ���Œ~�ρB����fbx�̃f�[�^�����x���ǂݍ��܂Ȃ����߂ɕۑ����Ă���
 * @author MIURA DAIKI
 * @date 2020/01/18
 */
#pragma once
#include "main.h"
#include "FbxModel.h"
#include "Singleton.h"
#include <string>
#include <map>
#include <memory>

 /**
  * @class ModelBank
  * @brief fbx�̃f�[�^�ۑ��p�N���X(�V���O���g������)�B
  */
class ModelBank :public Singleton<ModelBank> {
private:
	std::map<std::string,std::unique_ptr<C_FbxModel>> modelCashe; //!< @brief ���f���f�[�^�ۑ��p
public:
	ModelBank();
	~ModelBank();
	C_FbxModel* ModelCashe(LPCSTR name, LPCSTR vtxShader = NULL, LPCSTR pixShader = NULL);
};