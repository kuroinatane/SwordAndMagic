#include "Title.h"
#include "GameObj.h"
#include "input.h"
#include "RigidDynamic.h"
#include "ShaderLight.h"
#include "SceneController.h"
#include "Effect.h"

TitleComponent::TitleComponent(C_GameObject* obj) {
	fCnt = -1.0f;

	m_pObj = obj;

	LightInfo light;
	light.pos[0] = 0.0f;
	light.pos[1] = 0.0f;
	light.pos[2] = -300.0f;
	light.direction[0] = 0.0f;
	light.direction[1] = 0.0f;
	light.direction[2] = 1.0f;
	light.type = 0;
	light.power = 0;
	light.value[0] = 2.0f;
	light.value[1] = 4.0f;
	light.value[2] = 30.0f;
	light.value[3] = 60.0f;

	m_LightNo = ShaderLightBuffer::GetInstance().Register(light);
}
TitleComponent::~TitleComponent() {

}
void TitleComponent::Update() {

	if (m_isEnd) return;
	fCnt += 1.0f / 100.0f;
	ShaderLightBuffer::GetInstance().GetLight(m_LightNo).power = (fCnt + 1.0f) * 100000;
	if (fCnt >= 1.0f) {
		fCnt = 1.0f;
		m_isEnd = true;
	}
}


void TitleComponent::OnCollisionEnter(C_GameObject* col) {

}
void TitleComponent::OnCollisionStay(C_GameObject* col) {
}
void TitleComponent::OnCollisionExit(C_GameObject* col) {
	
}