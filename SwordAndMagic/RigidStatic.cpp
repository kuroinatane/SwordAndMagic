#include "RigidStatic.h"
#include "GameObj.h"

using namespace physx;

RigidStaticComponent::RigidStaticComponent(C_GameObject* obj, DirectX::XMFLOAT3 material, const physx::PxGeometry& geometory) {
	m_pObj = obj;
	DirectX::XMFLOAT4X4 xmMat = obj->transform.getWorldMatrix();
	PxMat44 mat = PxMat44(
		PxVec4(xmMat._11, xmMat._12, xmMat._13, xmMat._14),
		PxVec4(xmMat._21, xmMat._22, xmMat._23, xmMat._24),
		PxVec4(xmMat._31, xmMat._32, xmMat._33, xmMat._34),
		PxVec4(xmMat._41, xmMat._42, xmMat._43, xmMat._44)
	);
	PxTransform ptx = PxTransform(mat);
	m_pRigidStatic = PhyXGenerator::GetInstance().AddRigidStatic(ptx, material, geometory);
}

RigidStaticComponent:: ~RigidStaticComponent() {

}

void RigidStaticComponent::Update(void) {
	PxTransform trans = m_pRigidStatic->getGlobalPose();
	m_pObj->transform.setPosition(DirectX::XMFLOAT3(trans.p.x, trans.p.y, trans.p.z));
	DirectX::XMFLOAT4 qt = DirectX::XMFLOAT4(trans.q.x, trans.q.y, trans.q.z, trans.q.w);
	m_pObj->transform.setQuaternion(DirectX::XMLoadFloat4(&qt));
}

void RigidStaticComponent::Draw() {

}