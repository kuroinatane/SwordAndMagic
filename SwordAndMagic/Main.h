/**
 * @file Main.h
 * @brief エントリポイント
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <d3d11.h>
#include <DirectXMath.h>


//#include "resource.h"

//*****************************************************************************
// マクロ、定数定義
//*****************************************************************************
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if(x){(x)->Release();x=nullptr;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x){delete(x);x=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x){delete[](x);x=nullptr;}}
#endif

constexpr int SCREEN_WIDTH = 1280;	//!< @brief ウインドウの幅
constexpr int SCREEN_HEIGHT = 720;	//!< @brief ウインドウの高さ
constexpr int SCREEN_CENTER_X = SCREEN_WIDTH / 2;	//!< @brief ウインドウの中心Ｘ座標
constexpr int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;	//!< @brief ウインドウの中心Ｙ座標
constexpr int FRAME_RATE = 60; //!< @brief フレームレート(1/フレームレート秒に1回描画する)

//=======================================================================================
//	End of File
//=======================================================================================
