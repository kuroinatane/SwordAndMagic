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
  * @brief �g�p���̃E�B���h�E���Ǘ����邾���̃V���v���ȃN���X�B���C���E�B���h�E�͏��1�B
  */
class WindowManager : public Singleton<WindowManager> {
private:
	std::unique_ptr<C_Window> m_pMainWnd; //!< @brief ���C���E�B���h�E
	HINSTANCE m_hInst; //!< @brief �C���X�^���X�n���h��
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