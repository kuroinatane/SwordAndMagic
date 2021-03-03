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
 * @brief ただ徒にY軸回転するだけのためのクラス。
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
	void OnCollisionEnter(C_GameObject* col);
	void OnCollisionStay(C_GameObject* col);
	void OnCollisionExit(C_GameObject* col);
};

