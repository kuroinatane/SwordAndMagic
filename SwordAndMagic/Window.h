/**
 * @file Window.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Main.h"

 /**
  * @class C_Window
  * @brief ウィンドウの基底クラス。全てのウィンドウはこのクラスから派生する。
  */
class C_Window
{
public:
	C_Window();
	virtual ~C_Window();

// 属性
private:
	HWND		m_hWnd;				//!< @brief ウィンドウ ハンドル
	HINSTANCE	m_hInstance;		//!< @brief インスタンス ハンドル
	ATOM		m_Atom;				//!< @brief ウィンドウ クラス登録時の ATOM
	LPTSTR		m_pszClassName;		//!< @brief クラス名
	WNDPROC		m_lpfnOldWndProc;	//!< @brief ウィンドウサブクラス化する時用
	bool		m_bDialog;			//!< @brief ダイアログウィンドウかどうか
	MSG			m_msgCur;			//!< @brief 現在のメッセージ
	POINT		m_ptCursorLast;		//!< @brief カーソル保持用
	UINT		m_nMsgLast;			//!< @brief メッセージ保持用

protected:


	// ウィンドウのクライアント領域を描画
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
	// オーバーライド可能なウィンドウ(ダイアログ)プロシージャ
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

	// ウィンドウハンドルとCWindowオブジェクトを結び付ける
	bool Attach(HWND hWnd);
	// ウィンドウハンドルをCWindowオブジェクトから切り離す
	HWND Detach();

	// 初期化
	virtual bool InitInstance();

	// デフォルトのメッセージ ループ
	virtual int Run();
	virtual bool PreTranslateMessage(MSG* pMsg);
	virtual bool PumpMessage();
	virtual bool OnIdle(long lCount);
	virtual bool IsIdleMessage(MSG* pMsg);

	// 終了処理
	virtual int ExitInstance();

	// ウィンドウ ハンドルを返す
	HWND GetHwnd() const {return (this) ? m_hWnd : NULL;}
	operator HWND() const {return GetHwnd();}

	// ウィンドウの表示
	bool Show(int nCmdShow) {return ::ShowWindow(m_hWnd, nCmdShow) != FALSE;}
	void Update() {::UpdateWindow(m_hWnd);}

	// メッセージ ボックスの表示
	int MessageBox(LPCTSTR pszText, LPCTSTR pszCaption = NULL, UINT nType = MB_OK);

	// ウィンドウ サイズ
	bool GetClientRect(LPRECT pRect) {return ::GetClientRect(m_hWnd, pRect) != FALSE;}
};

//=======================================================================================
//	End of File
//=======================================================================================
