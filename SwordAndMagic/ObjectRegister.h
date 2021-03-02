/**
 * @file ObjectRegister.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Singleton.h"

constexpr int MAX_OBJECT = 1024; //!< @brief �����ɑ��݂ł���I�u�W�F�N�g�̍ő吔

 /**
  * @class ObjectRegister
  * @brief �I�u�W�F�N�g�o�^�p�̃V���O���g���N���X�B
  *        ��������I�u�W�F�N�g�ɔԍ���U�蕪���A�j�������I�u�W�F�N�g�̔ԍ��͕ԋp����B
  */
class ObjectRegister : public Singleton<ObjectRegister> {
private:
	bool objNumberList[MAX_OBJECT]; //!< @brief �I�u�W�F�N�g�ԍ��Ǘ��p�z��
public:
	ObjectRegister();
	~ObjectRegister();
	int RegisterObject();
	void UnregisterObject(int objNo);
};
