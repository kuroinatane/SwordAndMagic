#include "PhyXGenerator.h"

using namespace physx;

/*
* @brief �R���X�g���N�^�B
*/
PhyXGenerator::PhyXGenerator() {
	Init();
}

/*
* @brief �f�X�g���N�^�B
*/
PhyXGenerator::~PhyXGenerator() {

}

/*
* @brief �������B�e��ݒ荀�ڂ�ݒ�B�ݒ肵�������ڂ��o�Ă�������������đΉ����Ă����\��
*/
void PhyXGenerator::Init() {
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	// PVD(PhysX Visual Debugger) setting. To use PVD, we need to build the project as Debug mode.
	m_Pvd = PxCreatePvd(*m_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_Pvd);
}

/*
* @brief �X�V�����B�X�V���x�͌��݂̃V�[����fps�Ɉˑ�����B
*/
void PhyXGenerator::Update() {
	m_Scene->simulate(1.0f / m_FPS);
	m_Scene->fetchResults(true);
}

/*
* @brief PhyX�p�V�[�������B��{�I�ɃQ�[���V�[�����R�t����`�Ő�������B
* @param[in] fps ���̃V�[���̕������Z�p��fps���w��B���̒l��ݒ肷���60fps�B
*/
void PhyXGenerator::CreatePhyXScene(float fps) {
	// Scene setting
	if (m_Scene) {
		m_Scene->release();
	}

	PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);          // �E����W�n�AY+�����Ȃ̂�Y-�ɏd��
	m_Dispatcher = PxDefaultCpuDispatcherCreate(1);         // The number of worker threads is one.
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_Scene = m_Physics->createScene(sceneDesc); //�g���Ă���V�[��������ꍇ�͂����Ŕj�������
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
* @brief PhyX�p�V�[���擾�B
* @return �V�[�����
*/
PxScene* PhyXGenerator::GetCurrentPhyXScene() {
	return m_Scene;
}

/*
* @brief ���݂̃V�[����ɓ��I���̃I�u�W�F�N�g�𐶐�����B�����f�[�^�̓R���|�[�l���g���Ŏ󂯎���ė��p����K�v������B
* @param[in] t �I�u�W�F�N�g�̈ʒu���B�ʒu�Ɖ�]���w��ł���B
* @param[in] material x=�Ö��C y=�����C z=���˕Ԃ�W��
* @param[in] geometry �I�u�W�F�N�g�̌`����BPx****Geometry�Ő����B
*/
PxRigidDynamic* PhyXGenerator::AddRigidDynamic(const PxTransform& t, DirectX::XMFLOAT3 material, const PxGeometry& geometry) {
	PxMaterial* pMaterial = m_Physics->createMaterial(material.x, material.y, material.z);    //static 
	PxRigidDynamic* dynamic = physx::PxCreateDynamic(*m_Physics, t, geometry, *pMaterial, 100.0f);
	m_Scene->addActor(*dynamic);
	return dynamic;
}

/*
* @brief ���݂̃V�[����ɐÓI���̃I�u�W�F�N�g�𐶐�����B�����f�[�^�̓R���|�[�l���g���Ŏ󂯎���ė��p����K�v������B
* @param[in] t �I�u�W�F�N�g�̈ʒu���B�ʒu�Ɖ�]���w��ł���B
* @param[in] material x=�Ö��C y=�����C z=���˕Ԃ�W��
* @param[in] geometry �I�u�W�F�N�g�̌`����BPx****Geometry�Ő����B
*/
PxRigidStatic* PhyXGenerator::AddRigidStatic(const PxTransform& t, DirectX::XMFLOAT3 material, const PxGeometry& geometry) {
	PxMaterial* pMaterial = m_Physics->createMaterial(material.x, material.y, material.z);    //static 
	PxRigidStatic* rs = physx::PxCreateStatic(*m_Physics, t, geometry, *pMaterial);
	m_Scene->addActor(*rs);
	return rs;
}

/*
* @brief �Փ˃R�[���o�b�N�C�x���g�̐ݒ�
* @param[in] �Փ˃C�x���g���p�������N���X
*/
void PhyXGenerator::SetEventCallback(physx::PxSimulationEventCallback* callback) {
	m_Scene->setSimulationEventCallback(callback);
}

