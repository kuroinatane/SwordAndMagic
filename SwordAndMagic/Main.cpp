#include "GameWnd.h"
#include "WindowManager.h"
#include "Input.h"

//-------- ライブラリのリンク
#pragma comment(lib, "winmm")
#pragma comment(lib, "d3d11")

namespace {
	const TCHAR g_szCaption[] = _T("DirectX11_Game");	//!< @brief キャプションメッセージ
}

/**
 * @brief メイン処理。基本的にここには余計な事をかかずに、ウィンドウ側で処理する。
 * @return 正常終了したかどうか
 */
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int iCmdShow)
{
	// ウィンドウを作成
	int nResult = EXIT_FAILURE;

	WindowManager::GetInstance().Init(hInstance);
	InputManager::GetInstance().Init();

	//ウィンドウが作られた時点でWindowManagerによって管理される
	C_GameWindow* pWnd = new C_GameWindow();
	if (pWnd->CreateFrame(g_szCaption,				// ウィンドウ キャプション
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, 0,	// ウィンドウ スタイル，拡張スタイル
		CW_USEDEFAULT, CW_USEDEFAULT,		// ウィンドウ位置
		SCREEN_WIDTH, SCREEN_HEIGHT)) {				// クライアント領域サイズ
		pWnd->Show(iCmdShow);							// ウィンドウを表示
		pWnd->Update();									// ウィンドウ表示を更新
		nResult = pWnd->Run();							// メッセージ ループ
	}

	//ウィンドウはManagerによって管理されているのでdeleteで破棄はしない
	//delete pWnd;
	return nResult;
}

//=======================================================================================
//	End of File
//=======================================================================================
