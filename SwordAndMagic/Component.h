/**
 * @file Component.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "main.h"

class C_GameObject;

/**
 * @class C_Component
 * @brief �R���|�[�l���g�̃x�[�X�N���X�B�p���O��Ȃ̂Ŋ�{�I�ɒ��g�͉��������B
		�@�S�Ă̊֐��͏������z�֐��ł͂Ȃ�������ƂȂ��Ă���̂ŁA�I�[�o�[���C�h�͎��R�B
 */
class C_Component {
protected:
public:
	C_Component() { return; }
	virtual ~C_Component() { return;  }
	virtual void Start(void) { return; }
	virtual void Update(void) { return; }
	virtual void Draw(void) { return; }
	virtual void OnDestroy(void) { return; }
	virtual void OnCollision(C_GameObject* col) { return; }
	virtual void OnClick() { return; }
};