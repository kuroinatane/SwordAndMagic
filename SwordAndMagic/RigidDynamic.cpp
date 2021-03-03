#include "RigidDynamic.h"
#include "GameObj.h"
#include "SceneController.h"
#include "Effect.h"

using namespace physx;

RigidDynamicComponent::RigidDynamicComponent(C_GameObject* obj, DirectX::XMFLOAT3 material, const physx::PxGeometry& geometory, int density ,bool isKinematic) {
	m_pObj = obj;
	DirectX::XMFLOAT4X4 xmMat = obj->transform.getWorldMatrix();
	PxMat44 mat = PxMat44(
	PxVec4(xmMat._11, xmMat._12, xmMat._13, xmMat._14),
	PxVec4(xmMat._21, xmMat._22, xmMat._23, xmMat._24),
	PxVec4(xmMat._31, xmMat._32, xmMat._33, xmMat._34),
	PxVec4(xmMat._41, xmMat._42, xmMat._43, xmMat._44)
	);
	PxTransform ptx = PxTransform(mat);
	m_pShape = PhysXGenerator::GetInstance().CreatePhysXShape(material,geometory);
	m_pRigidDynamic = PhysXGenerator::GetInstance().AddRigidDynamic(ptx, m_pShape, density);
	m_pRigidDynamic->setMass(100);
	SetKinematic(isKinematic);
	m_pRigidDynamic->setMaxAngularVelocity(100000.0f);
	m_pRigidDynamic->setMaxLinearVelocity(100000.0f);
	obj->SetIsRigidbody(true);
}

RigidDynamicComponent:: ~RigidDynamicComponent() {
	if (m_pShape) {
		m_pShape->release();
	}
}

void RigidDynamicComponent::Update(void) {
	if (m_isKinematic) return;
	PxTransform trans = m_pRigidDynamic->getGlobalPose();
	m_pObj->transform.setPosition(DirectX::XMFLOAT3(trans.p.x, trans.p.y, trans.p.z));
	DirectX::XMFLOAT4 qt = DirectX::XMFLOAT4(trans.q.x, trans.q.y, trans.q.z, trans.q.w);
	m_pObj->transform.setQuaternion(DirectX::XMLoadFloat4(&qt));
}

void RigidDynamicComponent::Draw() {

}

void RigidDynamicComponent::SetPosition(DirectX::XMFLOAT3 setPosition) {
	
	PxTransform trans = m_pRigidDynamic->getGlobalPose();
	trans.p.x = setPosition.x;
	trans.p.y = setPosition.y;
	trans.p.z = setPosition.z;
	if (m_isKinematic) {
		m_pRigidDynamic->setKinematicTarget(trans);
	}
	else {
		m_pRigidDynamic->setGlobalPose(trans);
	}
	m_pObj->transform.setPosition(setPosition);
}
void RigidDynamicComponent::SetRotation(DirectX::XMFLOAT3 setRotation) {
	DirectX::XMVECTOR vec = DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&setRotation));
	DirectX::XMFLOAT4 qt;
	DirectX::XMStoreFloat4(&qt,vec);
	PxTransform trans = m_pRigidDynamic->getGlobalPose();
	trans.q.x = qt.x;
	trans.q.y = qt.y;
	trans.q.z = qt.z;
	trans.q.w = qt.z;
	if (m_isKinematic) {
		m_pRigidDynamic->setKinematicTarget(trans);
	}
	else {
		m_pRigidDynamic->setGlobalPose(trans);
	}
	m_pObj->transform.setRotation(setRotation);
}

void RigidDynamicComponent::Move(DirectX::XMFLOAT3 move) {
	PxTransform trans = m_pRigidDynamic->getGlobalPose();
	trans.p.x += move.x;
	trans.p.y += move.y;
	trans.p.z += move.z;
	if (m_isKinematic) {
		m_pRigidDynamic->setKinematicTarget(trans);
	}
	else {
		m_pRigidDynamic->setGlobalPose(trans);
	}
	m_pObj->transform.move(move);
}

void RigidDynamicComponent::Rotate(DirectX::XMFLOAT3 rot) {
	PxTransform trans = m_pRigidDynamic->getGlobalPose();
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

	if (m_isKinematic) {
		m_pRigidDynamic->setKinematicTarget(trans);
	}
	else {
		m_pRigidDynamic->setGlobalPose(trans);
	}
}

void RigidDynamicComponent::SetKinematic(bool isKinematic) {
	m_isKinematic = isKinematic;
	m_pRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
	if (!isKinematic) {
		m_pRigidDynamic->wakeUp();
	}
}

void RigidDynamicComponent::AddForce(DirectX::XMFLOAT3 force) {
	m_pRigidDynamic->addForce(PxVec3(force.x, force.y, force.z));
}
void RigidDynamicComponent::AddTorque(DirectX::XMFLOAT3 torque) {
	m_pRigidDynamic->addTorque(PxVec3(torque.x, torque.y, torque.z));
}


void RigidDynamicComponent::SetLock(PxRigidDynamicLockFlag::Enum flag, bool status) {
	m_pRigidDynamic->setRigidDynamicLockFlag(flag,status);
}

physx::PxRigidActor* RigidDynamicComponent::GetActor() {
	return m_pRigidDynamic;

}

void RigidDynamicComponent::SetGeometry(const PxGeometry& geometory){
	m_pRigidDynamic->detachShape(*m_pShape);
	m_pShape->setGeometry(geometory);
	m_pRigidDynamic->attachShape(*m_pShape);
	switch (m_pShape->getGeometryType()) {
	case PxGeometryType::eBOX:
		break;
	case PxGeometryType::eCAPSULE:
		break;
	case PxGeometryType::eSPHERE:
		break;
	case PxGeometryType::ePLANE:
		break;
	}
}