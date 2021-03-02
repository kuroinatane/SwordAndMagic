/**
 * @file PhyXGenerator.h
 * @author MIURA DAIKI
 * @date 2020/02/01
 */
#pragma once
#include "Main.h"
#include "Singleton.h"
#include "PxPhysicsAPI.h"
#include <memory>

//#define 	PX_FOUNDATION_VERSION_MAJOR   1
//#define 	PX_FOUNDATION_VERSION_MINOR   0
//#define 	PX_FOUNDATION_VERSION_BUGFIX   0
//#define 	PX_FOUNDATION_VERSION   ((PX_FOUNDATION_VERSION_MAJOR << 24) + (PX_FOUNDATION_VERSION_MINOR << 16) + (PX_FOUNDATION_VERSION_BUGFIX << 8) + 0)


class C_Scene;
/**
 * @class PhyXGenerator
 * @brief 物理演算の共通データ管理クラス。初期化とシーンおよびシーン内のオブジェクト管理を担当する。
 */
class PhyXGenerator : public Singleton<PhyXGenerator>
{
private:
	float m_FPS;
	physx::PxDefaultAllocator      m_Allocator; //!< @brief SDKに必要なアロケーターインターフェイスのデフォルトの実装
	physx::PxDefaultErrorCallback  m_ErrorCallback; //!< @brief エラーコールバックのデフォルトの実装
	physx::PxFoundation*          m_Foundation = nullptr; //!< @brief FoundationSDKのシングルトンクラス。上位レベルのSDKをインスタンス化するには、このクラスのインスタンスが必要。
	physx::PxPhysics*            m_Physics = nullptr; //!< @brief　オブジェクトをインスタンス化するために使用する抽象シングルトンファクトリクラス。
	physx::PxDefaultCpuDispatcher* m_Dispatcher = nullptr; //!< @brief　CPUタスクディスパッチャーのデフォルトの実装。
	physx::PxScene*              m_Scene = nullptr; //!< @brief PhyXで使用されるシーン。
	physx::PxPvd*              m_Pvd = nullptr; //!< @brief デバッグ用
public:
	PhyXGenerator();
	~PhyXGenerator();
	void Init();
	void Update();
	void CreatePhyXScene(float fps);
	physx::PxScene* GetCurrentPhyXScene();
	physx::PxRigidDynamic* AddRigidDynamic(const physx::PxTransform& t, DirectX::XMFLOAT3 material, const physx::PxGeometry& geometry);
	physx::PxRigidStatic* AddRigidStatic(const physx::PxTransform& t, DirectX::XMFLOAT3 material, const physx::PxGeometry& geometry);
	void SetEventCallback(physx::PxSimulationEventCallback* callback);
};
