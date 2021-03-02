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
  * @brief ‚½‚¾“k‚ÉY²‰ñ“]‚·‚é‚¾‚¯‚Ì‚½‚ß‚ÌƒNƒ‰ƒXB
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

