#include "WindowManager.h"



/**
 * @brief メインウィンドウをnullで初期化
 */
WindowManager::WindowManager() { 
	m_pMainWnd.reset();
}

/**
 * @brief ウィンドウはスマートポインタで管理されているので特にやることはない。
 */
WindowManager::~WindowManager() {

}

/**
 * @brief 初期化。WindowManagerのインスタンス生成と同時にアプリケーションのインスタンスハンドルを取得
 */
void WindowManager::Init(HINSTANCE hinst) {
	m_hInst = hinst;
}

/**
 * @brief アクティブなウィンドウのハンドルを取得する
 */
HWND WindowManager::GetMainHwnd() {
	return (m_pMainWnd) ? m_pMainWnd->GetHwnd() : NULL; 
}

/**
 * @brief アクティブなウィンドウを設定、変更する。
 */
void WindowManager::SetMainWindow(C_Window* window) {
	m_pMainWnd.reset(window);
}

/**
 * @brief アクティブなウィンドウを取得する。
 */
C_Window* WindowManager::GetMainWindow() {
	return m_pMainWnd.get();
}

/**
 * @brief アプリケーションのインスタンスハンドルを取得
 * @return インスタンスハンドル
 */
HINSTANCE WindowManager::GetInstanceHandle() {
	return m_hInst;
}

/**
 * @brief メッセージを振り分けるウィンドウ(ダイアログ)プロシージャ。GetPropの引数でWindowクラスの定数値を使用しているため、
 * 向こうの定数を変えた場合はこちらの関数もセットで変える必要がある。(逆も然り)
 */
LRESULT CALLBACK WindowManager::WndMapProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	C_Window* pWnd = (C_Window*)::GetProp(hWnd, _T("PropCWindowProc"));
	if (pWnd)	// インスタンス毎のプロシージャ処理
		return pWnd->WndProc(uMsg, wParam, lParam);

	// ダイアログの場合、FALSEを返す
	if (::GetWindowLongPtr(hWnd, DWLP_DLGPROC))
		return FALSE;
	// デフォルトウィンドウプロシージャを呼び出す
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}