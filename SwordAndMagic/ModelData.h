/**
 * @file ModelData.h
 * @brief fbxのデータ管理用。アニメーション情報も保持。
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
 * @brief fbxのデータ管理用。TODO:アニメーションはいずれ別クラスに切り離しをする予定。
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