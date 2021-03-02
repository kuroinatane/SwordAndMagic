#pragma once
/**
 * @file Camera.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */

#include "Main.h"
#include "Utility.h"

constexpr DirectX::XMFLOAT3 CAM_START_POS = DirectX::XMFLOAT3(0.0f,0.0f,-300.0f); //!< @brief カメラの初期視点位置
constexpr DirectX::XMFLOAT3 CAM_START_POS_R = DirectX::XMFLOAT3(0.0f,0.0f,0.0f); //!< @brief カメラの初期焦点位置
constexpr float VIEW_ANGLE = DirectX::XMConvertToRadians(45.0f); //!< @brief カメラの画角
constexpr float VIEW_NEAR_Z = (10.0f);	 //!< @brief カメラのニアクリップ
constexpr float VIEW_FAR_Z = (1000.0f);	 //!< @brief カメラのファークリップ

 /**
  * @class C_Camera
  * @brief カメラを定義するクラス。
  *		  シーンごとに生成して使うことを想定しているのでシーン跨ぎは要検討。
  */
class C_Camera
{
private:
	DirectX::XMFLOAT3		posCameraP;		//!< @brief カメラの視点位置
	DirectX::XMFLOAT3		posCameraR;		//!< @brief カメラの焦点位置
	DirectX::XMFLOAT3		vecCameraU;		//!< @brief カメラのアップベクトル
	DirectX::XMFLOAT3		rotCamera;		//!< @brief カメラの回転
	DirectX::XMFLOAT4X4	mtxView;			//!< @brief カメラのビュー行列
	DirectX::XMFLOAT4X4	mtxProjection;		//!< @brief カメラのプロジェクション行列
	float shakePower = 0;					//!< @brief カメラの振動値(揺らしたい時)
public:
	C_Camera();
	C_Camera(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 posr);
	~C_Camera();
	void Update();
	void Set();
	DirectX::XMFLOAT3& GetPosition(void);
	void SetPosition(DirectX::XMFLOAT3);
	DirectX::XMFLOAT4X4& GetViewMatrix(void);
	DirectX::XMFLOAT4X4& GetProjMatrix(void);
	DirectX::XMFLOAT3 GetLookAt();
	void SetShake(float pow);

	float GetViewAspect() {
		return ((float)GetScreenSize().x / GetScreenSize().y);
	}
};

