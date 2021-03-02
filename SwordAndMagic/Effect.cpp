#include "Effect.h"
#include "GameObj.h"

EffectComponent::EffectComponent(const EFK_CHAR* path,C_GameObject* obj,float loopTime) {
	if (loopTime < 0.0f) {
		//ループが負の値ならOneShot
		m_loopTime = -loopTime;
		m_bLoop = false;
	}
	else {
		m_loopTime = loopTime;
	}
	
	m_Effect = EffectGenerator::GetInstance().CreateEffect(path);
	m_DrawFlg = false;
	m_pObj = obj;
}
EffectComponent::~EffectComponent() {

}
void EffectComponent::Update() {
	timeCount += 1.0f / 100.0f;
	if (m_DrawFlg) {
		if (m_loopTime < timeCount) {
			if (m_bLoop) {
				timeCount = 0.0f;
				m_DrawFlg = false;
				EffectGenerator::GetInstance().Stop(m_Handle);
			}
			else {
				//エフェクトのポインタはGenerator内のManagerが管理
				m_pObj->Destroy();
			}
			
		}
	}
	else {
		m_DrawFlg = true;
		DirectX::XMFLOAT3 pos = m_pObj->transform.getPosition();
		DirectX::XMFLOAT3 rot = m_pObj->transform.getRotation();
		DirectX::XMFLOAT3 scl = m_pObj->transform.getScale();
		m_Handle = EffectGenerator::GetInstance().Play(m_Effect,pos,rot,scl);
	}
	
}
void EffectComponent::Draw() {

}