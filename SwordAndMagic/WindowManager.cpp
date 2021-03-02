#include "WindowManager.h"



/**
 * @brief ���C���E�B���h�E��null�ŏ�����
 */
WindowManager::WindowManager() { 
	m_pMainWnd.reset();
}

/**
 * @brief �E�B���h�E�̓X�}�[�g�|�C���^�ŊǗ�����Ă���̂œ��ɂ�邱�Ƃ͂Ȃ��B
 */
WindowManager::~WindowManager() {

}

/**
 * @brief �������BWindowManager�̃C���X�^���X�����Ɠ����ɃA�v���P�[�V�����̃C���X�^���X�n���h�����擾
 */
void WindowManager::Init(HINSTANCE hinst) {
	m_hInst = hinst;
}

/**
 * @brief �A�N�e�B�u�ȃE�B���h�E�̃n���h�����擾����
 */
HWND WindowManager::GetMainHwnd() {
	return (m_pMainWnd) ? m_pMainWnd->GetHwnd() : NULL; 
}

/**
 * @brief �A�N�e�B�u�ȃE�B���h�E��ݒ�A�ύX����B
 */
void WindowManager::SetMainWindow(C_Window* window) {
	m_pMainWnd.reset(window);
}

/**
 * @brief �A�N�e�B�u�ȃE�B���h�E���擾����B
 */
C_Window* WindowManager::GetMainWindow() {
	return m_pMainWnd.get();
}

/**
 * @brief �A�v���P�[�V�����̃C���X�^���X�n���h�����擾
 * @return �C���X�^���X�n���h��
 */
HINSTANCE WindowManager::GetInstanceHandle() {
	return m_hInst;
}

/**
 * @brief ���b�Z�[�W��U�蕪����E�B���h�E(�_�C�A���O)�v���V�[�W���BGetProp�̈�����Window�N���X�̒萔�l���g�p���Ă��邽�߁A
 * �������̒萔��ς����ꍇ�͂�����̊֐����Z�b�g�ŕς���K�v������B(�t���R��)
 */
LRESULT CALLBACK WindowManager::WndMapProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	C_Window* pWnd = (C_Window*)::GetProp(hWnd, _T("PropCWindowProc"));
	if (pWnd)	// �C���X�^���X���̃v���V�[�W������
		return pWnd->WndProc(uMsg, wParam, lParam);

	// �_�C�A���O�̏ꍇ�AFALSE��Ԃ�
	if (::GetWindowLongPtr(hWnd, DWLP_DLGPROC))
		return FALSE;
	// �f�t�H���g�E�B���h�E�v���V�[�W�����Ăяo��
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}