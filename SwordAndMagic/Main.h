/**
 * @file Main.h
 * @brief �G���g���|�C���g
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <d3d11.h>
#include <DirectXMath.h>


//#include "resource.h"

//*****************************************************************************
// �}�N���A�萔��`
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

constexpr int SCREEN_WIDTH = 1280;	//!< @brief �E�C���h�E�̕�
constexpr int SCREEN_HEIGHT = 720;	//!< @brief �E�C���h�E�̍���
constexpr int SCREEN_CENTER_X = SCREEN_WIDTH / 2;	//!< @brief �E�C���h�E�̒��S�w���W
constexpr int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;	//!< @brief �E�C���h�E�̒��S�x���W
constexpr int FRAME_RATE = 60; //!< @brief �t���[�����[�g(1/�t���[�����[�g�b��1��`�悷��)

//=======================================================================================
//	End of File
//=======================================================================================
