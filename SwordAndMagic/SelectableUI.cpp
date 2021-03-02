#include "SelectableUI.h"
#include "GameObj.h"
#include "input.h"

/**
 * @brief コンストラクタ
 */
SelectableUIComponent::SelectableUIComponent(C_GameObject* obj,bool defaultState) {
	isActive = defaultState;
}

/**
 * @brief デストラクタ
 */
SelectableUIComponent::~SelectableUIComponent() {

}

/**
 * @brief 更新処理。回転するUIとかを作りたい場合に。
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
 * @brief 選択時に決定ボタンが押された場合の処理。
 */
void SelectableUIComponent::OnPlay() {

}

/**
 * @brief ボタンが非選択→選択に変わった時に呼ばれる処理
 */
void SelectableUIComponent::OnActive() {

}

/**
 * @brief ボタンが選択→非選択に変わった時に呼ばれる処理
 */
void SelectableUIComponent::OnInactive() {

}

/**
 * @brief 上方向への遷移先登録
 * @param[in] pUp 上ボタンを押された時の遷移先
 */
void SelectableUIComponent::SetUpUI(SelectableUIComponent* pUp) {
	m_UpUI = pUp;
}

/**
 * @brief 下方向への遷移先登録
 * @param[in] pDown 下ボタンを押された時の遷移先
 */
void SelectableUIComponent::SetDownUI(SelectableUIComponent* pDown) {
	m_DownUI = pDown;
}

/**
 * @brief 右方向への遷移先登録
 * @param[in] pRight 右ボタンを押された時の遷移先
 */
void SelectableUIComponent::SetRightUI(SelectableUIComponent* pRight) {
	m_RightUI = pRight;
}

/**
 * @brief 左方向への遷移先登録
 * @param[in] pLeft 左ボタンを押された時の遷移先
 */
void SelectableUIComponent::SetLeftUI(SelectableUIComponent* pLeft) {
	m_LeftUI = pLeft;
}

/**
 * @brief アクティブ状態の変更。状態によってOnActive系が呼ばれる
 * @param[in] active アクティブ状態
 */
void SelectableUIComponent::SetActive(bool active) {
	bool act = isActive;
	isActive = active;
	if (isActive == act) return;

	if (isActive) OnActive();
	else OnInactive();
}

/**
 * @brief 上方向遷移
 */
void SelectableUIComponent::ToUp() {
	if (!isActive) return;
	if (m_UpUI) {
		SetActive(false);
		m_UpUI->SetActive(true);
	}
}

/**
 * @brief　下方向遷移
 */
void SelectableUIComponent::ToDown() {
	if (!isActive) return;
	if (m_DownUI) {
		SetActive(false);
		m_DownUI->SetActive(true);
	}
}

/**
 * @brief 右方向遷移
 */
void SelectableUIComponent::ToRight() {
	if (!isActive) return;
	if (m_RightUI) {
		SetActive(false);
		m_DownUI->SetActive(true);
	}
}

/**
 * @brief 左方向遷移
 */
void SelectableUIComponent::ToLeft() {
	if (!isActive) return;
	if (m_RightUI) {
		SetActive(false);
		m_RightUI->SetActive(true);
	}
}