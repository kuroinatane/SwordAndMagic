/**
 * @file Rotation.h
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
class RotationComponent : public C_Component
{
private:
	C_GameObject* m_pObj;
	float RotateSpeed;
public:
	RotationComponent(C_GameObject* obj, float speed);
	~RotationComponent();
	void Update();
};

