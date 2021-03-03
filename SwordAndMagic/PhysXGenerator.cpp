#include "PhysXGenerator.h"

using namespace physx;

/*
* @brief コンストラクタ。
*/
PhysXGenerator::PhysXGenerator() {
	Init();
}

/*
* @brief デストラクタ。
*/
PhysXGenerator::~PhysXGenerator() {

}

/*
* @brief 初期化。各種設定項目を設定。設定したい項目が出てきたら引数を入れて対応していく予定
*/
void PhysXGenerator::Init() {
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	// PVD(PhysX Visual Debugger) setting. To use PVD, we need to build the project as Debug mode.
	m_Pvd = PxCreatePvd(*m_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_Pvd);
}

/*
* @brief 更新処理。更新速度は現在のシーンのfpsに依存する。
*/
void PhysXGenerator::Update() {
	m_Scene->simulate(1.0f / m_FPS);
	m_Scene->fetchResults(true);
}

/*
* @brief PhyX用シーン生成。基本的にゲームシーンｔ紐付ける形で生成する。
* @param[in] fps そのシーンの物理演算用のfpsを指定。負の値を設定すると60fps。
*/
void PhysXGenerator::CreatePhyXScene(float fps, PxSimulationEventCallback* callback) {
	// Scene setting
	if (m_Scene) {
		m_Scene->release();
	}

	PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);          // 右手座標系、Y+が↑なのでY-に重力
	m_Dispatcher = PxDefaultCpuDispatcherCreate(1);         // The number of worker threads is one.
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = callback;
	m_Scene = m_Physics->createScene(sceneDesc); //使っているシーンがある場合はここで破棄される
	if (fps < 0.0f) m_FPS = 60.0f;
	else m_FPS = fps;
	//// PVD setting
	PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

/*
* @brief PhyX用シーン取得。
* @return シーン情報
*/
PxScene* PhysXGenerator::GetCurrentPhyXScene() {
	return m_Scene;
}

/*
* @brief 現在のシーン上に動的剛体オブジェクトを生成する。生成データはコンポーネント側で受け取って利用する必要がある。
* @param[in] t オブジェクトの位置情報。位置と回転を指定できる。
* @param[in] material x=静摩擦 y=動摩擦 z=跳ね返り係数
* @param[in] geometry オブジェクトの形状情報。Px****Geometryで生成。
* @param[in] density 物理シミュレーションの「強さ」。0にすると無効。大きくするほど大げさな挙動になる。
*/
PxRigidDynamic* PhysXGenerator::AddRigidDynamic(const PxTransform& t, PxShape* shape, float density) {
	PxRigidDynamic* rd = physx::PxCreateDynamic(*m_Physics, t, *shape, density);
	m_Scene->addActor(*rd);
	return rd;
}

/*
* @brief 現在のシーン上に静的剛体オブジェクトを生成する。生成データはコンポーネント側で受け取って利用する必要がある。
* @param[in] t オブジェクトの位置情報。位置と回転を指定できる。
* @param[in] shape オブジェクトの形状情報。CreatePhysXShapeで生成。
*/
PxRigidStatic* PhysXGenerator::AddRigidStatic(const PxTransform& t, PxShape* shape) {
	PxRigidStatic* rs = physx::PxCreateStatic(*m_Physics, t, *shape);
	m_Scene->addActor(*rs);
	return rs;
}

/*
* @brief 形状の生成
* @param[in] material x=静摩擦 y=動摩擦 z=跳ね返り係数
* @param[in] geometry オブジェクトの形状情報。Px****Geometryで生成。
*/
PxShape* PhysXGenerator::CreatePhysXShape(DirectX::XMFLOAT3 material, const PxGeometry& geometry,bool isExclusive ,PxShapeFlags shapeFlags) {
	PxMaterial* pMaterial = m_Physics->createMaterial(material.x, material.y, material.z);
	return m_Physics->createShape(geometry, *pMaterial, true);
}

/*
* @brief 衝突コールバックイベントの設定
* @param[in] 衝突イベントを継承したクラス
*/
void PhysXGenerator::SetEventCallback(physx::PxSimulationEventCallback* callback) {
	m_Scene->setSimulationEventCallback(callback);
	//m_Physics->createHeightField();
	
}

/*
* @brief 当たり判定フィルタ
*/
PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	// all initial and persisting reports for everything, with per-point data
	pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
		| PxPairFlag::eNOTIFY_TOUCH_FOUND
		| PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| PxPairFlag::eNOTIFY_CONTACT_POINTS;
	return PxFilterFlag::eDEFAULT;
}
