#include "Utility.h"





DirectX::XMFLOAT2 GetScreenSize() {
	RECT rec;
	HWND hDeskWnd = GetDesktopWindow(); //この関数でデスクトップのハンドルを取得
	GetWindowRect(hDeskWnd, &rec); //デスクトップのハンドルからその(画面の)大きさを取得
	return DirectX::XMFLOAT2(rec.right, rec.bottom);
}

DirectX::XMFLOAT2 GetScreenAspect() {
	RECT rec;
	HWND hDeskWnd = GetDesktopWindow(); //この関数でデスクトップのハンドルを取得
	GetWindowRect(hDeskWnd, &rec); //デスクトップのハンドルからその(画面の)大きさを取得
	return DirectX::XMFLOAT2((rec.right - rec.left) / SCREEN_WIDTH, (rec.bottom - rec.top) / SCREEN_HEIGHT);
}