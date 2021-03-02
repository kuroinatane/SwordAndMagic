/**
 * @file ModelBank.h
 * @brief fbxのデータを読み込んで蓄積。同じfbxのデータを何度も読み込まないために保存しておく
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
  * @brief fbxのデータ保存用クラス(シングルトン実装)。
  */
class ModelBank :public Singleton<ModelBank> {
private:
	std::map<std::string,std::unique_ptr<C_FbxModel>> modelCashe; //!< @brief モデルデータ保存用
public:
	ModelBank();
	~ModelBank();
	C_FbxModel* ModelCashe(LPCSTR name, LPCSTR vtxShader = NULL, LPCSTR pixShader = NULL);
};