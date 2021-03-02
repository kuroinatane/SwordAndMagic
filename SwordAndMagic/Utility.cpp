#include "Utility.h"





DirectX::XMFLOAT2 GetScreenSize() {
	RECT rec;
	HWND hDeskWnd = GetDesktopWindow(); //���̊֐��Ńf�X�N�g�b�v�̃n���h�����擾
	GetWindowRect(hDeskWnd, &rec); //�f�X�N�g�b�v�̃n���h�����炻��(��ʂ�)�傫�����擾
	return DirectX::XMFLOAT2(rec.right, rec.bottom);
}

DirectX::XMFLOAT2 GetScreenAspect() {
	RECT rec;
	HWND hDeskWnd = GetDesktopWindow(); //���̊֐��Ńf�X�N�g�b�v�̃n���h�����擾
	GetWindowRect(hDeskWnd, &rec); //�f�X�N�g�b�v�̃n���h�����炻��(��ʂ�)�傫�����擾
	return DirectX::XMFLOAT2((rec.right - rec.left) / SCREEN_WIDTH, (rec.bottom - rec.top) / SCREEN_HEIGHT);
}