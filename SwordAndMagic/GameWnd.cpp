#include "GameWnd.h"
#include "GraphicsManager.h"
#include "Input.h"
#include "Scene.h"
#include "SceneController.h"

//-------- ���C�u�����̃����N
#pragma comment(lib, "imm32")

/**
 * @brief C_Window�N���X����̃I�[�o�[���C�h�B�Q�[���p��FPS�v�Z�Ȃǂ̂��߂̏��������s���Ƌ��ɁA�E�B���h�E���[�h���I��������B
 * @return �����ς�(true)/������(false)
 */
bool C_GameWindow::InitInstance()
{
	// IME ������
	::ImmAssociateContext(GetHwnd(), NULL);

	if (!C_Window::InitInstance()) return false;

	m_bWindow = true;
	int nID = MessageBox(_T("�E�B���h�E���[�h�Ŏ��s���܂����H"), _T("��ʃ��[�h"), MB_YESNOCANCEL);
	if (nID == IDCANCEL) return false;
	if (nID == IDNO) m_bWindow = false;

	::timeBeginPeriod(1);	// �^�C�}�̕���\���ŏ��ɃZ�b�g

	// �O���t�B�b�N�f�o�C�X������
	if (FAILED(GraphicsManager::GetInstance().Init(GetHwnd(), SCREEN_WIDTH, SCREEN_HEIGHT, m_bWindow)))
		return false;

	// ���͏�����
	InputManager::GetInstance().Init();

	// �V�[��������
	//m_pScene = CScene::Create(m_pGraph);
	//if (!m_pScene) return false;

	m_dwFPSLastTime = ::timeGetTime();
	m_dwExecLastTime = m_dwFPSLastTime - (1000/FRAME_RATE);
	m_dwFrameCount = 0;

	return true;
}

/**
 * @brief C_Window�N���X����̃I�[�o�[���C�h�B����\�����Ƃɖ߂������ȂǁB
 * @return ���s�t�@�C���̖߂�l
 */
int C_GameWindow::ExitInstance()
{
	// �V�[�����
	//SAFE_RELEASE(m_pScene);

	// ���͏I������
	InputManager::GetInstance().Uninit();

	// �O���t�B�b�N �f�o�C�X���
	GraphicsManager::GetInstance().Uninit();

	::timeEndPeriod(1);				// �^�C�}�̕���\�����ɖ߂�

	return C_Window::ExitInstance();	// ���s�t�@�C���̖߂�l
}

/**
 * @brief C_Window�N���X����̃I�[�o�[���C�h�B�V�[���̍X�V�Ȃǂ̃��C���������s���B
 * @param[in] lCount �J�E���g(���͎g���Ă��Ȃ�)
 * @return true(�Ȃɂ���O����������Βǉ�����)
 */
bool C_GameWindow::OnIdle(long lCount)
{
	// ���̕ӂŃt���[�����J�E���g
	DWORD dwCurrentTime = ::timeGetTime();			// ���݂̃^�C�}�l���擾
	if (dwCurrentTime - m_dwFPSLastTime >= 500) {	// 0.5 �b���ƂɌv��
		// �t���[�������v�Z
		//if (m_pScene) {
		//	m_pScene->SetFPS(m_dwFrameCount * 1000 / (dwCurrentTime - m_dwFPSLastTime));
		//}
		m_dwFPSLastTime = dwCurrentTime;	// �^�C�}�l���X�V
		m_dwFrameCount = 0;					// �t���[���J�E���^�����Z�b�g
	}
	// ���̕ӂŎ��ԊǗ�
	while (dwCurrentTime - m_dwExecLastTime >= (1000/FRAME_RATE)) {
		m_dwExecLastTime += (1000/FRAME_RATE);
		InputManager::GetInstance().Update();				// ���͏����X�V
		//if (m_pScene) {
		SceneController::GetInstance().GetCurrentScene()->Update();			// �Q�[�� ���C������
		//}
	}
	GraphicsManager::GetInstance().Draw(SceneController::GetInstance().GetCurrentScene());		// �����_�����O����
	m_dwFrameCount++;					// �t���[���J�E���g�{�P

	//�V�[���؂�ւ��`�F�b�N
	SceneController::GetInstance().SwapScene();
	return true;
}

/**
 * @brief C_Window�N���X����̃I�[�o�[���C�h�B
 * �L�[���͂ɔ������邪�A��{�I��input�n�̏�����input.h�ł���Ă���̂ŃE�B���h�E����鏈���̂݋L�q�B
 * @param[in] nChar �����ꂽ�L�[ID
 * @param[in] wParam �ʒm�R�[�h+�R���g���[��/���j���[ID
 * @param[in] lParam �E�B���h�E�n���h��
 */
void C_GameWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// [Esc] ��������Ă�����
	if (nChar == VK_ESCAPE) {
		// �E�B���h�E�����
		OnClose();
	}
}

/**
 * @brief C_Window�N���X����̃I�[�o�[���C�h�B����͔w�i�̏������s�킸�����ς݂Ƃ��ĕԂ��B
 * @param[in] hdc �f�o�C�X�R���e�L�X�g�n���h��
 * @return true
 */
bool C_GameWindow::OnEraseBkgnd(HDC hdc)
{
	// �w�i�������Ȃ��ŏ����ς݂Ƃ��ĕԂ�
	return true;
}

/**
 * @brief C_Window�N���X����̃I�[�o�[���C�h�B�}�E�X�J�[�\�����N���C�A���g�̈���ɂ���ꍇ�ɂ͏�������B
 * @param[in] hwnd �E�B���h�E�n���h��
 * @param[in] nHitTest �}�E�X�J�[�\�����W���N���C�A���g�̈�����ǂ���
 * @param[in] uMouseMsg ���b�Z�[�WID
 * @return ��������(true)/�������Ȃ�����(false)
 */
bool C_GameWindow::OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg)
{
	// �S��ʃ��[�h���A�}�E�X�J�[�\�����W���N���C�A���g�̈���Ȃ�
	if (!m_bWindow || nHitTest == HTCLIENT) {
		// �}�E�X�J�[�\������
		::SetCursor(NULL);
		return true;
	}
	return false;
}

//=======================================================================================
//	End of File
//=======================================================================================
