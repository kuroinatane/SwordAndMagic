#include "GameWnd.h"
#include "WindowManager.h"
#include "Input.h"

//-------- ���C�u�����̃����N
#pragma comment(lib, "winmm")
#pragma comment(lib, "d3d11")

namespace {
	const TCHAR g_szCaption[] = _T("DirectX11_Game");	//!< @brief �L���v�V�������b�Z�[�W
}

/**
 * @brief ���C�������B��{�I�ɂ����ɂ͗]�v�Ȏ����������ɁA�E�B���h�E���ŏ�������B
 * @return ����I���������ǂ���
 */
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int iCmdShow)
{
	// �E�B���h�E���쐬
	int nResult = EXIT_FAILURE;

	WindowManager::GetInstance().Init(hInstance);
	InputManager::GetInstance().Init();

	//�E�B���h�E�����ꂽ���_��WindowManager�ɂ���ĊǗ������
	C_GameWindow* pWnd = new C_GameWindow();
	if (pWnd->CreateFrame(g_szCaption,				// �E�B���h�E �L���v�V����
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, 0,	// �E�B���h�E �X�^�C���C�g���X�^�C��
		CW_USEDEFAULT, CW_USEDEFAULT,		// �E�B���h�E�ʒu
		SCREEN_WIDTH, SCREEN_HEIGHT)) {				// �N���C�A���g�̈�T�C�Y
		pWnd->Show(iCmdShow);							// �E�B���h�E��\��
		pWnd->Update();									// �E�B���h�E�\�����X�V
		nResult = pWnd->Run();							// ���b�Z�[�W ���[�v
	}

	//�E�B���h�E��Manager�ɂ���ĊǗ�����Ă���̂�delete�Ŕj���͂��Ȃ�
	//delete pWnd;
	return nResult;
}

//=======================================================================================
//	End of File
//=======================================================================================
