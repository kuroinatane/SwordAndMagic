#include "StepObject.h"

/**
 * @brief コンストラクタ。ステップを初期化します。
 */
C_StepObject::C_StepObject(STEPNUM maxStep) {
	m_Step = 0;
	m_StepFirst = true;
	m_MaxStep = maxStep;
}

/**
 * @brief デストラクタ。
 */
C_StepObject::~C_StepObject() {

}

/**
 * @brief 今のステップが初回かどうかを判定。この関数を一度呼んだ時点でそのステップでの初回実行フラグが落ちる。
 * @return 今のステップが初回実行時はtrue/そうでなければfalse
 */
bool C_StepObject::CheckStepFirst() {
	bool step = m_StepFirst;
	m_StepFirst = false;
	return step;
}

/**
 * @brief 今のステップが初回かどうかを判定。この関数は呼んでも初回実行フラグが落ちない。
 * @return 今のステップが初回実行時はtrue/そうでなければfalse
 */
bool C_StepObject::GetStepFirst() {
	return m_StepFirst;
}

/**
 * @brief 今のステップ番号を返す。
 * @return 今のステップ番号
 */
STEPNUM C_StepObject::GetStep() {
	return m_Step;
}

/**
 * @brief ステップを指定したステップにジャンプします。
 * @param[in] step ジャンプさせるステップ番号
 */
void C_StepObject::SetStep(STEPNUM step) {
	if (step < m_MaxStep) {
		m_Step = step;
		m_StepFirst = true;
	}
}

/**
 * @brief ステップを次のステップ
 * @param[in] step ジャンプさせるステップ番号
 */
void C_StepObject::SetStepNext() {
	if (m_Step < m_MaxStep - 1) {
		m_Step++;
		m_StepFirst = true;
	}
}
