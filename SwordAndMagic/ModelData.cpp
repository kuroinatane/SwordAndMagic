#include "ModelData.h"
#include "SceneController.h"
#include "GraphicsManager.h"
#include "FbxModel.h"
#include "camera.h"

/**
* @brief コンストラクタ。モデルとシェーダーの読み込み。
* @param[in] name モデルのファイル名
* @param[in] vtxShader モデルの頂点シェーダのファイル名(デフォルト引数NULL)
* @param[in] pixShader モデルのピクセルシェーダのファイル名(デフォルト引数NULL)
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
* @brief デストラクタ
*/
ModelDataComponent::~ModelDataComponent() {
	
}

/**
* @brief アニメーターを設定
* @param[in] animator アニメーター情報
*/
void ModelDataComponent::SetAnimator(std::unique_ptr<C_Animator> animator) {
	m_Animator = std::move(animator);
}

/**
* @brief アニメーターを取得
* @return アニメーター情報
*/
C_Animator* ModelDataComponent::GetAnimator() {
	return m_Animator.get();
}

/**
* @brief 描画関数。アニメーターがあればそちらから情報取得
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