#include "Scene.h"
#include "Camera.h"
#include <algorithm>
#include "PhyXGenerator.h"
#include "EffectGenerator.h"

/**
 * @brief コンストラクタ、カメラ初期化
 */
C_Scene::C_Scene() {
	sceneCamera.reset(new C_Camera());
	sceneLight.reset(new C_FbxLight());
	PhyXGenerator::GetInstance().CreatePhyXScene(30);
	sceneObjList.clear();
	EffectGenerator::EffectGenerator();
}

/**
 * @brief コンストラクタ、カメラ設定
 */
C_Scene::C_Scene(C_Camera* camera, C_FbxLight* light) {
	sceneCamera.reset(camera);
	sceneLight.reset(light);

	sceneObjList.clear();
}

/**
 * @brief デストラクタ、全てのオブジェクトの参照を破棄
 */
C_Scene::~C_Scene() {
	//全てのオブジェクトの参照を破棄
	sceneObjList.clear();
	addObjList.clear();
	deleteObjList.clear();
}

/**
 * @brief カメラ情報の取得
 * @return シーンが使用しているカメラへのポインタ
 */
C_Camera* C_Scene::GetCamera() { 
	return sceneCamera.get();
}

/**
 * @brief メイン光源の取得
 * @return シーンが使用しているメイン光源へのポインタ
 */
C_FbxLight* C_Scene::GetLight() {
	return sceneLight.get();
}

/**
 * @brief AddObjの処理
 */
void C_Scene::Init() {
	SpawnSceneObject();
	//コンテナが空になるまでループし続ける
	std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
	for (itr = addObjList.begin(); itr != addObjList.end(); itr++) {
		sceneObjList.push_back(std::move((*itr)));
	}
	addObjList.clear();

	for (itr = sceneObjList.begin(); itr != sceneObjList.end(); itr++) {
		(*itr)->Start();
	}
}

/**
 * @brief シーン上のオブジェクト全ての更新→新規オブジェクトの追加
 */
void C_Scene::Update() {
	PhyXGenerator::GetInstance().Update();

	if (m_bFirst) {
		Init();
		m_bFirst = false;
	}
	sceneCamera->Update();
	std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
	for (itr = sceneObjList.begin(); itr != sceneObjList.end(); itr++) {
		(*itr)->Update();
	}

	for (itr = addObjList.begin(); itr != addObjList.end(); itr++) {
		(*itr)->Start();
		sceneObjList.push_back(std::move((*itr)));
	}
	addObjList.clear();

	//std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
	for (itr = sceneObjList.begin(); itr != sceneObjList.end();) {
		bool isDestroy = false;
		for (int deleteNum : deleteObjList) {
			if ((*itr)->GetID() == deleteNum) {
				isDestroy = true;
				break;
			}
		}
		if (isDestroy) {
			itr = sceneObjList.erase(itr);
		}
		else {
			itr++;
		}
	}
	deleteObjList.clear();
	EffectGenerator::GetInstance().Update();
}

/**
 * @brief 描画(3D座標上のオブジェクト)
 */
void C_Scene::Draw() {

	//オーダーソート
	std::sort(sceneObjList.begin(), sceneObjList.end(), [](const std::unique_ptr<C_GameObject>& lhoge, const std::unique_ptr<C_GameObject>& rhoge)
	{
		return lhoge->GetSortOrder() < rhoge->GetSortOrder();
	});

	std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
	for (itr = sceneObjList.begin(); itr != sceneObjList.end(); itr++) {
		if(((*itr)->Getis2D()) == false)
		(*itr)->Draw();
	}
	EffectGenerator::GetInstance().Draw();
}

/**
 * @brief 描画(スクリーン座標上の2Dオブジェクト)
 */
void C_Scene::Draw2D() {
	std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
	for (itr = sceneObjList.begin(); itr != sceneObjList.end(); itr++) {
		if (((*itr)->Getis2D()) == true)
			(*itr)->Draw();
	}
}

/**
 * @brief オブジェクトの削除
 * @param[in] objID 削除するオブジェクトの番号
 */
void C_Scene::DeleteObject(int objID) {
	deleteObjList.push_back(objID);
}

/**
 * @brief オブジェクトの追加
 * @param[in] obj make_uniqueで作成したオブジェクトのユニークポインタ
 */
void C_Scene::AddObject(std::unique_ptr<C_GameObject> obj) {
	addObjList.push_back(std::move(obj));
}