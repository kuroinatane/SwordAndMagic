#include "GameWnd.h"
#include "GraphicsManager.h"
#include "Input.h"
#include "Scene.h"
#include "SceneController.h"

//-------- ライブラリのリンク
#pragma comment(lib, "imm32")

/**
 * @brief C_Windowクラスからのオーバーライド。ゲーム用にFPS計算などのための初期化を行うと共に、ウィンドウモードも選択させる。
 * @return 処理済み(true)/未処理(false)
 */
bool C_GameWindow::InitInstance()
{
	// IME 無効化
	::ImmAssociateContext(GetHwnd(), NULL);

	if (!C_Window::InitInstance()) return false;

	m_bWindow = true;
	int nID = MessageBox(_T("ウィンドウモードで実行しますか？"), _T("画面モード"), MB_YESNOCANCEL);
	if (nID == IDCANCEL) return false;
	if (nID == IDNO) m_bWindow = false;

	::timeBeginPeriod(1);	// タイマの分解能を最小にセット

	// グラフィックデバイス初期化
	if (FAILED(GraphicsManager::GetInstance().Init(GetHwnd(), SCREEN_WIDTH, SCREEN_HEIGHT, m_bWindow)))
		return false;

	// 入力初期化
	InputManager::GetInstance().Init();

	// シーン初期化
	//m_pScene = CScene::Create(m_pGraph);
	//if (!m_pScene) return false;

	m_dwFPSLastTime = ::timeGetTime();
	m_dwExecLastTime = m_dwFPSLastTime - (1000/FRAME_RATE);
	m_dwFrameCount = 0;

	return true;
}

/**
 * @brief C_Windowクラスからのオーバーライド。分解能をもとに戻す処理など。
 * @return 実行ファイルの戻り値
 */
int C_GameWindow::ExitInstance()
{
	// シーン解放
	//SAFE_RELEASE(m_pScene);

	// 入力終了処理
	InputManager::GetInstance().Uninit();

	// グラフィック デバイス解放
	GraphicsManager::GetInstance().Uninit();

	::timeEndPeriod(1);				// タイマの分解能を元に戻す

	return C_Window::ExitInstance();	// 実行ファイルの戻り値
}

/**
 * @brief C_Windowクラスからのオーバーライド。シーンの更新などのメイン処理を行う。
 * @param[in] lCount カウント(今は使っていない)
 * @return true(なにか例外処理があれば追加する)
 */
bool C_GameWindow::OnIdle(long lCount)
{
	// この辺でフレーム数カウント
	DWORD dwCurrentTime = ::timeGetTime();			// 現在のタイマ値を取得
	if (dwCurrentTime - m_dwFPSLastTime >= 500) {	// 0.5 秒ごとに計測
		// フレーム数を計算
		//if (m_pScene) {
		//	m_pScene->SetFPS(m_dwFrameCount * 1000 / (dwCurrentTime - m_dwFPSLastTime));
		//}
		m_dwFPSLastTime = dwCurrentTime;	// タイマ値を更新
		m_dwFrameCount = 0;					// フレームカウンタをリセット
	}
	// この辺で時間管理
	while (dwCurrentTime - m_dwExecLastTime >= (1000/FRAME_RATE)) {
		m_dwExecLastTime += (1000/FRAME_RATE);
		InputManager::GetInstance().Update();				// 入力処理更新
		//if (m_pScene) {
		SceneController::GetInstance().GetCurrentScene()->Update();			// ゲーム メイン処理
		//}
	}
	GraphicsManager::GetInstance().Draw(SceneController::GetInstance().GetCurrentScene());		// レンダリング処理
	m_dwFrameCount++;					// フレームカウント＋１

	//シーン切り替えチェック
	SceneController::GetInstance().SwapScene();
	return true;
}

/**
 * @brief C_Windowクラスからのオーバーライド。
 * キー入力に反応するが、基本的なinput系の処理はinput.hでやっているのでウィンドウを閉じる処理のみ記述。
 * @param[in] nChar 押されたキーID
 * @param[in] wParam 通知コード+コントロール/メニューID
 * @param[in] lParam ウィンドウハンドル
 */
void C_GameWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// [Esc] が押されていたら
	if (nChar == VK_ESCAPE) {
		// ウィンドウを閉じる
		OnClose();
	}
}

/**
 * @brief C_Windowクラスからのオーバーライド。現状は背景の消去を行わず消去済みとして返す。
 * @param[in] hdc デバイスコンテキストハンドル
 * @return true
 */
bool C_GameWindow::OnEraseBkgnd(HDC hdc)
{
	// 背景消去しないで消去済みとして返す
	return true;
}

/**
 * @brief C_Windowクラスからのオーバーライド。マウスカーソルがクライアント領域内にある場合には消去する。
 * @param[in] hwnd ウィンドウハンドル
 * @param[in] nHitTest マウスカーソル座標がクライアント領域内かどうか
 * @param[in] uMouseMsg メッセージID
 * @return 消去した(true)/消去しなかった(false)
 */
bool C_GameWindow::OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg)
{
	// 全画面モードか、マウスカーソル座標がクライアント領域内なら
	if (!m_bWindow || nHitTest == HTCLIENT) {
		// マウスカーソル消去
		::SetCursor(NULL);
		return true;
	}
	return false;
}

//=======================================================================================
//	End of File
//=======================================================================================
