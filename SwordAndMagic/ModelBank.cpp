#include "ModelBank.h"
#include "FbxLight.h"
#include "SceneController.h"
#include "GraphicsManager.h"

/**
* @brief コンストラクタ。
*/
ModelBank::ModelBank() {

}

/**
* @brief デストラクタ。モデルデータの一括解放
*/
ModelBank::~ModelBank() {
	modelCashe.clear();
}

/**
* @brief モデルデータのキャッシュ。既に読み込んである場合は読み込み済みのデータを返す。
* @param[in] name モデルのファイル名
* @param[in] vtxShader モデルの頂点シェーダのファイル名(デフォルト引数NULL)
* @param[in] pixShader モデルのピクセルシェーダのファイル名(デフォルト引数NULL)
* @return 読み込まれたデータ
*/
C_FbxModel* ModelBank::ModelCashe(LPCSTR name, LPCSTR vtxShader, LPCSTR pixShader) {
	std::string str(name);
	if (modelCashe[str]) return modelCashe[str].get();
	modelCashe[str] = std::make_unique<C_FbxModel>(name, vtxShader, pixShader);
	modelCashe[str]->SetLight(SceneController::GetInstance().GetCurrentScene()->GetLight());
	return modelCashe[str].get();
}

