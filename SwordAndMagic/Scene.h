/**
 * @file Scene.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Camera.h"
#include "FbxLight.h"
#include "GameObj.h"
#include "PhysXGenerator.h"

#include <memory>
#include <vector>


class C_GameObject;
 /**
  * @class C_Scene
  * @brief シーンのベースクラス。基本的にシーンの基本的な動きは同じなので、
  *		   コンストラクタでシーンごとのオブジェクトを生成する形で対応する。
  *		   シーンごとに「どのオブジェクトを置くか」以外の機能はすべて共通なので
  *		   ベースクラスにすべての機能を実装し、子クラスはprivateで継承する
  */
class C_Scene : public physx::PxSimulationEventCallback {
private:
	bool m_bFirst = true;
	std::unique_ptr<C_Camera> sceneCamera;	//!< @brief シーンで使用するカメラ
	std::unique_ptr<C_FbxLight> sceneLight;	//!< @brief シーンで使用するカメラ
	std::vector<std::unique_ptr<C_GameObject>> sceneObjList;	//!< @brief シーン上の全オブジェクト
	std::vector<std::unique_ptr<C_GameObject>> addObjList;	//!< @brief シーンに追加予定のオブジェクト(ムーブ代入で所有権を移動させる)
	std::vector<int> deleteObjList;	//!< @brief シーンから削除予定のオブジェクトの番号
public:
	C_Scene();

	/**
	 * @brief オーバーライド用、全てのシーンオブジェクトを生成する純粋
	*/
	virtual void SpawnSceneObject() = 0;
	C_Scene(C_Camera* camera, C_FbxLight* light);
	~C_Scene();
	C_Camera* GetCamera();
	C_FbxLight* GetLight();
	void Init();
	void Update();
	void Draw();
	void Draw2D();
	void DeleteObject(int objID);
	void AddObject(std::unique_ptr<C_GameObject> obj);
	C_GameObject* GetObjectWithID(int id);

	virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
	virtual void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) {}
	virtual void onWake(physx::PxActor**, physx::PxU32) {}
	virtual void onSleep(physx::PxActor**, physx::PxU32) {}
	virtual void onAdvance(const physx::PxRigidBody*const*, const physx::PxTransform*, const physx::PxU32) {}
};