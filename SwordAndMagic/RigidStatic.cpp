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
	m_pShape = PhysXGenerator::GetInstance().CreatePhysXShape(material, geometory);
	m_pRigidStatic = PhysXGenerator::GetInstance().AddRigidStatic(ptx, m_pShape);
	obj->SetIsRigidbody(true);
}

RigidStaticComponent:: ~RigidStaticComponent() {
	if (m_pShape) {
		m_pShape->release();
	}
}

void RigidStaticComponent::Update(void) {
	PxTransform trans = m_pRigidStatic->getGlobalPose();
	m_pObj->transform.setPosition(DirectX::XMFLOAT3(trans.p.x, trans.p.y, trans.p.z));
	DirectX::XMFLOAT4 qt = DirectX::XMFLOAT4(trans.q.x, trans.q.y, trans.q.z, trans.q.w);
	m_pObj->transform.setQuaternion(DirectX::XMLoadFloat4(&qt));
}

void RigidStaticComponent::Draw() {

}

physx::PxRigidActor* RigidStaticComponent::GetActor() {
	return m_pRigidStatic;
}

void RigidStaticComponent::SetPosition(DirectX::XMFLOAT3 setPosition) {

	PxTransform trans = m_pRigidStatic->getGlobalPose();
	trans.p.x = setPosition.x;
	trans.p.y = setPosition.y;
	trans.p.z = setPosition.z;
	m_pRigidStatic->setGlobalPose(trans);
	m_pObj->transform.setPosition(setPosition);
}
void RigidStaticComponent::SetRotation(DirectX::XMFLOAT3 setRotation) {
	DirectX::XMVECTOR vec = DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&setRotation));
	DirectX::XMFLOAT4 qt;
	DirectX::XMStoreFloat4(&qt, vec);
	PxTransform trans = m_pRigidStatic->getGlobalPose();
	trans.q.x = qt.x;
	trans.q.y = qt.y;
	trans.q.z = qt.z;
	trans.q.w = qt.z;
	m_pRigidStatic->setGlobalPose(trans);
	m_pObj->transform.setRotation(setRotation);
}

void RigidStaticComponent::Move(DirectX::XMFLOAT3 move) {
	PxTransform trans = m_pRigidStatic->getGlobalPose();
	trans.p.x += move.x;
	trans.p.y += move.y;
	trans.p.z += move.z;
	m_pRigidStatic->setGlobalPose(trans);
	m_pObj->transform.move(move);
}

void RigidStaticComponent::Rotate(DirectX::XMFLOAT3 rot) {
	PxTransform trans = m_pRigidStatic->getGlobalPose();

	//‰ñ“]‚³‚¹‚Ä·•ª‚ðŽZo
	DirectX::XMFLOAT4 qt = m_pObj->transform.getQuaternion();
	m_pObj->transform.Rotate(rot);
	qt.x = m_pObj->transform.getQuaternion().x - qt.x;
	qt.y = m_pObj->transform.getQuaternion().y - qt.y;
	qt.z = m_pObj->transform.getQuaternion().z - qt.z;
	qt.w = m_pObj->transform.getQuaternion().w - qt.w;

	//·•ª‚ð‘«‚·
	trans.q.x += qt.x;
	trans.q.y += qt.y;
	trans.q.z += qt.z;
	trans.q.w += qt.w;
	m_pRigidStatic->setGlobalPose(trans);
}