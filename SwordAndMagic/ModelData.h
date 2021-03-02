/**
 * @file ModelData.h
 * @brief fbx�̃f�[�^�Ǘ��p�B�A�j���[�V���������ێ��B
 * @author MIURA DAIKI
 * @date 2020/01/18
 */
#pragma once
#include "main.h"
#include "Component.h"
#include "FbxModel.h"
#include "GameObj.h"
#include "ModelBank.h"
#include "Animator.h"
#include <memory>

class C_Camera;

/**
 * @class ModelDataComponent
 * @brief fbx�̃f�[�^�Ǘ��p�BTODO:�A�j���[�V�����͂�����ʃN���X�ɐ؂藣��������\��B
 */
class ModelDataComponent:public C_Component {
public:
	ModelDataComponent(LPCSTR name, C_GameObject* object, LPCSTR vtxShader = NULL, LPCSTR pixShader = NULL, bool outline = false);
	~ModelDataComponent();
	void Draw();
	void SetAnimator(std::unique_ptr<C_Animator> animator);
	C_Animator* GetAnimator();
private:
	C_GameObject* m_pObj;
	C_Camera* m_pSceneCamera;
	std::unique_ptr<C_Animator> m_Animator;
	LPCSTR  modelName;
};