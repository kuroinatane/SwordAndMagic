#include "Animator.h"
#include "ModelBank.h"

/**
 * @brief コンストラクタ
 */
C_Animator::C_Animator(LPCSTR currentAnim) {
	m_currentAnimation = currentAnim;
	if (m_AnimSpeedList.find(m_currentAnimation) == m_AnimSpeedList.end()) {
		m_AnimSpeedList[m_currentAnimation] = 1;
	}
	m_currentAnimSpeed = m_AnimSpeedList[m_currentAnimation];
}

/**
 * @brief デストラクタ
 */
C_Animator::~C_Animator() {
	m_transistList.clear();
}
/**
 * @brief bool値を取得
 * @param[in] name 変数名
 * @return 変数の値
 */
bool C_Animator::GetBool(std::string name) {
	return m_bValueList[name];
}
/**
 * @brief int値を取得
 * @param[in] name 変数名
 * @return 変数の値
 */
int C_Animator::GetInt(std::string name) {
	return m_nValueList[name];
}
/**
 * @brief float値を取得
 * @param[in] name 変数名
 * @return 変数の値
 */
float C_Animator::GetFloat(std::string name) {
	return m_fValueList[name];
}
/**
 * @brief bool値を設定
 * @param[in] name 変数名
 * @param[in] value 数値
 */
void C_Animator::SetBool(std::string name, bool value) {
	m_bValueList[name] = value;
}
/**
 * @brief int値を設定
 * @param[in] name 変数名
 * @param[in] value 数値
 */
void C_Animator::SetInt(std::string name, int value) {
	m_nValueList[name] = value;
}
/**
 * @brief float値を設定
 * @param[in] name 変数名
 * @param[in] value 数値
 */
void C_Animator::SetFloat(std::string name, float value) {
	m_fValueList[name] = value;
}


/**
 * @brief 更新関数。アニメーション遷移チェック。
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
 * @brief 現在のアニメーション名を取得
 * @return 現在のアニメーション名
 */
LPCSTR C_Animator::GetCurrentAnimation() {
	return m_currentAnimation;
}

/**
 * @brief 現在のアニメーションフレームを取得
 * @return 現在のアニメーションフレーム
 */
int C_Animator::GetCurrentAnimFrame() {
	return m_currentAnimFrame;
}

/**
 * @brief 現在のアニメーションフレームを取得
 * @param[in] speed 変更する速度
 */
void C_Animator::SetCurrentAnimSpeed(int speed) {
	m_currentAnimSpeed = speed;
}

/**
 * @brief 遷移先の追加
 * @param[in] modelName 遷移元モデル名
 * @param[in] transister 遷移方法
 */
void C_Animator::AddTransister(LPCSTR modelName, std::unique_ptr<C_Transistor> transister) {
	m_transistList[modelName].push_back(std::move(transister));
}

/**
 * @brief アニメーション速度設定
 * @param[in] modelName 設定モデル名
 * @param[in] speed 速度
 */
void C_Animator::SetAnimSpeed(LPCSTR modelName, int speed) {
	m_AnimSpeedList[modelName] = speed;
}

bool C_Animator::GetAnimEnd() {
	return (ModelBank::GetInstance().ModelCashe(m_currentAnimation)->GetMaxAnimFrame() == m_currentAnimFrame);
}

/**
 * @brief コンストラクタ(自動遷移用、アニメーション再生終了時に自動的に遷移)
 * @param[in] nextAnimName 遷移先モデル名
 */
C_Transistor::C_Transistor(LPCSTR nextAnimName, bool doOnce) {
	isAuto = true;
	isOnce = doOnce;
}


/**
 * @brief コンストラクタ(bool値で遷移用)
 * @param[in] nextAnimName 遷移先モデル名
 * @param[in] valueName 遷移用変数名
 * @param[in] value 遷移条件値
 * @param[in] doOnce trueなら一度きりの再生(デフォルトはfalse)
 */
C_Transistor::C_Transistor(LPCSTR nextAnimName,std::string valueName, bool value, bool doOnce) {
	m_valueType = 0;
	m_valueBool = value;
	m_valueName = valueName;
	isOnce = doOnce;
}

/**
 * @brief コンストラクタ(int値で遷移用)
 * @param[in] nextAnimName 遷移先モデル名
 * @param[in] valueName 遷移用変数名
 * @param[in] value 遷移条件値
 * @param[in] doOnce trueなら一度きりの再生(デフォルトはfalse)
 */
C_Transistor::C_Transistor(LPCSTR nextAnimName,std::string valueName, int value, bool doOnce) {
	m_valueType = 1;
	m_valueInt = value;
	m_valueName = valueName;
	isOnce = doOnce;
}

/**
 * @brief コンストラクタ(float値で遷移用)
 * @param[in] nextAnimName 遷移先モデル名
 * @param[in] valueName 遷移用変数名
 * @param[in] value 遷移条件値
 * @param[in] doOnce trueなら一度きりの再生(デフォルトはfalse)
 */
C_Transistor::C_Transistor(LPCSTR nextAnimName,std::string valueName, float value, bool doOnce) {
	m_valueType = 2;
	m_valueFloat = value;
	m_valueName = valueName;
	isOnce = doOnce;
}

/**
 * @brief デストラクタ
 */
C_Transistor::~C_Transistor() {

}

/**
 * @brief 判定関数。アニメーションが遷移するかどうかを判定。
 * @param[in] animator 呼び出し元アニメーター
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
 * @brief 遷移先アニメーションを取得
 * @return 遷移先アニメーション名
 */
LPCSTR C_Transistor::getNextAnimation() {
	return m_nextAnimation;
}