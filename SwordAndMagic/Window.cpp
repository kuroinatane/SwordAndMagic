//=======================================================================================
//
//	  ウィンドウ クラス
//
//=======================================================================================
#include "Window.h"
#include "WindowManager.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//--定数定義--
namespace {
	constexpr TCHAR g_szProc[] = _T("PropCWindowProc");	//!< @brief プロパティを識別する文字列。変更時はWindowManagerのWndMapProcの内部も一緒に変更すること。
	constexpr TCHAR g_szClass[] = _T("CWindow"); //!< @brief クラス名。
}

/**
 * @brief 各変数の初期化
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
 * @brief 終了処理
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
 * @brief オブジェクト毎のウィンドウプロシージャ。
 * @param[in] uMsg メッセージ番号
 * @param[in] wParam 通知コード+コントロール/メニューID
 * @param[in] lParam ウィンドウハンドル
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
  * @brief ウィンドウプロシージャが処理しない部分を処理する関数。ウィンドウプロシージャの最後に呼び出す。
  * @param[in] uMsg メッセージ番号
  * @param[in] wParam 通知コード+コントロール/メニューID
  * @param[in] lParam ウィンドウハンドル
  */
LRESULT C_Window::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_lpfnOldWndProc)
		return ::CallWindowProc(m_lpfnOldWndProc, m_hWnd, uMsg, wParam, lParam);
	return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

/**
 * @brief キーが入力されたときにウィンドウプロシージャから呼ばれる関数。
 * @param[in] nChar 仮想キーコード
 * @param[in] nRepCnt リピート回数
 * @param[in] nFlags キーデータ
 */
void C_Window::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	DefWindowProc(WM_KEYDOWN, nChar, MAKELPARAM(nRepCnt, nFlags));
}

/**
 * @brief 描画を行うときにウィンドウプロシージャから呼ばれる関数。
 */
void C_Window::OnPaint()
{
	PAINTSTRUCT ps;
	OnDraw(::BeginPaint(m_hWnd, &ps));
	::EndPaint(m_hWnd, &ps);
}

/**
 * @brief 未実装(常に未処理を返す)
 * @param[in] hdc デバイスコンテキストハンドル
 * @return 処理済み(true)/未処理(false)
 */
bool C_Window::OnEraseBkgnd(HDC hdc)
{
	return false;
}

/**
 * @brief 未実装
 * @param[in] hdc デバイスコンテキストハンドル
 */
void C_Window::OnDraw(HDC hdc)
{
}

/**
 * @brief ウィンドウを破棄するときにウィンドウプロシージャから呼ばれる関数
 */
void C_Window::OnClose()
{
	// ウィンドウ破棄
	::DestroyWindow(m_hWnd);
}

/**
 * @brief アプリケーションを終了するときにウィンドウプロシージャから呼ばれる関数
 */
void C_Window::OnDestroy()
{
	// ウィンドウ ハンドル解放
	Detach();
	// アプリケーション終了
	::PostQuitMessage(0);
}

/**
 * @brief 各種設定を行いウィンドウクラスを登録する
 * @return 処理済み(true)/未処理(false)
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
		wcex.lpszClassName	= m_pszClassName;						// クラス名
		wcex.lpfnWndProc	= WindowManager::GetInstance().WndMapProc;							// ウィンドウプロシージャ
		wcex.style			= CS_VREDRAW | CS_HREDRAW;				// 占有指定
		wcex.hInstance		= m_hInstance;							// インスタンス
		wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW);		// デフォルト カーソル
		wcex.hIcon			= ::LoadIcon(NULL, IDI_APPLICATION);	// デフォルト アイコン
		wcex.hbrBackground	= (HBRUSH)::GetStockObject(WHITE_BRUSH);// 背景色
		wcex.hIconSm		= ::LoadIcon(NULL, IDI_APPLICATION);	// デフォルト アイコン
		m_Atom = ::RegisterClassEx(&wcex);
		if (!m_Atom)
			return false;
	}

	return true;
}

/**
 * @brief ウィンドウを生成します。
 * @param[in] pszWindowName ウィンドウの名前
 * @param[in] dwStyle 作成されるウィンドウスタイル
 * @param[in] dwExStyle 作成される拡張ウィンドウスタイル
 * @param[in] nx	ウィンドウ座標X
 * @param[in] ny	ウィンドウ座標Y
 * @param[in] nWidth	ウィンドウ幅
 * @param[in] nHeight	ウィンドウ高さ
 * @param[in] hwndParent	親ウィンドウのハンドル
 * @param[in] nIDorHMenu	メニューハンドル/子ウィンドウID
 * @param[in] pParam	CREATESTRUCT構造体へのポインタ
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
 * @brief ウィンドウを生成します。
 * @param[in] pszWindowName ウィンドウの名前
 * @param[in] dwStyle 作成されるウィンドウスタイル
 * @param[in] dwExStyle 作成される拡張ウィンドウスタイル
 * @param[in] rc	ウィンドウの位置、大きさの情報
 * @param[in] pParentWnd	親ウィンドウ
 * @param[in] nID	メニューハンドル/子ウィンドウID
 * @param[in] pParam	CREATESTRUCT構造体へのポインタ
 */
bool C_Window::Create(LPCTSTR pszWindowName, DWORD dwStyle, DWORD dwExStyle,
	const RECT& rc, C_Window* pParentWnd, UINT nID, LPVOID pParam)
{
	return Create(pszWindowName, dwStyle, dwExStyle,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		pParentWnd->m_hWnd, (HMENU)nID, pParam);
}

/**
 * @brief クライアントサイズを指定してウィンドウを生成します
 * @param[in] pszWindowName ウィンドウの名前
 * @param[in] dwStyle 作成されるウィンドウスタイル
 * @param[in] dwExStyle 作成される拡張ウィンドウスタイル
 * @param[in] nx	ウィンドウ座標X
 * @param[in] ny	ウィンドウ座標Y
 * @param[in] nWidth	クライアント幅
 * @param[in] nHeight	クライアント高さ
 * @param[in] hwndParent	親ウィンドウのハンドル
 * @param[in] nIDorHMenu	メニューハンドル/子ウィンドウID
 * @param[in] pParam	CREATESTRUCT構造体へのポインタ
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
 * @brief ウィンドウ ハンドルと CWindow オブジェクトを結び付ける
 * @param[in] hWnd	ウィンドウハンドル
 * @return 処理済み(true)/未処理(false)
 */
bool C_Window::Attach(HWND hWnd)
{
	// ウィンドウ ハンドル hWnd を m_hWnd に保存
	if (!hWnd)
		return false;
	m_hWnd = hWnd;
	// ダイアログかウィンドウかを判定する
	m_bDialog = (::GetWindowLongPtr(hWnd, DWLP_DLGPROC) != 0);
	int nIndex = m_bDialog ? DWLP_DLGPROC : GWLP_WNDPROC;

	// ウィンドウハンドルと CWindow オブジェクトを結びつける
	::SetProp(m_hWnd, g_szProc, (HANDLE)this);

	// 既存のウィンドウをサブクラス化する場合は、ウィンドウ(ダイアログ)
	// プロシージャをWndMapProcに置き換える
	if (::GetWindowLongPtr(m_hWnd, nIndex) != (LONG_PTR)WindowManager::GetInstance().WndMapProc)
		m_lpfnOldWndProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, nIndex, (LONG_PTR)WindowManager::GetInstance().WndMapProc);

	return true;
}

/**
 * @brief ウィンドウハンドルを CWindow オブジェクトから切り離す
 * @param[in] hWnd	ウィンドウハンドル
 * @return 切り離されたハンドル
 */
HWND C_Window::Detach()
{
	if (!m_hWnd)
		return NULL;

	// ウィンドウがサブクラス化されている場合は、
	// ウィンドウ(ダイアログ)プロシージャを元に戻す。
	if (m_lpfnOldWndProc) {
		::SetWindowLongPtr(m_hWnd, (m_bDialog ? DWLP_DLGPROC : GWLP_WNDPROC), (LONG_PTR)m_lpfnOldWndProc);
		m_lpfnOldWndProc = NULL;
	}

	// ウィンドウ ハンドルを CWindow オブジェクトから切り離す
	::RemoveProp(m_hWnd, g_szProc);

	HWND hWnd = m_hWnd;
	m_hWnd = NULL;
	return hWnd;
}

/**
 * @brief 何かあれば書き足す。
 * @return true
 */
bool C_Window::InitInstance()
{
	return true;
}

/**
 * @brief 初期化、ループ、終了まで。基本的にExitInstanceで終了する。
 * @return 正常終了ならExitInstanceの戻り値。異常終了なら-1。
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
	return -1;	// ここには来ないはず。
}

/**
 * @brief 現状は常にfalse
 * @param[in] pMsg メッセージのポインタ
 * @return 判定結果(true/false)
 */
bool C_Window::PreTranslateMessage(MSG* pMsg)
{
	return false;
}

/**
 * @brief メッセージの取得とディスパッチ
 * @return 処理済み(true)/未処理(false)
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
 * @brief カウントが0未満になるまで待つ。
 * @param[in] lCount カウント
 * @return カウントが0未満かどうか(0未満でtrue/0以上でfalse)
 */
bool C_Window::OnIdle(long lCount)
{
	return lCount < 0;
}

/**
 * @brief Idle時に呼び出せるものとそうでないものの振り分け
 * @param[in] pMsg メッセージのポインタ
 * @return 呼び出し可否(true/false)
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
	// WM_PAINT と WM_SYSTIMER(キャレット点滅)
	return pMsg->message != WM_PAINT && pMsg->message != 0x0118;
}

/**
 * @brief アプリケーション終了時の処理
 * @return 現在のメッセージのwParamフィールド
 */
int C_Window::ExitInstance()
{
	return m_msgCur.wParam;
}

/**
 * @brief ウィンドウ生成時にウィンドウプロシージャから呼ばれる関数。現状は特に仕事無し。
 * @param[in] pcs 座標
 * @return 0
 */
int C_Window::OnCreate(LPCREATESTRUCT pcs)
{
	return 0;
}

/**
 * @brief コマンド通知時にウィンドウプロシージャから呼ばれる関数。
 * @param[in] wParam 通知コード+コントロール/メニューID
 * @param[in] lParam ウィンドウハンドル
 * @return 処理済み(true)/未処理(false)
 */
bool C_Window::OnCommand(WPARAM wParam, LPARAM lParam)
{
//	WORD wNotifyCode = HIWORD(wParam);	// 通知コード
	WORD wID = LOWORD(wParam);			// 項目ID､ コントロールID､ またはアクセラレータID
//	HWND hwndCtl = (HWND)lParam;		// コントロールのハンドル
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
 * @brief ウィンドウプロシージャがメッセージを受け取ったときに呼ばれる関数
 * @param[in] hwnd カーソルのあるウィンドウのハンドル
 * @param[in] nHitTest	ヒット テスト コード
 * @param[in] uMouseMsg	マウス メッセージ識別子
 * @return 処理済み(true)/未処理(false)
 */
bool C_Window::OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg)
{
	return false;
}

/**
 * @brief ウィンドウプロシージャがメッセージを受け取ったときに呼ばれる関数
 * @param[in] nFlags キーのフラグ
 * @param[in] pt	カーソルの位置
 */
void C_Window::OnMouseMove(UINT nFlags, POINT pt)
{
	DefWindowProc(WM_MOUSEMOVE, nFlags, MAKELPARAM(pt.x, pt.y));
}

/**
 * @brief ウィンドウプロシージャがメッセージを受け取ったときに呼ばれる関数
 * @param[in] hDropInfo ドロップされたファイルを格納する構造体のハンドル
 */
void C_Window::OnDropFiles(HDROP hDropInfo)
{
	DefWindowProc(WM_DROPFILES, (WPARAM)hDropInfo, 0);
}

/**
 * @brief ウィンドウプロシージャがメッセージを受け取ったときに呼ばれる関数
 * @param[in] nFlags キーのフラグ
 * @param[in] zDelta	ホイール回転数
 * @param[in] pt	カーソルの位置
 * @return 処理済み(true)/未処理(false)
 */
bool C_Window::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
{
	return false;
}

/**
 * @brief　テキスト、タイトルなどを指定してメッセージボックスを表示します
 * @param[in] pszText メッセージ ボックスのテキストのアドレス
 * @param[in] pszCaption	メッセージ ボックスのタイトルのアドレス
 * @param[in] nType	メッセージ ボックスのスタイル
 * @return 押されたボタン
 */
int C_Window::MessageBox(LPCTSTR pszText, LPCTSTR pszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, pszText, pszCaption, nType);
}

//=======================================================================================
//	End of File
//=======================================================================================
