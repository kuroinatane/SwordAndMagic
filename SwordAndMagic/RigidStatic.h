/**
 * @file RigidStatic.h
 * @author MIURA DAIKI
 * @date 2020/02/18
 */
#pragma once

#include "main.h"
#include "Component.h"
#include "PhysXGenerator.h"

class C_GameObject;
/**
 * @class RigidStaticComponent
 * @brief 静的剛体定義用クラス。
 */
class RigidStaticComponent : public C_Component {
private:
	C_GameObject*					m_pObj;					// オブジェクトへのポインタ
	physx::PxRigidStatic*			m_pRigidStatic;					// オブジェクトへのポインタ
	physx::PxShape*			m_pShape;					// オブジェクトへのポインタ
public:
	RigidStaticComponent(C_GameObject* obj, DirectX::XMFLOAT3 material, const physx::PxGeometry& geometory);
	~RigidStaticComponent();
	void Update(void);
	void Draw();
	physx::PxRigidActor* GetActor();
	void SetPosition(DirectX::XMFLOAT3 setPosition);
	void SetRotation(DirectX::XMFLOAT3 setRotation);
	void Move(DirectX::XMFLOAT3 move);
	void Rotate(DirectX::XMFLOAT3 rot);
};
