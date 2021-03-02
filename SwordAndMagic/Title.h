/**
 * @file TitleComponent.h
 * @author MIURA DAIKI
 * @date 2020/12/15
 */
#pragma once
#include "Component.h"

class C_GameObject;
/**
 * @class RotationComponent
 * @brief ‚½‚¾“k‚ÉY²‰ñ“]‚·‚é‚¾‚¯‚Ì‚½‚ß‚ÌƒNƒ‰ƒXB
 */
class TitleComponent : public C_Component
{
private:
	C_GameObject* m_pObj;
	float fCnt = -100.0f;
	bool m_isStart = false;
	bool m_isEnd = false;
	bool m_isActive = true;
	int m_LightNo;
public:
	TitleComponent(C_GameObject* obj);
	~TitleComponent();
	void Update();
};

