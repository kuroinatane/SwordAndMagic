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
 * @brief 静的剛体定義用クラス。
 */
class RigidStaticComponent : public C_Component {
private:
	C_GameObject*					m_pObj;					// オブジェクトへのポインタ
	physx::PxRigidStatic*			m_pRigidStatic;					// オブジェクトへのポインタ
public:
	RigidStaticComponent(C_GameObject* obj, DirectX::XMFLOAT3 material, const physx::PxGeometry& geometory);
	~RigidStaticComponent();
	void Update(void);
	void Draw();
};
