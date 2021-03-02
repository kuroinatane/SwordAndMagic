//=============================================================================
//
// 入力処理 [Input.cpp]
//
//=============================================================================
#include "Input.h"
#include "Window.h"
#include "WindowManager.h"

// 定数
namespace {
	constexpr int	g_nMouseBtn[] = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_XBUTTON1, VK_XBUTTON2 }; //!< @brief マウス用のボタン定義を配列化
}

/**
 * @brief キーボード、パッド、マウスを初期化
 * @return 処理結果(S_OK/エラーメッセージ)
 */
HRESULT InputManager::Init()
{
	HRESULT hr = S_OK;

	// キーボードの初期化
	hr = InitKeyboard();

	// ゲームパッドの初期化
	if (SUCCEEDED(hr)) {
		hr = InitJoystick();
	}

	// マウスの初期化
	if (SUCCEEDED(hr)) {
		hr = InitMouse();
	}

	return hr;
}

/**
 * @brief 終了処理
 */
void InputManager::Uninit()
{
	// マウスの終了処理
	UninitMouse();

	// ゲームパッドの終了処理
	UninitJoystick();

	// キーボードの終了処理
	UninitKeyboard();
}

/**
 * @brief 更新処理
 */
void InputManager::Update()
{
	// キーボードの更新
	UpdateKeyboard();

	// ゲームパッドの更新
	UpdateJoystick();

	// マウスの更新
	UpdateMouse();
}

/**
 * @brief キーボードの初期化処理
 * @return 処理結果(S_OK/エラーメッセージ)
 */
HRESULT InputManager::InitKeyboard()
{
	HRESULT hr = S_OK;

	return hr;
}

/**
 * @brief キーボードの終了処理
 */
void InputManager::UninitKeyboard()
{
	// (何もしない)
}

/**
 * @brief キーボードの更新処理
 * @return 処理結果(S_OK/エラーメッセージ)
 */
HRESULT InputManager::UpdateKeyboard()
{
	HRESULT hr = S_OK;
	WORD wKeyState;

	// キーボードの状態を取得
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++) {
		wKeyState = ::GetAsyncKeyState(nCntKey);

		m_wKeyStateTrigger[nCntKey] = (m_wKeyState[nCntKey] ^ wKeyState) & wKeyState;
		m_wKeyStateRelease[nCntKey] = (m_wKeyState[nCntKey] ^ wKeyState) & ~wKeyState;
		m_wKeyStateRepeat[nCntKey] = m_wKeyStateTrigger[nCntKey];

		if (wKeyState & 0x8000) {
			m_nKeyStateRepeatCnt[nCntKey]++;
			if (m_nKeyStateRepeatCnt[nCntKey] >= 20) {
				m_wKeyStateRepeat[nCntKey] = wKeyState;
			}
		} else {
			m_nKeyStateRepeatCnt[nCntKey] = 0;
			m_wKeyStateRepeat[nCntKey] = 0;
		}

		m_wKeyState[nCntKey] = wKeyState;
	}

	return hr;
}

/**
 * @brief ゲームパッドの初期化処理
 * @return 処理結果(S_OK/エラーメッセージ)
 */
HRESULT InputManager::InitJoystick()
{
	GetJoyCount();

	return S_OK;
}

/**
 * @brief ゲームパッドの終了処理
 */
void InputManager::UninitJoystick()
{
	// (何もしない)
}

/**
 * @brief ゲームパッドの更新処理
 * @return 処理結果(S_OK/エラーメッセージ)
 */
HRESULT InputManager::UpdateJoystick()
{
	HRESULT hr = S_OK;

	for (DWORD i = 0; i < m_dwJoyCount; ++i) {
		JOYINFOEX& ji = m_joyState[i];
		DWORD dwButtons = ji.dwButtons;
		ZeroMemory(&ji, sizeof(ji));
		ji.dwSize = sizeof(ji);
		ji.dwFlags = JOY_RETURNALL;
		if (joyGetPosEx(i, &ji) != JOYERR_NOERROR) {
			hr = E_FAIL;
			break;
		}
		m_dwJoyButtonTrigger[i] = (m_joyState[i].dwButtons ^ dwButtons) & dwButtons;
		m_dwJoyButtonRelease[i] = (m_joyState[i].dwButtons ^ dwButtons) & ~dwButtons;
	}
	return hr;
}

/**
 * @brief マウスの初期化処理
 * @return 処理結果(S_OK/エラーメッセージ)
 */
HRESULT InputManager::InitMouse()
{
	return S_OK;
}

/**
 * @brief マウスの終了処理
 */
void InputManager::UninitMouse()
{
	// (何もしない)
}

/**
 * @brief マウスの更新処理
 * @return 処理結果(S_OK/エラーメッセージ)
 */
HRESULT InputManager::UpdateMouse()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(WindowManager::GetInstance().GetMainHwnd(), &pt);
	m_ptMouse = pt;
	return S_OK;
}

/**
 * @brief キーボードの押下状態を取得
 * @param[in] nKey 取得したいキーのID
 * @return 押下されている(true)/押下されていない(false)
 */
bool InputManager::GetKeyPress(int nKey)
{
	return (m_wKeyState[nKey] & 0x8000) ? true : false;
}

/**
 * @brief キーボードのトリガ状態を取得
 * @param[in] nKey 取得したいキーのID
 * @return トリガされている(true)/トリガされていない(false)
 */
bool InputManager::GetKeyTrigger(int nKey)
{
	return (m_wKeyStateTrigger[nKey] & 0x8000) ? true : false;
}

/**
 * @brief キーボードのリピート状態を取得
 * @param[in] nKey 取得したいキーのID
 * @return リピートされている(true)/リピートされていない(false)
 */
bool InputManager::GetKeyRepeat(int nKey)
{
	return (m_wKeyStateRepeat[nKey] & 0x8000) ? true : false;
}

/**
 * @brief キーボードのリリース状態を取得
 * @param[in] nKey 取得したいキーのID
 * @return リリースされている(true)/リリースされていない(false)
 */
bool InputManager::GetKeyRelease(int nKey)
{
	return (m_wKeyStateRelease[nKey] & 0x8000) ? true : false;
}

/**
 * @brief ゲームパッド接続数取得
 * @return ゲームパッド接続数
 */
DWORD InputManager::GetJoyCount()
{
	JOYINFO	ji;
	DWORD dwCount = joyGetNumDevs();
	if (dwCount > NUM_JOY_MAX)
		dwCount = NUM_JOY_MAX;
	for (m_dwJoyCount = 0; m_dwJoyCount < dwCount; ++m_dwJoyCount) {
		if (joyGetPos(m_dwJoyCount, &ji) != JOYERR_NOERROR)
			break;
	}
	return m_dwJoyCount;
}

/**
 * @brief ゲームパッド情報取得
 * @param[in] dwJoy ゲームパッド番号
 * @return ゲームパッドの情報(無効ならnullptr)
 */
JOYINFOEX* InputManager::GetJoyState(DWORD dwJoy)
{
	if (dwJoy >= NUM_JOY_MAX) return nullptr;
	return &m_joyState[dwJoy];
}

/**
 * @brief ゲームパッドX軸取得
 * @param[in] dwJoy ゲームパッド番号
 * @return 指定したゲームパッドのX軸入力(無効なら0)
 */
LONG InputManager::GetJoyX(DWORD dwJoy)
{
	if (dwJoy >= NUM_JOY_MAX) return 0L;
	return (LONG)m_joyState[dwJoy].dwXpos - 0x08000;
}

/**
 * @brief ゲームパッドY軸取得
 * @param[in] dwJoy ゲームパッド番号
 * @return 指定したゲームパッドのY軸入力(無効なら0)
 */
LONG InputManager::GetJoyY(DWORD dwJoy)
{
	if (dwJoy >= NUM_JOY_MAX) return 0L;
	return (LONG)m_joyState[dwJoy].dwYpos - 0x08000;
}

/**
 * @brief ゲームパッドZ軸取得
 * @param[in] dwJoy ゲームパッド番号
 * @return 指定したゲームパッドのZ軸入力(無効なら0)
 */
LONG InputManager::GetJoyZ(DWORD dwJoy)
{
	if (dwJoy >= NUM_JOY_MAX) return 0L;
	return (LONG)m_joyState[dwJoy].dwZpos - 0x08000;
}

/**
 * @brief ゲームパッドボタン情報取得
 * @param[in] dwJoy ゲームパッド番号
 * @param[in] dwBtn ボタンID
 * @return 指定したゲームパッドの指定したボタンが押下されている(true)/押下されていない(false)
 */
bool InputManager::GetJoyButton(DWORD dwJoy, DWORD dwBtn)
{
	if (dwJoy >= NUM_JOY_MAX) return false;
	return (m_joyState[dwJoy].dwButtons & (1 << dwBtn)) ? true : false;
}

/**
 * @brief ゲームパッドボタントリガ情報取得
 * @param[in] dwJoy ゲームパッド番号
 * @param[in] dwBtn ボタンID
 * @return 指定したゲームパッドの指定したボタンがトリガされている(true)/トリガされていない(false)
 */
bool InputManager::GetJoyTrigger(DWORD dwJoy, DWORD dwBtn)
{
	if (dwJoy >= NUM_JOY_MAX) return false;
	return (m_dwJoyButtonTrigger[dwJoy] & (1 << dwBtn)) ? true : false;
}

/**
 * @brief ゲームパッドボタンリリース情報取得
 * @param[in] dwJoy ゲームパッド番号
 * @param[in] dwBtn ボタンID
 * @return 指定したゲームパッドの指定したボタンがリリースされている(true)/リリースされていない(false)
 */
bool InputManager::GetJoyRelease(DWORD dwJoy, DWORD dwBtn)
{
	if (dwJoy >= NUM_JOY_MAX) return false;
	return (m_dwJoyButtonRelease[dwJoy] & (1 << dwBtn)) ? true : false;
}

/**
 * @brief マウス座標取得
 * @return マウス座標のポインタ
 */
POINT* InputManager::GetMousePosition()
{
	return &m_ptMouse;
}

/**
 * @brief マウスボタン情報取得
 * @param[in] dwBtn ボタンID
 * @return 指定したボタンが押下されている(true)/押下されていない(false)
 */
bool InputManager::GetMouseButton(DWORD dwBtn)
{
	if (dwBtn >= _countof(g_nMouseBtn)) return false;
	return GetKeyPress(g_nMouseBtn[dwBtn]);
}

/**
 * @brief マウスボタントリガ情報取得
 * @param[in] dwBtn ボタンID
 * @return 指定したボタンがトリガされている(true)/トリガされていない(false)
 */
bool InputManager::GetMouseTrigger(DWORD dwBtn)
{
	if (dwBtn >= _countof(g_nMouseBtn)) return false;
	return GetKeyTrigger(g_nMouseBtn[dwBtn]);
}

/**
 * @brief マウスボタンリリース情報取得
 * @param[in] dwBtn ボタンID
 * @return 指定したボタンがリリースされている(true)/リリースされていない(false)
 */
bool InputManager::GetMouseRelease(DWORD dwBtn)
{
	if (dwBtn >= _countof(g_nMouseBtn)) return false;
	return GetKeyRelease(g_nMouseBtn[dwBtn]);
}

//=======================================================================================
//	End of File
//=======================================================================================
