/**
 * @file GameWnd.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Window.h"

 /**
  * @class C_GameWindow
  * @brief �E�B���h�E�N���X���p�������Q�[���p�E�B���h�E�̃N���X�B
  */
class C_GameWindow : public C_Window
{
private:
	bool		m_bWindow;			//!< @brief �E�B���h�E / �t���X�N���[��

	DWORD		m_dwExecLastTime;	//!< @brief FPS�v���p
	DWORD		m_dwFPSLastTime;  //!< @brief FPS�v���p
	DWORD		m_dwFrameCount;  //!< @brief FPS�v���p

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
