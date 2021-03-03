#include "Scene.h"
#include "Camera.h"
#include <algorithm>
#include "PhysXGenerator.h"
#include "EffectGenerator.h"
#include "RigidDynamic.h"
#include "RigidStatic.h"
#include "EffectGenerator.h"

/**
 * @brief コンストラクタ、カメラ初期化
 */
C_Scene::C_Scene() {
	sceneCamera.reset(new C_Camera());
	sceneLight.reset(new C_FbxLight());
	PhysXGenerator::GetInstance().CreatePhyXScene(30, this);
	sceneObjList.clear();
	EffectGenerator::EffectGenerator();
	//PhysXGenerator::GetInstance().SetEventCallback();
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
	PhysXGenerator::GetInstance().Update();

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

/**
 * @brief IDでオブジェクト取得(主に当たり判定用)
 * @param[in] id 取得したいオブジェクトのID
 * @return 取得されたオブジェクトID
 */
C_GameObject* C_Scene::GetObjectWithID(int id) {
	std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
	for (itr = sceneObjList.begin(); itr != sceneObjList.end(); itr++) {
		if ((*itr)->GetID() == id) {
			return (*itr).get();
		}
	}
}


void C_Scene::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	for (physx::PxU32 i = 0; i < nbPairs; i++)
	{
		const physx::PxContactPair& cp = pairs[i];
		std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
		for (itr = sceneObjList.begin(); itr != sceneObjList.end(); itr++) {
			if (((*itr)->GetIsRigidbody())) {
				RigidDynamicComponent* rd = (*itr)->GetComponent<RigidDynamicComponent>();
				RigidStaticComponent* rs = (*itr)->GetComponent<RigidStaticComponent>();
				physx::PxRigidActor* actor;
				if (rd != NULL) {
					actor = rd->GetActor();
				}
				else if (rs != NULL) {
					actor = rs->GetActor();
				}
				else {
					continue;
				}

				if ((pairHeader.actors[0] == actor))
				{
					std::vector<std::unique_ptr<C_GameObject>>::iterator itr2;
					for (itr2 = sceneObjList.begin(); itr2 != sceneObjList.end(); itr2++) {
						if (((*itr)->GetIsRigidbody())) {
							RigidDynamicComponent* rd2 = (*itr2)->GetComponent<RigidDynamicComponent>();
							RigidStaticComponent* rs2 = (*itr2)->GetComponent<RigidStaticComponent>();
							physx::PxRigidActor* actor2;
							if (rd2 != NULL) {
								actor2 = rd2->GetActor();
							}
							else if (rs2 != NULL) {
								actor2 = rs2->GetActor();
							}
							else {
								continue;
							}
							if ((pairHeader.actors[1] == actor2)) {
								(*itr)->OnCollision((*itr2).get());
								break;
							}
						}
					}
				}
				else if ((pairHeader.actors[1] == actor))
				{
					std::vector<std::unique_ptr<C_GameObject>>::iterator itr2;
					for (itr2 = sceneObjList.begin(); itr2 != sceneObjList.end(); itr2++) {
						if (((*itr)->GetIsRigidbody())) {
							RigidDynamicComponent* rd2 = (*itr2)->GetComponent<RigidDynamicComponent>();
							RigidStaticComponent* rs2 = (*itr2)->GetComponent<RigidStaticComponent>();
							physx::PxRigidActor* actor2;
							if (rd2 != NULL) {
								actor2 = rd2->GetActor();
							}
							else if (rs2 != NULL) {
								actor2 = rs2->GetActor();
							}
							else {
								continue;
							}
							if ((pairHeader.actors[0] == actor2)) {
								(*itr)->OnCollision((*itr2).get());
								break;
							}
						}
					}
				}

			}
		}
	}
}

void C_Scene::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {
	for (physx::PxU32 i = 0; i < count; i++)
	{
		std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
		for (itr = sceneObjList.begin(); itr != sceneObjList.end(); itr++) {
			if (((*itr)->GetIsRigidbody())) {
				RigidDynamicComponent* rd = (*itr)->GetComponent<RigidDynamicComponent>();
				RigidStaticComponent* rs = (*itr)->GetComponent<RigidStaticComponent>();
				physx::PxRigidActor* actor;
				if (rd != NULL) {
					actor = rd->GetActor();
				}
				else if (rs != NULL) {
					actor = rs->GetActor();
				}
				else {
					continue;
				}

				if ((pairs[i].otherActor == actor))
				{
					std::vector<std::unique_ptr<C_GameObject>>::iterator itr2;
					for (itr2 = sceneObjList.begin(); itr2 != sceneObjList.end(); itr2++) {
						if (((*itr)->GetIsRigidbody())) {
							RigidDynamicComponent* rd2 = (*itr2)->GetComponent<RigidDynamicComponent>();
							RigidStaticComponent* rs2 = (*itr2)->GetComponent<RigidStaticComponent>();
							physx::PxRigidActor* actor2;
							if (rd2 != NULL) {
								actor2 = rd2->GetActor();
							}
							else if (rs2 != NULL) {
								actor2 = rs2->GetActor();
							}
							else {
								continue;
							}
							if ((pairs[i].triggerActor == actor2)) {
								(*itr)->OnCollision((*itr2).get());
								break;
							}
						}
					}
				}
				else if ((pairs[1].triggerActor == actor))
				{
					std::vector<std::unique_ptr<C_GameObject>>::iterator itr2;
					for (itr2 = sceneObjList.begin(); itr2 != sceneObjList.end(); itr2++) {
						if (((*itr)->GetIsRigidbody())) {
							RigidDynamicComponent* rd2 = (*itr2)->GetComponent<RigidDynamicComponent>();
							RigidStaticComponent* rs2 = (*itr2)->GetComponent<RigidStaticComponent>();
							physx::PxRigidActor* actor2;
							if (rd2 != NULL) {
								actor2 = rd2->GetActor();
							}
							else if (rs2 != NULL) {
								actor2 = rs2->GetActor();
							}
							else {
								continue;
							}
							if ((pairs[i].otherActor == actor2)) {
								(*itr)->OnCollision((*itr2).get());
								break;
							}
						}
					}
				}

			}
		}
	}
}