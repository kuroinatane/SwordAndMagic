/**
 * @file Window.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Main.h"

 /**
  * @class C_Window
  * @brief �E�B���h�E�̊��N���X�B�S�ẴE�B���h�E�͂��̃N���X����h������B
  */
class C_Window
{
public:
	C_Window();
	virtual ~C_Window();

// ����
private:
	HWND		m_hWnd;				//!< @brief �E�B���h�E �n���h��
	HINSTANCE	m_hInstance;		//!< @brief �C���X�^���X �n���h��
	ATOM		m_Atom;				//!< @brief �E�B���h�E �N���X�o�^���� ATOM
	LPTSTR		m_pszClassName;		//!< @brief �N���X��
	WNDPROC		m_lpfnOldWndProc;	//!< @brief �E�B���h�E�T�u�N���X�����鎞�p
	bool		m_bDialog;			//!< @brief �_�C�A���O�E�B���h�E���ǂ���
	MSG			m_msgCur;			//!< @brief ���݂̃��b�Z�[�W
	POINT		m_ptCursorLast;		//!< @brief �J�[�\���ێ��p
	UINT		m_nMsgLast;			//!< @brief ���b�Z�[�W�ێ��p

protected:


	// �E�B���h�E�̃N���C�A���g�̈��`��
	virtual void OnDraw(HDC hdc);

	virtual LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual bool OnCommand(WPARAM wParam, LPARAM lParam);

	virtual int OnCreate(LPCREATESTRUCT pcs);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnPaint();
	virtual bool OnEraseBkgnd(HDC hdc);
	virtual void OnClose();
	virtual void OnDestroy();
	virtual bool OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg);
	virtual void OnMouseMove(UINT nFlags, POINT pt);
	virtual void OnDropFiles(HDROP hDropInfo);
	virtual bool OnMouseWheel(UINT nFlags, short zDelta, POINT pt);

	bool RegisterClass();

public:
	// �I�[�o�[���C�h�\�ȃE�B���h�E(�_�C�A���O)�v���V�[�W��
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool Create(LPCTSTR pszWindowName,
		DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwExStyle = 0,
		int nX = CW_USEDEFAULT, int nY = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hwndParent = NULL, HMENU nIDorHMenu = NULL, LPVOID pParam = NULL);
	bool Create(LPCTSTR pszWindowName, DWORD dwStyle, DWORD dwExStyle,
		const RECT& rc, C_Window* pParentWnd, UINT nID, LPVOID pParam = NULL);
	bool CreateFrame(LPCTSTR pszWindowName,
		DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwExStyle = 0,
		int nX = CW_USEDEFAULT, int nY = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hwndParent = NULL, HMENU nIDorHMenu = NULL, LPVOID pParam = NULL);
	void Close() {OnClose();}

	// �E�B���h�E�n���h����CWindow�I�u�W�F�N�g�����ѕt����
	bool Attach(HWND hWnd);
	// �E�B���h�E�n���h����CWindow�I�u�W�F�N�g����؂藣��
	HWND Detach();

	// ������
	virtual bool InitInstance();

	// �f�t�H���g�̃��b�Z�[�W ���[�v
	virtual int Run();
	virtual bool PreTranslateMessage(MSG* pMsg);
	virtual bool PumpMessage();
	virtual bool OnIdle(long lCount);
	virtual bool IsIdleMessage(MSG* pMsg);

	// �I������
	virtual int ExitInstance();

	// �E�B���h�E �n���h����Ԃ�
	HWND GetHwnd() const {return (this) ? m_hWnd : NULL;}
	operator HWND() const {return GetHwnd();}

	// �E�B���h�E�̕\��
	bool Show(int nCmdShow) {return ::ShowWindow(m_hWnd, nCmdShow) != FALSE;}
	void Update() {::UpdateWindow(m_hWnd);}

	// ���b�Z�[�W �{�b�N�X�̕\��
	int MessageBox(LPCTSTR pszText, LPCTSTR pszCaption = NULL, UINT nType = MB_OK);

	// �E�B���h�E �T�C�Y
	bool GetClientRect(LPRECT pRect) {return ::GetClientRect(m_hWnd, pRect) != FALSE;}
};

//=======================================================================================
//	End of File
//=======================================================================================
