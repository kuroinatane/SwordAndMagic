//=======================================================================================
//
//	  �E�B���h�E �N���X
//
//=======================================================================================
#include "Window.h"
#include "WindowManager.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//--�萔��`--
namespace {
	constexpr TCHAR g_szProc[] = _T("PropCWindowProc");	//!< @brief �v���p�e�B�����ʂ��镶����B�ύX����WindowManager��WndMapProc�̓������ꏏ�ɕύX���邱�ƁB
	constexpr TCHAR g_szClass[] = _T("CWindow"); //!< @brief �N���X���B
}

/**
 * @brief �e�ϐ��̏�����
 */
C_Window::C_Window() :
	m_hWnd(NULL),
	m_lpfnOldWndProc(NULL),
	m_bDialog(false),
	m_nMsgLast(0),
	m_pszClassName(NULL),
	m_Atom(NULL)
{
	m_ptCursorLast.x = m_ptCursorLast.y = -1;
	m_hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	::ZeroMemory(&m_msgCur, sizeof(m_msgCur));
}

/**
 * @brief �I������
 */
C_Window::~C_Window()
{
	HWND hWnd = Detach();
	if (hWnd)
		::DestroyWindow(hWnd);
	if (m_Atom) {
		::UnregisterClass(m_pszClassName, m_hInstance);
		m_Atom = NULL;
	}
	if (m_pszClassName) {
		delete[] m_pszClassName;
		m_pszClassName = NULL;
	}
}

/**
 * @brief �I�u�W�F�N�g���̃E�B���h�E�v���V�[�W���B
 * @param[in] uMsg ���b�Z�[�W�ԍ�
 * @param[in] wParam �ʒm�R�[�h+�R���g���[��/���j���[ID
 * @param[in] lParam �E�B���h�E�n���h��
 */
LRESULT C_Window::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(WindowManager::GetInstance().GetMainHwnd(), uMsg, wParam, lParam))
		return true;

	switch (uMsg) {
	case WM_CREATE:
		return OnCreate((LPCREATESTRUCT)lParam);

	case WM_KEYDOWN:
		OnKeyDown(wParam, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_PAINT:
		OnPaint();
		return 1;

	case WM_ERASEBKGND:
		if (OnEraseBkgnd((HDC)wParam))
			return 1;
		break;

	case WM_CLOSE:
		OnClose();
		return 0;

	case WM_DESTROY:
		OnDestroy();
		break;

	case WM_COMMAND:
		if (OnCommand(wParam, lParam))
			return 0;
		break;

	case WM_SETCURSOR:
		if (OnSetCursor((HWND)wParam, LOWORD(lParam), HIWORD(lParam)))
			return TRUE;
		break;

	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = (short)LOWORD(lParam);
			pt.y = (short)HIWORD(lParam);
			OnMouseMove(wParam, pt);
		}
		return 0;

	case WM_DROPFILES:
		OnDropFiles((HDROP)wParam);
		return 0;

	case WM_MOUSEWHEEL:
		{
			POINT pt;
			pt.x = (short)LOWORD(lParam);
			pt.y = (short)HIWORD(lParam);
			if (OnMouseWheel(GET_KEYSTATE_WPARAM(wParam),
				GET_WHEEL_DELTA_WPARAM(wParam), pt)) return 0;
		}
		break;

	default:
		break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

/**
  * @brief �E�B���h�E�v���V�[�W�����������Ȃ���������������֐��B�E�B���h�E�v���V�[�W���̍Ō�ɌĂяo���B
  * @param[in] uMsg ���b�Z�[�W�ԍ�
  * @param[in] wParam �ʒm�R�[�h+�R���g���[��/���j���[ID
  * @param[in] lParam �E�B���h�E�n���h��
  */
LRESULT C_Window::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_lpfnOldWndProc)
		return ::CallWindowProc(m_lpfnOldWndProc, m_hWnd, uMsg, wParam, lParam);
	return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

/**
 * @brief �L�[�����͂��ꂽ�Ƃ��ɃE�B���h�E�v���V�[�W������Ă΂��֐��B
 * @param[in] nChar ���z�L�[�R�[�h
 * @param[in] nRepCnt ���s�[�g��
 * @param[in] nFlags �L�[�f�[�^
 */
void C_Window::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	DefWindowProc(WM_KEYDOWN, nChar, MAKELPARAM(nRepCnt, nFlags));
}

/**
 * @brief �`����s���Ƃ��ɃE�B���h�E�v���V�[�W������Ă΂��֐��B
 */
void C_Window::OnPaint()
{
	PAINTSTRUCT ps;
	OnDraw(::BeginPaint(m_hWnd, &ps));
	::EndPaint(m_hWnd, &ps);
}

/**
 * @brief ������(��ɖ�������Ԃ�)
 * @param[in] hdc �f�o�C�X�R���e�L�X�g�n���h��
 * @return �����ς�(true)/������(false)
 */
bool C_Window::OnEraseBkgnd(HDC hdc)
{
	return false;
}

/**
 * @brief ������
 * @param[in] hdc �f�o�C�X�R���e�L�X�g�n���h��
 */
void C_Window::OnDraw(HDC hdc)
{
}

/**
 * @brief �E�B���h�E��j������Ƃ��ɃE�B���h�E�v���V�[�W������Ă΂��֐�
 */
void C_Window::OnClose()
{
	// �E�B���h�E�j��
	::DestroyWindow(m_hWnd);
}

/**
 * @brief �A�v���P�[�V�������I������Ƃ��ɃE�B���h�E�v���V�[�W������Ă΂��֐�
 */
void C_Window::OnDestroy()
{
	// �E�B���h�E �n���h�����
	Detach();
	// �A�v���P�[�V�����I��
	::PostQuitMessage(0);
}

/**
 * @brief �e��ݒ���s���E�B���h�E�N���X��o�^����
 * @return �����ς�(true)/������(false)
 */
bool C_Window::RegisterClass()
{
	if (!m_pszClassName) {
		m_pszClassName = new TCHAR[_tcslen(g_szClass) + 8 + 1];
		::wsprintf(m_pszClassName, _T("%s%08X"), g_szClass, ::GetTickCount());
		if (!m_pszClassName)
			return false;
	}

	if (!m_Atom) {
		WNDCLASSEX wcex;
		ZeroMemory(&wcex, sizeof(WNDCLASSEX));
		wcex.cbSize			= sizeof(wcex);
		wcex.lpszClassName	= m_pszClassName;						// �N���X��
		wcex.lpfnWndProc	= WindowManager::GetInstance().WndMapProc;							// �E�B���h�E�v���V�[�W��
		wcex.style			= CS_VREDRAW | CS_HREDRAW;				// ��L�w��
		wcex.hInstance		= m_hInstance;							// �C���X�^���X
		wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW);		// �f�t�H���g �J�[�\��
		wcex.hIcon			= ::LoadIcon(NULL, IDI_APPLICATION);	// �f�t�H���g �A�C�R��
		wcex.hbrBackground	= (HBRUSH)::GetStockObject(WHITE_BRUSH);// �w�i�F
		wcex.hIconSm		= ::LoadIcon(NULL, IDI_APPLICATION);	// �f�t�H���g �A�C�R��
		m_Atom = ::RegisterClassEx(&wcex);
		if (!m_Atom)
			return false;
	}

	return true;
}

/**
 * @brief �E�B���h�E�𐶐����܂��B
 * @param[in] pszWindowName �E�B���h�E�̖��O
 * @param[in] dwStyle �쐬�����E�B���h�E�X�^�C��
 * @param[in] dwExStyle �쐬�����g���E�B���h�E�X�^�C��
 * @param[in] nx	�E�B���h�E���WX
 * @param[in] ny	�E�B���h�E���WY
 * @param[in] nWidth	�E�B���h�E��
 * @param[in] nHeight	�E�B���h�E����
 * @param[in] hwndParent	�e�E�B���h�E�̃n���h��
 * @param[in] nIDorHMenu	���j���[�n���h��/�q�E�B���h�EID
 * @param[in] pParam	CREATESTRUCT�\���̂ւ̃|�C���^
 */
bool C_Window::Create(LPCTSTR pszWindowName, DWORD dwStyle, DWORD dwExStyle,
	int nX, int nY, int nWidth, int nHeight,
	HWND hwndParent, HMENU nIDorHMenu, LPVOID pParam)
{
	if (!RegisterClass())
		return false;

	HWND hWnd = ::CreateWindowEx(dwExStyle, m_pszClassName, pszWindowName, dwStyle,
		nX, nY, nWidth, nHeight, hwndParent, nIDorHMenu, m_hInstance, pParam);
	if (hWnd) {
		if (!Attach(hWnd)) {
			::DestroyWindow(hWnd);
			m_hWnd = NULL;
		}
	}
	if (hWnd && !WindowManager::GetInstance().GetMainWindow()) {
		WindowManager::GetInstance().SetMainWindow(this);
	}
	return m_hWnd != NULL;
}

/**
 * @brief �E�B���h�E�𐶐����܂��B
 * @param[in] pszWindowName �E�B���h�E�̖��O
 * @param[in] dwStyle �쐬�����E�B���h�E�X�^�C��
 * @param[in] dwExStyle �쐬�����g���E�B���h�E�X�^�C��
 * @param[in] rc	�E�B���h�E�̈ʒu�A�傫���̏��
 * @param[in] pParentWnd	�e�E�B���h�E
 * @param[in] nID	���j���[�n���h��/�q�E�B���h�EID
 * @param[in] pParam	CREATESTRUCT�\���̂ւ̃|�C���^
 */
bool C_Window::Create(LPCTSTR pszWindowName, DWORD dwStyle, DWORD dwExStyle,
	const RECT& rc, C_Window* pParentWnd, UINT nID, LPVOID pParam)
{
	return Create(pszWindowName, dwStyle, dwExStyle,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		pParentWnd->m_hWnd, (HMENU)nID, pParam);
}

/**
 * @brief �N���C�A���g�T�C�Y���w�肵�ăE�B���h�E�𐶐����܂�
 * @param[in] pszWindowName �E�B���h�E�̖��O
 * @param[in] dwStyle �쐬�����E�B���h�E�X�^�C��
 * @param[in] dwExStyle �쐬�����g���E�B���h�E�X�^�C��
 * @param[in] nx	�E�B���h�E���WX
 * @param[in] ny	�E�B���h�E���WY
 * @param[in] nWidth	�N���C�A���g��
 * @param[in] nHeight	�N���C�A���g����
 * @param[in] hwndParent	�e�E�B���h�E�̃n���h��
 * @param[in] nIDorHMenu	���j���[�n���h��/�q�E�B���h�EID
 * @param[in] pParam	CREATESTRUCT�\���̂ւ̃|�C���^
 */
bool C_Window::CreateFrame(LPCTSTR pszWindowName,
	DWORD dwStyle, DWORD dwExStyle,
	int nX, int nY, int nClientWidth, int nClientHeight,
	HWND hwndParent, HMENU nIDorHMenu, LPVOID pParam)
{
	RECT rc = {0, 0, nClientWidth, nClientHeight};
	if (!::AdjustWindowRectEx(&rc, dwStyle,
		nIDorHMenu && !IS_INTRESOURCE(nIDorHMenu), dwExStyle))
		return false;

	return Create(pszWindowName, dwStyle, dwExStyle,
		nX, nY, rc.right - rc.left, rc.bottom - rc.top,
		hwndParent, nIDorHMenu, pParam);
}

/**
 * @brief �E�B���h�E �n���h���� CWindow �I�u�W�F�N�g�����ѕt����
 * @param[in] hWnd	�E�B���h�E�n���h��
 * @return �����ς�(true)/������(false)
 */
bool C_Window::Attach(HWND hWnd)
{
	// �E�B���h�E �n���h�� hWnd �� m_hWnd �ɕۑ�
	if (!hWnd)
		return false;
	m_hWnd = hWnd;
	// �_�C�A���O���E�B���h�E���𔻒肷��
	m_bDialog = (::GetWindowLongPtr(hWnd, DWLP_DLGPROC) != 0);
	int nIndex = m_bDialog ? DWLP_DLGPROC : GWLP_WNDPROC;

	// �E�B���h�E�n���h���� CWindow �I�u�W�F�N�g�����т���
	::SetProp(m_hWnd, g_szProc, (HANDLE)this);

	// �����̃E�B���h�E���T�u�N���X������ꍇ�́A�E�B���h�E(�_�C�A���O)
	// �v���V�[�W����WndMapProc�ɒu��������
	if (::GetWindowLongPtr(m_hWnd, nIndex) != (LONG_PTR)WindowManager::GetInstance().WndMapProc)
		m_lpfnOldWndProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, nIndex, (LONG_PTR)WindowManager::GetInstance().WndMapProc);

	return true;
}

/**
 * @brief �E�B���h�E�n���h���� CWindow �I�u�W�F�N�g����؂藣��
 * @param[in] hWnd	�E�B���h�E�n���h��
 * @return �؂藣���ꂽ�n���h��
 */
HWND C_Window::Detach()
{
	if (!m_hWnd)
		return NULL;

	// �E�B���h�E���T�u�N���X������Ă���ꍇ�́A
	// �E�B���h�E(�_�C�A���O)�v���V�[�W�������ɖ߂��B
	if (m_lpfnOldWndProc) {
		::SetWindowLongPtr(m_hWnd, (m_bDialog ? DWLP_DLGPROC : GWLP_WNDPROC), (LONG_PTR)m_lpfnOldWndProc);
		m_lpfnOldWndProc = NULL;
	}

	// �E�B���h�E �n���h���� CWindow �I�u�W�F�N�g����؂藣��
	::RemoveProp(m_hWnd, g_szProc);

	HWND hWnd = m_hWnd;
	m_hWnd = NULL;
	return hWnd;
}

/**
 * @brief ��������Ώ��������B
 * @return true
 */
bool C_Window::InitInstance()
{
	return true;
}

/**
 * @brief �������A���[�v�A�I���܂ŁB��{�I��ExitInstance�ŏI������B
 * @return ����I���Ȃ�ExitInstance�̖߂�l�B�ُ�I���Ȃ�-1�B
 */
int C_Window::Run()
{
	if (!InitInstance())
		return ExitInstance();

	bool bIdle = TRUE;
	long lIdleCount = 0;
	for (;;) {
		while (bIdle && !::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE)) {
			if (!OnIdle(lIdleCount++))
				bIdle = false;
		}

		do {
			if (!PumpMessage())
				return ExitInstance();
			if (IsIdleMessage(&m_msgCur)) {
				bIdle = true;
				lIdleCount = 0;
			}

		} while (::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE));
	}
	return -1;	// �����ɂ͗��Ȃ��͂��B
}

/**
 * @brief ����͏��false
 * @param[in] pMsg ���b�Z�[�W�̃|�C���^
 * @return ���茋��(true/false)
 */
bool C_Window::PreTranslateMessage(MSG* pMsg)
{
	return false;
}

/**
 * @brief ���b�Z�[�W�̎擾�ƃf�B�X�p�b�`
 * @return �����ς�(true)/������(false)
 */
bool C_Window::PumpMessage()
{
	if (!::GetMessage(&m_msgCur, NULL, NULL, NULL))
		return false;

	if (!PreTranslateMessage(&m_msgCur)) {
		::TranslateMessage(&m_msgCur);
		::DispatchMessage(&m_msgCur);
	}
	return true;
}

/**
 * @brief �J�E���g��0�����ɂȂ�܂ő҂B
 * @param[in] lCount �J�E���g
 * @return �J�E���g��0�������ǂ���(0������true/0�ȏ��false)
 */
bool C_Window::OnIdle(long lCount)
{
	return lCount < 0;
}

/**
 * @brief Idle���ɌĂяo������̂Ƃ����łȂ����̂̐U�蕪��
 * @param[in] pMsg ���b�Z�[�W�̃|�C���^
 * @return �Ăяo����(true/false)
 */
bool C_Window::IsIdleMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_NCMOUSEMOVE) {
		if (m_ptCursorLast.x == pMsg->pt.x && m_ptCursorLast.y == pMsg->pt.y &&
			pMsg->message == m_nMsgLast)
			return false;
		m_ptCursorLast = pMsg->pt;
		m_nMsgLast = pMsg->message;
		return true;
	}
	// WM_PAINT �� WM_SYSTIMER(�L�����b�g�_��)
	return pMsg->message != WM_PAINT && pMsg->message != 0x0118;
}

/**
 * @brief �A�v���P�[�V�����I�����̏���
 * @return ���݂̃��b�Z�[�W��wParam�t�B�[���h
 */
int C_Window::ExitInstance()
{
	return m_msgCur.wParam;
}

/**
 * @brief �E�B���h�E�������ɃE�B���h�E�v���V�[�W������Ă΂��֐��B����͓��Ɏd�������B
 * @param[in] pcs ���W
 * @return 0
 */
int C_Window::OnCreate(LPCREATESTRUCT pcs)
{
	return 0;
}

/**
 * @brief �R�}���h�ʒm���ɃE�B���h�E�v���V�[�W������Ă΂��֐��B
 * @param[in] wParam �ʒm�R�[�h+�R���g���[��/���j���[ID
 * @param[in] lParam �E�B���h�E�n���h��
 * @return �����ς�(true)/������(false)
 */
bool C_Window::OnCommand(WPARAM wParam, LPARAM lParam)
{
//	WORD wNotifyCode = HIWORD(wParam);	// �ʒm�R�[�h
	WORD wID = LOWORD(wParam);			// ����ID� �R���g���[��ID� �܂��̓A�N�Z�����[�^ID
//	HWND hwndCtl = (HWND)lParam;		// �R���g���[���̃n���h��
	if (m_bDialog) {
		switch (wID) {
		case IDOK:
		case IDCANCEL:
			::EndDialog(m_hWnd, wID);
			return true;
		default:
			break;
		}
	}
	return false;
}

/**
 * @brief �E�B���h�E�v���V�[�W�������b�Z�[�W���󂯎�����Ƃ��ɌĂ΂��֐�
 * @param[in] hwnd �J�[�\���̂���E�B���h�E�̃n���h��
 * @param[in] nHitTest	�q�b�g �e�X�g �R�[�h
 * @param[in] uMouseMsg	�}�E�X ���b�Z�[�W���ʎq
 * @return �����ς�(true)/������(false)
 */
bool C_Window::OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg)
{
	return false;
}

/**
 * @brief �E�B���h�E�v���V�[�W�������b�Z�[�W���󂯎�����Ƃ��ɌĂ΂��֐�
 * @param[in] nFlags �L�[�̃t���O
 * @param[in] pt	�J�[�\���̈ʒu
 */
void C_Window::OnMouseMove(UINT nFlags, POINT pt)
{
	DefWindowProc(WM_MOUSEMOVE, nFlags, MAKELPARAM(pt.x, pt.y));
}

/**
 * @brief �E�B���h�E�v���V�[�W�������b�Z�[�W���󂯎�����Ƃ��ɌĂ΂��֐�
 * @param[in] hDropInfo �h���b�v���ꂽ�t�@�C�����i�[����\���̂̃n���h��
 */
void C_Window::OnDropFiles(HDROP hDropInfo)
{
	DefWindowProc(WM_DROPFILES, (WPARAM)hDropInfo, 0);
}

/**
 * @brief �E�B���h�E�v���V�[�W�������b�Z�[�W���󂯎�����Ƃ��ɌĂ΂��֐�
 * @param[in] nFlags �L�[�̃t���O
 * @param[in] zDelta	�z�C�[����]��
 * @param[in] pt	�J�[�\���̈ʒu
 * @return �����ς�(true)/������(false)
 */
bool C_Window::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
{
	return false;
}

/**
 * @brief�@�e�L�X�g�A�^�C�g���Ȃǂ��w�肵�ă��b�Z�[�W�{�b�N�X��\�����܂�
 * @param[in] pszText ���b�Z�[�W �{�b�N�X�̃e�L�X�g�̃A�h���X
 * @param[in] pszCaption	���b�Z�[�W �{�b�N�X�̃^�C�g���̃A�h���X
 * @param[in] nType	���b�Z�[�W �{�b�N�X�̃X�^�C��
 * @return �����ꂽ�{�^��
 */
int C_Window::MessageBox(LPCTSTR pszText, LPCTSTR pszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, pszText, pszCaption, nType);
}

//=======================================================================================
//	End of File
//=======================================================================================
