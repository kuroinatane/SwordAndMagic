//=============================================================================
//
// ���͏��� [Input.cpp]
//
//=============================================================================
#include "Input.h"
#include "Window.h"
#include "WindowManager.h"

// �萔
namespace {
	constexpr int	g_nMouseBtn[] = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_XBUTTON1, VK_XBUTTON2 }; //!< @brief �}�E�X�p�̃{�^����`��z��
}

/**
 * @brief �L�[�{�[�h�A�p�b�h�A�}�E�X��������
 * @return ��������(S_OK/�G���[���b�Z�[�W)
 */
HRESULT InputManager::Init()
{
	HRESULT hr = S_OK;

	// �L�[�{�[�h�̏�����
	hr = InitKeyboard();

	// �Q�[���p�b�h�̏�����
	if (SUCCEEDED(hr)) {
		hr = InitJoystick();
	}

	// �}�E�X�̏�����
	if (SUCCEEDED(hr)) {
		hr = InitMouse();
	}

	return hr;
}

/**
 * @brief �I������
 */
void InputManager::Uninit()
{
	// �}�E�X�̏I������
	UninitMouse();

	// �Q�[���p�b�h�̏I������
	UninitJoystick();

	// �L�[�{�[�h�̏I������
	UninitKeyboard();
}

/**
 * @brief �X�V����
 */
void InputManager::Update()
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �Q�[���p�b�h�̍X�V
	UpdateJoystick();

	// �}�E�X�̍X�V
	UpdateMouse();
}

/**
 * @brief �L�[�{�[�h�̏���������
 * @return ��������(S_OK/�G���[���b�Z�[�W)
 */
HRESULT InputManager::InitKeyboard()
{
	HRESULT hr = S_OK;

	return hr;
}

/**
 * @brief �L�[�{�[�h�̏I������
 */
void InputManager::UninitKeyboard()
{
	// (�������Ȃ�)
}

/**
 * @brief �L�[�{�[�h�̍X�V����
 * @return ��������(S_OK/�G���[���b�Z�[�W)
 */
HRESULT InputManager::UpdateKeyboard()
{
	HRESULT hr = S_OK;
	WORD wKeyState;

	// �L�[�{�[�h�̏�Ԃ��擾
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
 * @brief �Q�[���p�b�h�̏���������
 * @return ��������(S_OK/�G���[���b�Z�[�W)
 */
HRESULT InputManager::InitJoystick()
{
	GetJoyCount();

	return S_OK;
}

/**
 * @brief �Q�[���p�b�h�̏I������
 */
void InputManager::UninitJoystick()
{
	// (�������Ȃ�)
}

/**
 * @brief �Q�[���p�b�h�̍X�V����
 * @return ��������(S_OK/�G���[���b�Z�[�W)
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
 * @brief �}�E�X�̏���������
 * @return ��������(S_OK/�G���[���b�Z�[�W)
 */
HRESULT InputManager::InitMouse()
{
	return S_OK;
}

/**
 * @brief �}�E�X�̏I������
 */
void InputManager::UninitMouse()
{
	// (�������Ȃ�)
}

/**
 * @brief �}�E�X�̍X�V����
 * @return ��������(S_OK/�G���[���b�Z�[�W)
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
 * @brief �L�[�{�[�h�̉�����Ԃ��擾
 * @param[in] nKey �擾�������L�[��ID
 * @return ��������Ă���(true)/��������Ă��Ȃ�(false)
 */
bool InputManager::GetKeyPress(int nKey)
{
	return (m_wKeyState[nKey] & 0x8000) ? true : false;
}

/**
 * @brief �L�[�{�[�h�̃g���K��Ԃ��擾
 * @param[in] nKey �擾�������L�[��ID
 * @return �g���K����Ă���(true)/�g���K����Ă��Ȃ�(false)
 */
bool InputManager::GetKeyTrigger(int nKey)
{
	return (m_wKeyStateTrigger[nKey] & 0x8000) ? true : false;
}

/**
 * @brief �L�[�{�[�h�̃��s�[�g��Ԃ��擾
 * @param[in] nKey �擾�������L�[��ID
 * @return ���s�[�g����Ă���(true)/���s�[�g����Ă��Ȃ�(false)
 */
bool InputManager::GetKeyRepeat(int nKey)
{
	return (m_wKeyStateRepeat[nKey] & 0x8000) ? true : false;
}

/**
 * @brief �L�[�{�[�h�̃����[�X��Ԃ��擾
 * @param[in] nKey �擾�������L�[��ID
 * @return �����[�X����Ă���(true)/�����[�X����Ă��Ȃ�(false)
 */
bool InputManager::GetKeyRelease(int nKey)
{
	return (m_wKeyStateRelease[nKey] & 0x8000) ? true : false;
}

/**
 * @brief �Q�[���p�b�h�ڑ����擾
 * @return �Q�[���p�b�h�ڑ���
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
 * @brief �Q�[���p�b�h���擾
 * @param[in] dwJoy �Q�[���p�b�h�ԍ�
 * @return �Q�[���p�b�h�̏��(�����Ȃ�nullptr)
 */
JOYINFOEX* InputManager::GetJoyState(DWORD dwJoy)
{
	if (dwJoy >= NUM_JOY_MAX) return nullptr;
	return &m_joyState[dwJoy];
}

/**
 * @brief �Q�[���p�b�hX���擾
 * @param[in] dwJoy �Q�[���p�b�h�ԍ�
 * @return �w�肵���Q�[���p�b�h��X������(�����Ȃ�0)
 */
LONG InputManager::GetJoyX(DWORD dwJoy)
{
	if (dwJoy >= NUM_JOY_MAX) return 0L;
	return (LONG)m_joyState[dwJoy].dwXpos - 0x08000;
}

/**
 * @brief �Q�[���p�b�hY���擾
 * @param[in] dwJoy �Q�[���p�b�h�ԍ�
 * @return �w�肵���Q�[���p�b�h��Y������(�����Ȃ�0)
 */
LONG InputManager::GetJoyY(DWORD dwJoy)
{
	if (dwJoy >= NUM_JOY_MAX) return 0L;
	return (LONG)m_joyState[dwJoy].dwYpos - 0x08000;
}

/**
 * @brief �Q�[���p�b�hZ���擾
 * @param[in] dwJoy �Q�[���p�b�h�ԍ�
 * @return �w�肵���Q�[���p�b�h��Z������(�����Ȃ�0)
 */
LONG InputManager::GetJoyZ(DWORD dwJoy)
{
	if (dwJoy >= NUM_JOY_MAX) return 0L;
	return (LONG)m_joyState[dwJoy].dwZpos - 0x08000;
}

/**
 * @brief �Q�[���p�b�h�{�^�����擾
 * @param[in] dwJoy �Q�[���p�b�h�ԍ�
 * @param[in] dwBtn �{�^��ID
 * @return �w�肵���Q�[���p�b�h�̎w�肵���{�^������������Ă���(true)/��������Ă��Ȃ�(false)
 */
bool InputManager::GetJoyButton(DWORD dwJoy, DWORD dwBtn)
{
	if (dwJoy >= NUM_JOY_MAX) return false;
	return (m_joyState[dwJoy].dwButtons & (1 << dwBtn)) ? true : false;
}

/**
 * @brief �Q�[���p�b�h�{�^���g���K���擾
 * @param[in] dwJoy �Q�[���p�b�h�ԍ�
 * @param[in] dwBtn �{�^��ID
 * @return �w�肵���Q�[���p�b�h�̎w�肵���{�^�����g���K����Ă���(true)/�g���K����Ă��Ȃ�(false)
 */
bool InputManager::GetJoyTrigger(DWORD dwJoy, DWORD dwBtn)
{
	if (dwJoy >= NUM_JOY_MAX) return false;
	return (m_dwJoyButtonTrigger[dwJoy] & (1 << dwBtn)) ? true : false;
}

/**
 * @brief �Q�[���p�b�h�{�^�������[�X���擾
 * @param[in] dwJoy �Q�[���p�b�h�ԍ�
 * @param[in] dwBtn �{�^��ID
 * @return �w�肵���Q�[���p�b�h�̎w�肵���{�^���������[�X����Ă���(true)/�����[�X����Ă��Ȃ�(false)
 */
bool InputManager::GetJoyRelease(DWORD dwJoy, DWORD dwBtn)
{
	if (dwJoy >= NUM_JOY_MAX) return false;
	return (m_dwJoyButtonRelease[dwJoy] & (1 << dwBtn)) ? true : false;
}

/**
 * @brief �}�E�X���W�擾
 * @return �}�E�X���W�̃|�C���^
 */
POINT* InputManager::GetMousePosition()
{
	return &m_ptMouse;
}

/**
 * @brief �}�E�X�{�^�����擾
 * @param[in] dwBtn �{�^��ID
 * @return �w�肵���{�^������������Ă���(true)/��������Ă��Ȃ�(false)
 */
bool InputManager::GetMouseButton(DWORD dwBtn)
{
	if (dwBtn >= _countof(g_nMouseBtn)) return false;
	return GetKeyPress(g_nMouseBtn[dwBtn]);
}

/**
 * @brief �}�E�X�{�^���g���K���擾
 * @param[in] dwBtn �{�^��ID
 * @return �w�肵���{�^�����g���K����Ă���(true)/�g���K����Ă��Ȃ�(false)
 */
bool InputManager::GetMouseTrigger(DWORD dwBtn)
{
	if (dwBtn >= _countof(g_nMouseBtn)) return false;
	return GetKeyTrigger(g_nMouseBtn[dwBtn]);
}

/**
 * @brief �}�E�X�{�^�������[�X���擾
 * @param[in] dwBtn �{�^��ID
 * @return �w�肵���{�^���������[�X����Ă���(true)/�����[�X����Ă��Ȃ�(false)
 */
bool InputManager::GetMouseRelease(DWORD dwBtn)
{
	if (dwBtn >= _countof(g_nMouseBtn)) return false;
	return GetKeyRelease(g_nMouseBtn[dwBtn]);
}

//=======================================================================================
//	End of File
//=======================================================================================
