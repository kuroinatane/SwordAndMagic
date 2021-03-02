/**
 * @file WindowManager.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Singleton.h"
#include "Window.h"
#include <memory>

 /**
  * @class WindowManager
  * @brief 使用中のウィンドウを管理するだけのシンプルなクラス。メインウィンドウは常に1つ。
  */
class WindowManager : public Singleton<WindowManager> {
private:
	std::unique_ptr<C_Window> m_pMainWnd; //!< @brief メインウィンドウ
	HINSTANCE m_hInst; //!< @brief インスタンスハンドル
public:
	WindowManager();
	virtual ~WindowManager();
	void Init(HINSTANCE hinst);
	void SetMainWindow(C_Window* window);
	C_Window* GetMainWindow();
	HWND GetMainHwnd();
	HINSTANCE GetInstanceHandle();
	static LRESULT CALLBACK WndMapProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};