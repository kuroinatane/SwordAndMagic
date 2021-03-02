#include "Animator.h"
#include "ModelBank.h"

/**
 * @brief �R���X�g���N�^
 */
C_Animator::C_Animator(LPCSTR currentAnim) {
	m_currentAnimation = currentAnim;
	if (m_AnimSpeedList.find(m_currentAnimation) == m_AnimSpeedList.end()) {
		m_AnimSpeedList[m_currentAnimation] = 1;
	}
	m_currentAnimSpeed = m_AnimSpeedList[m_currentAnimation];
}

/**
 * @brief �f�X�g���N�^
 */
C_Animator::~C_Animator() {
	m_transistList.clear();
}
/**
 * @brief bool�l���擾
 * @param[in] name �ϐ���
 * @return �ϐ��̒l
 */
bool C_Animator::GetBool(std::string name) {
	return m_bValueList[name];
}
/**
 * @brief int�l���擾
 * @param[in] name �ϐ���
 * @return �ϐ��̒l
 */
int C_Animator::GetInt(std::string name) {
	return m_nValueList[name];
}
/**
 * @brief float�l���擾
 * @param[in] name �ϐ���
 * @return �ϐ��̒l
 */
float C_Animator::GetFloat(std::string name) {
	return m_fValueList[name];
}
/**
 * @brief bool�l��ݒ�
 * @param[in] name �ϐ���
 * @param[in] value ���l
 */
void C_Animator::SetBool(std::string name, bool value) {
	m_bValueList[name] = value;
}
/**
 * @brief int�l��ݒ�
 * @param[in] name �ϐ���
 * @param[in] value ���l
 */
void C_Animator::SetInt(std::string name, int value) {
	m_nValueList[name] = value;
}
/**
 * @brief float�l��ݒ�
 * @param[in] name �ϐ���
 * @param[in] value ���l
 */
void C_Animator::SetFloat(std::string name, float value) {
	m_fValueList[name] = value;
}


/**
 * @brief �X�V�֐��B�A�j���[�V�����J�ڃ`�F�b�N�B
 */
void C_Animator::Update() {
	std::vector<std::unique_ptr<C_Transistor>>::iterator itr;
	for (itr = m_transistList[m_currentAnimation].begin(); itr != m_transistList[m_currentAnimation].end(); itr++) {
		if ((*itr)->Judge(this)) {
			m_currentAnimation = (*itr)->getNextAnimation();
			if (m_AnimSpeedList.find(m_currentAnimation) == m_AnimSpeedList.end()) {
				m_AnimSpeedList[m_currentAnimation] = 1;
			}
			m_currentAnimFrame = 0;
			m_currentAnimSpeed = m_AnimSpeedList[m_currentAnimation];
			return;
		}
	}
	m_currentAnimFrame += m_currentAnimSpeed;
	if (ModelBank::GetInstance().ModelCashe(m_currentAnimation)->GetMaxAnimFrame() < m_currentAnimFrame) {
		m_currentAnimFrame = 0;
	}

}

/**
 * @brief ���݂̃A�j���[�V���������擾
 * @return ���݂̃A�j���[�V������
 */
LPCSTR C_Animator::GetCurrentAnimation() {
	return m_currentAnimation;
}

/**
 * @brief ���݂̃A�j���[�V�����t���[�����擾
 * @return ���݂̃A�j���[�V�����t���[��
 */
int C_Animator::GetCurrentAnimFrame() {
	return m_currentAnimFrame;
}

/**
 * @brief ���݂̃A�j���[�V�����t���[�����擾
 * @param[in] speed �ύX���鑬�x
 */
void C_Animator::SetCurrentAnimSpeed(int speed) {
	m_currentAnimSpeed = speed;
}

/**
 * @brief �J�ڐ�̒ǉ�
 * @param[in] modelName �J�ڌ����f����
 * @param[in] transister �J�ڕ��@
 */
void C_Animator::AddTransister(LPCSTR modelName, std::unique_ptr<C_Transistor> transister) {
	m_transistList[modelName].push_back(std::move(transister));
}

/**
 * @brief �A�j���[�V�������x�ݒ�
 * @param[in] modelName �ݒ胂�f����
 * @param[in] speed ���x
 */
void C_Animator::SetAnimSpeed(LPCSTR modelName, int speed) {
	m_AnimSpeedList[modelName] = speed;
}

bool C_Animator::GetAnimEnd() {
	return (ModelBank::GetInstance().ModelCashe(m_currentAnimation)->GetMaxAnimFrame() == m_currentAnimFrame);
}

/**
 * @brief �R���X�g���N�^(�����J�ڗp�A�A�j���[�V�����Đ��I�����Ɏ����I�ɑJ��)
 * @param[in] nextAnimName �J�ڐ惂�f����
 */
C_Transistor::C_Transistor(LPCSTR nextAnimName, bool doOnce) {
	isAuto = true;
	isOnce = doOnce;
}


/**
 * @brief �R���X�g���N�^(bool�l�őJ�ڗp)
 * @param[in] nextAnimName �J�ڐ惂�f����
 * @param[in] valueName �J�ڗp�ϐ���
 * @param[in] value �J�ڏ����l
 * @param[in] doOnce true�Ȃ��x����̍Đ�(�f�t�H���g��false)
 */
C_Transistor::C_Transistor(LPCSTR nextAnimName,std::string valueName, bool value, bool doOnce) {
	m_valueType = 0;
	m_valueBool = value;
	m_valueName = valueName;
	isOnce = doOnce;
}

/**
 * @brief �R���X�g���N�^(int�l�őJ�ڗp)
 * @param[in] nextAnimName �J�ڐ惂�f����
 * @param[in] valueName �J�ڗp�ϐ���
 * @param[in] value �J�ڏ����l
 * @param[in] doOnce true�Ȃ��x����̍Đ�(�f�t�H���g��false)
 */
C_Transistor::C_Transistor(LPCSTR nextAnimName,std::string valueName, int value, bool doOnce) {
	m_valueType = 1;
	m_valueInt = value;
	m_valueName = valueName;
	isOnce = doOnce;
}

/**
 * @brief �R���X�g���N�^(float�l�őJ�ڗp)
 * @param[in] nextAnimName �J�ڐ惂�f����
 * @param[in] valueName �J�ڗp�ϐ���
 * @param[in] value �J�ڏ����l
 * @param[in] doOnce true�Ȃ��x����̍Đ�(�f�t�H���g��false)
 */
C_Transistor::C_Transistor(LPCSTR nextAnimName,std::string valueName, float value, bool doOnce) {
	m_valueType = 2;
	m_valueFloat = value;
	m_valueName = valueName;
	isOnce = doOnce;
}

/**
 * @brief �f�X�g���N�^
 */
C_Transistor::~C_Transistor() {

}

/**
 * @brief ����֐��B�A�j���[�V�������J�ڂ��邩�ǂ����𔻒�B
 * @param[in] animator �Ăяo�����A�j���[�^�[
 */
bool C_Transistor::Judge(C_Animator* animator) {
	if (isAuto) return animator->GetAnimEnd();
	if (isOnce) {
		animator->SetCurrentAnimSpeed(0);
		return false;
	}
	switch (m_valueType) {
	case 0:
		if (animator->GetBool(m_valueName) == m_valueBool) return true;
		else return false;
		break;
	case 1:
		if (animator->GetInt(m_valueName) == m_valueInt) return true;
		else return false;
		break;
	case 2:
		if (animator->GetFloat(m_valueName) == m_valueFloat) return true;
		else return false;
		break;
	default:
		return false;
	}
}

/**
 * @brief �J�ڐ�A�j���[�V�������擾
 * @return �J�ڐ�A�j���[�V������
 */
LPCSTR C_Transistor::getNextAnimation() {
	return m_nextAnimation;
}