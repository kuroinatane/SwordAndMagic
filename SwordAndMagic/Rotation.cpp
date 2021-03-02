#include "Rotation.h"
#include "GameObj.h"

RotationComponent::RotationComponent(C_GameObject* obj, float speed){
	m_pObj = obj;
	RotateSpeed = speed;
}
RotationComponent::~RotationComponent() {

}
void RotationComponent::Update() {
	m_pObj->transform.Rotate(DirectX::XMFLOAT3(0, RotateSpeed,0));
}
