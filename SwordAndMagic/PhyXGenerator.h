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
 * @brief �������Z�̋��ʃf�[�^�Ǘ��N���X�B�������ƃV�[������уV�[�����̃I�u�W�F�N�g�Ǘ���S������B
 */
class PhyXGenerator : public Singleton<PhyXGenerator>
{
private:
	float m_FPS;
	physx::PxDefaultAllocator      m_Allocator; //!< @brief SDK�ɕK�v�ȃA���P�[�^�[�C���^�[�t�F�C�X�̃f�t�H���g�̎���
	physx::PxDefaultErrorCallback  m_ErrorCallback; //!< @brief �G���[�R�[���o�b�N�̃f�t�H���g�̎���
	physx::PxFoundation*          m_Foundation = nullptr; //!< @brief FoundationSDK�̃V���O���g���N���X�B��ʃ��x����SDK���C���X�^���X������ɂ́A���̃N���X�̃C���X�^���X���K�v�B
	physx::PxPhysics*            m_Physics = nullptr; //!< @brief�@�I�u�W�F�N�g���C���X�^���X�����邽�߂Ɏg�p���钊�ۃV���O���g���t�@�N�g���N���X�B
	physx::PxDefaultCpuDispatcher* m_Dispatcher = nullptr; //!< @brief�@CPU�^�X�N�f�B�X�p�b�`���[�̃f�t�H���g�̎����B
	physx::PxScene*              m_Scene = nullptr; //!< @brief PhyX�Ŏg�p�����V�[���B
	physx::PxPvd*              m_Pvd = nullptr; //!< @brief �f�o�b�O�p
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
