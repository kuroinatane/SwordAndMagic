#include "Particle.h"
#include "GameObj.h"

ParticleComponent::ParticleComponent(C_GameObject* obj) {
	m_pObj = obj;
}

ParticleComponent::~ParticleComponent() {

}
void ParticleComponent::Update() {
	m_nLife--;
	m_pObj->transform.move(moveSpeed);
	m_pObj->transform.Rotate(rotateSpeed);
	if (m_nLife < 0) {
		m_pObj->Destroy();
	}
}

void ParticleComponent::SetLife(int life) {
	m_nLife = life;
}
void ParticleComponent::SetSpeed(DirectX::XMFLOAT3 speed) {
	moveSpeed = speed;
}
void ParticleComponent::SetRotate(DirectX::XMFLOAT3 rot) {
	rotateSpeed = rot;
}