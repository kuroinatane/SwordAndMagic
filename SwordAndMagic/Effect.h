/**
 * @file EffectComponent.h
 * @author MIURA DAIKI
 * @date 2020/02/18
 */
#pragma once

#include "main.h"
#include "Component.h"
#include "EffectGenerator.h"

class C_GameObject;
/**
 * @class EffectComponent
 * @brief �G�t�F�N�g��`�p�N���X�B
 */
class EffectComponent : public C_Component {
private:
	C_GameObject*					m_pObj;					// �I�u�W�F�N�g�ւ̃|�C���^
	Effekseer::Handle m_Handle = 0;
	Effekseer::Effect* m_Effect;
	float m_loopTime = 0.0f;
	float timeCount = 0.0f;
	bool m_DrawFlg = false;
	bool m_bLoop = true;
public:
	EffectComponent(const EFK_CHAR* path, C_GameObject* obj, float loopTime);
	~EffectComponent();
	void Update(void);
	void Draw();
};
