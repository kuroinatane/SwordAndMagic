/**
 * @file PhysXGenerator.h
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
 * @class PhysXGenerator
 * @brief 物理演算の共通データ管理クラス。初期化とシーンおよびシーン内のオブジェクト管理を担当する。
 */
class PhysXGenerator : public Singleton<PhysXGenerator>
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
	PhysXGenerator();
	~PhysXGenerator();
	void Init();
	void Update();
	void CreatePhyXScene(float fps, physx::PxSimulationEventCallback* callback);
	physx::PxScene* GetCurrentPhyXScene();
	physx::PxRigidDynamic* AddRigidDynamic(const physx::PxTransform& t, physx::PxShape* shape, float density);
	physx::PxRigidStatic* AddRigidStatic(const physx::PxTransform& t, physx::PxShape* shape);
	void SetEventCallback(physx::PxSimulationEventCallback* callback);
	physx::PxShape* CreatePhysXShape(DirectX::XMFLOAT3 material, const physx::PxGeometry& geometry, bool isExclusive = false, physx::PxShapeFlags shapeFlags = physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eSCENE_QUERY_SHAPE | physx::PxShapeFlag::eSIMULATION_SHAPE);
};

physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);