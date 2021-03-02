#include "ObjectRegister.h"

/**
 * @brief �R���X�g���N�^�B�I�u�W�F�N�g�̔ԍ��Ǘ��p�z������������܂�
 */
ObjectRegister::ObjectRegister() {
	for (int i = 0; i < MAX_OBJECT; i++) {
		if (objNumberList[i]) continue;
		objNumberList[i] = false;
	}
}

/**
 * @brief �f�X�g���N�^�B������ɂ�邱�Ƃ͂���܂���B
 */
ObjectRegister::~ObjectRegister() {

}

/**
 * @brief �I�u�W�F�N�g��ԍ��œo�^���܂��B�����ԍ��̃I�u�W�F�N�g�͓����ɂ͑��݂ł��܂���B
 * @return �o�^���ꂽ�I�u�W�F�N�g�ԍ�
 */
int ObjectRegister::RegisterObject() {
	for (int i = 0; i < MAX_OBJECT; i++) {
		if (objNumberList[i]) continue;
		objNumberList[i] = true;
		return i;
	}
	return -1;//�G���[
}

/**
 * @brief �I�u�W�F�N�g�̓o�^���������܂��B�I�u�W�F�N�g���폜���ꂽ���ɁB
 * @param[in] �o�^��������I�u�W�F�N�g�ԍ�
 */
void ObjectRegister::UnregisterObject(int objNo) {
	if (objNo < 0 || objNo >= MAX_OBJECT) return;
	objNumberList[objNo] = false;
}