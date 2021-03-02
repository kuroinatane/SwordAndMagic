#include "StepObject.h"

/**
 * @brief �R���X�g���N�^�B�X�e�b�v�����������܂��B
 */
C_StepObject::C_StepObject(STEPNUM maxStep) {
	m_Step = 0;
	m_StepFirst = true;
	m_MaxStep = maxStep;
}

/**
 * @brief �f�X�g���N�^�B
 */
C_StepObject::~C_StepObject() {

}

/**
 * @brief ���̃X�e�b�v�����񂩂ǂ����𔻒�B���̊֐�����x�Ă񂾎��_�ł��̃X�e�b�v�ł̏�����s�t���O��������B
 * @return ���̃X�e�b�v��������s����true/�����łȂ����false
 */
bool C_StepObject::CheckStepFirst() {
	bool step = m_StepFirst;
	m_StepFirst = false;
	return step;
}

/**
 * @brief ���̃X�e�b�v�����񂩂ǂ����𔻒�B���̊֐��͌Ă�ł�������s�t���O�������Ȃ��B
 * @return ���̃X�e�b�v��������s����true/�����łȂ����false
 */
bool C_StepObject::GetStepFirst() {
	return m_StepFirst;
}

/**
 * @brief ���̃X�e�b�v�ԍ���Ԃ��B
 * @return ���̃X�e�b�v�ԍ�
 */
STEPNUM C_StepObject::GetStep() {
	return m_Step;
}

/**
 * @brief �X�e�b�v���w�肵���X�e�b�v�ɃW�����v���܂��B
 * @param[in] step �W�����v������X�e�b�v�ԍ�
 */
void C_StepObject::SetStep(STEPNUM step) {
	if (step < m_MaxStep) {
		m_Step = step;
		m_StepFirst = true;
	}
}

/**
 * @brief �X�e�b�v�����̃X�e�b�v
 * @param[in] step �W�����v������X�e�b�v�ԍ�
 */
void C_StepObject::SetStepNext() {
	if (m_Step < m_MaxStep - 1) {
		m_Step++;
		m_StepFirst = true;
	}
}
