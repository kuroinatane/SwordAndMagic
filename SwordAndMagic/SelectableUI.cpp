#include "SelectableUI.h"
#include "GameObj.h"
#include "input.h"

/**
 * @brief �R���X�g���N�^
 */
SelectableUIComponent::SelectableUIComponent(C_GameObject* obj,bool defaultState) {
	isActive = defaultState;
}

/**
 * @brief �f�X�g���N�^
 */
SelectableUIComponent::~SelectableUIComponent() {

}

/**
 * @brief �X�V�����B��]����UI�Ƃ�����肽���ꍇ�ɁB
 */
void SelectableUIComponent::Update() {
	if (!isActive) return;
	if(InputManager::GetInstance().GetKeyTrigger(VK_UP)) {
		ToUp();
	}
	if (InputManager::GetInstance().GetKeyTrigger(VK_DOWN)) {
		ToDown();
	}
	if (InputManager::GetInstance().GetKeyTrigger(VK_RIGHT)) {
		ToRight();
	}
	if (InputManager::GetInstance().GetKeyTrigger(VK_LEFT)) {
		ToLeft();
	}
	if (InputManager::GetInstance().GetKeyTrigger(VK_RETURN)) {
		OnPlay();
	}

}

/**
 * @brief �I�����Ɍ���{�^���������ꂽ�ꍇ�̏����B
 */
void SelectableUIComponent::OnPlay() {

}

/**
 * @brief �{�^������I�����I���ɕς�������ɌĂ΂�鏈��
 */
void SelectableUIComponent::OnActive() {

}

/**
 * @brief �{�^�����I������I���ɕς�������ɌĂ΂�鏈��
 */
void SelectableUIComponent::OnInactive() {

}

/**
 * @brief ������ւ̑J�ڐ�o�^
 * @param[in] pUp ��{�^���������ꂽ���̑J�ڐ�
 */
void SelectableUIComponent::SetUpUI(SelectableUIComponent* pUp) {
	m_UpUI = pUp;
}

/**
 * @brief �������ւ̑J�ڐ�o�^
 * @param[in] pDown ���{�^���������ꂽ���̑J�ڐ�
 */
void SelectableUIComponent::SetDownUI(SelectableUIComponent* pDown) {
	m_DownUI = pDown;
}

/**
 * @brief �E�����ւ̑J�ڐ�o�^
 * @param[in] pRight �E�{�^���������ꂽ���̑J�ڐ�
 */
void SelectableUIComponent::SetRightUI(SelectableUIComponent* pRight) {
	m_RightUI = pRight;
}

/**
 * @brief �������ւ̑J�ڐ�o�^
 * @param[in] pLeft ���{�^���������ꂽ���̑J�ڐ�
 */
void SelectableUIComponent::SetLeftUI(SelectableUIComponent* pLeft) {
	m_LeftUI = pLeft;
}

/**
 * @brief �A�N�e�B�u��Ԃ̕ύX�B��Ԃɂ����OnActive�n���Ă΂��
 * @param[in] active �A�N�e�B�u���
 */
void SelectableUIComponent::SetActive(bool active) {
	bool act = isActive;
	isActive = active;
	if (isActive == act) return;

	if (isActive) OnActive();
	else OnInactive();
}

/**
 * @brief ������J��
 */
void SelectableUIComponent::ToUp() {
	if (!isActive) return;
	if (m_UpUI) {
		SetActive(false);
		m_UpUI->SetActive(true);
	}
}

/**
 * @brief�@�������J��
 */
void SelectableUIComponent::ToDown() {
	if (!isActive) return;
	if (m_DownUI) {
		SetActive(false);
		m_DownUI->SetActive(true);
	}
}

/**
 * @brief �E�����J��
 */
void SelectableUIComponent::ToRight() {
	if (!isActive) return;
	if (m_RightUI) {
		SetActive(false);
		m_DownUI->SetActive(true);
	}
}

/**
 * @brief �������J��
 */
void SelectableUIComponent::ToLeft() {
	if (!isActive) return;
	if (m_RightUI) {
		SetActive(false);
		m_RightUI->SetActive(true);
	}
}