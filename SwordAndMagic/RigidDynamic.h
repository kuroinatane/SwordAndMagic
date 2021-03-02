/**
 * @file RigidDynamic.h
 * @author MIURA DAIKI
 * @date 2020/02/18
 */
#pragma once

#include "main.h"
#include "Component.h"
#include "PhyXGenerator.h"
#include <functional>

class C_GameObject;
/**
 * @class RigidDynamicComponent
 * @brief 動的剛体用コンポーネント。
 */
class RigidDynamicComponent : public C_Component, public physx::PxSimulationEventCallback {
private:
	C_GameObject*					m_pObj;					// オブジェクトへのポインタ
	physx::PxRigidDynamic*			m_pRigidDynamic;					// オブジェクトへのポインタ
	bool m_isKinematic;	// 物理シミュレーション有効無効
	bool m_Hit = false;;
public:
	RigidDynamicComponent(C_GameObject* obj, DirectX::XMFLOAT3 material, const physx::PxGeometry& geometory, bool isKinematic = false);
	~RigidDynamicComponent();
	void Update(void);
	void Draw();
	void SetPosition(DirectX::XMFLOAT3 setPosition);
	void SetRotation(DirectX::XMFLOAT3 setRotation);
	void Move(DirectX::XMFLOAT3 move);
	void Rotate(DirectX::XMFLOAT3 rot);
	void AddForce(DirectX::XMFLOAT3 force);
	void AddTorque(DirectX::XMFLOAT3 torque);
	void SetKinematic(bool isKinematic);
	void SetLock(physx::PxRigidDynamicLockFlag::Enum flag, bool status);

	virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
	virtual void							onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {}
	virtual void							onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) {}
	virtual void							onWake(physx::PxActor**, physx::PxU32) {}
	virtual void							onSleep(physx::PxActor**, physx::PxU32) {}
	virtual void							onAdvance(const physx::PxRigidBody*const*, const physx::PxTransform*, const physx::PxU32) {}
};
