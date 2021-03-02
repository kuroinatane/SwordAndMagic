/**
 * @file RigidStatic.h
 * @author MIURA DAIKI
 * @date 2020/02/18
 */
#pragma once

#include "main.h"
#include "Component.h"
#include "PhyXGenerator.h"

class C_GameObject;
/**
 * @class RigidStaticComponent
 * @brief �ÓI���̒�`�p�N���X�B
 */
class RigidStaticComponent : public C_Component {
private:
	C_GameObject*					m_pObj;					// �I�u�W�F�N�g�ւ̃|�C���^
	physx::PxRigidStatic*			m_pRigidStatic;					// �I�u�W�F�N�g�ւ̃|�C���^
public:
	RigidStaticComponent(C_GameObject* obj, DirectX::XMFLOAT3 material, const physx::PxGeometry& geometory);
	~RigidStaticComponent();
	void Update(void);
	void Draw();
};
