/**
 * @file GameWnd.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Window.h"

 /**
  * @class C_GameWindow
  * @brief ウィンドウクラスを継承したゲーム用ウィンドウのクラス。
  */
class C_GameWindow : public C_Window
{
private:
	bool		m_bWindow;			//!< @brief ウィンドウ / フルスクリーン

	DWORD		m_dwExecLastTime;	//!< @brief FPS計測用
	DWORD		m_dwFPSLastTime;  //!< @brief FPS計測用
	DWORD		m_dwFrameCount;  //!< @brief FPS計測用

public:
	bool InitInstance();
	int ExitInstance();
	bool OnIdle(long lCount);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool OnEraseBkgnd(HDC hdc);
	bool OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg);
};

//=======================================================================================
//	End of File
//=======================================================================================
