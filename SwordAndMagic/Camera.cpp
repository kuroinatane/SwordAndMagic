#include "Camera.h"
#include "input.h"


using namespace DirectX;

/**
 * @brief カメラの位置、回転などの情報を初期化。引数なしバージョンでは定数で初期化する。
 */
C_Camera::C_Camera() {
	posCameraP = XMFLOAT3(CAM_START_POS.x, CAM_START_POS.y, CAM_START_POS.z);
	posCameraR = XMFLOAT3(CAM_START_POS_R.x, CAM_START_POS_R.y, CAM_START_POS_R.z);
	vecCameraU = XMFLOAT3(0.0f, 1.0f, 0.0f);
	rotCamera = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Set();

}

/**
 * @brief カメラの位置、回転などの情報を初期化。
 * @param[in] pos カメラの視点位置
 * @param[in] posr カメラの焦点位置
 */
C_Camera::C_Camera(XMFLOAT3 pos, XMFLOAT3 posr) {
	posCameraP = XMFLOAT3(pos.x, pos.y, pos.z);
	posCameraR = XMFLOAT3(posr.x, posr.y, posr.z);
	vecCameraU = XMFLOAT3(0.0f, 1.0f, 0.0f);
	rotCamera = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Set();
}

/**
 * @brief 破棄するデータなどは特に無いため現状は処理無し
 */
C_Camera::~C_Camera() {

}

/**
 * @brief カメラの設定を行い、振動中は次フレームの振動値の計算を行う
 */
void C_Camera::Update(void)
{
	Set();
	
	if (fabs(shakePower) < 0.01f) {
		shakePower = 0.0f;
	}
	else {
		shakePower *= -0.999f;
	}
}

/**
 * @brief 更新処理からの呼び出し用。ビューマトリクス、プロジェクションマトリクスを更新してメンバ変数に格納する。
 */
void C_Camera::Set(void)
{
	//ビューマトリクスの作成
	XMStoreFloat4x4(&mtxView,
		XMMatrixLookAtLH(
			XMVectorSet(posCameraP.x, posCameraP.y + shakePower, posCameraP.z, 1.f),
			XMVectorSet(posCameraR.x, posCameraR.y + shakePower, posCameraR.z, 1.f),
			XMVectorSet(vecCameraU.x, vecCameraU.y, vecCameraU.z, 0.f)));

	//プロジェクションマトリクスの作成
	XMStoreFloat4x4(&mtxProjection,
		XMMatrixPerspectiveFovLH(
			VIEW_ANGLE,
			GetViewAspect(),
			VIEW_NEAR_Z,
			VIEW_FAR_Z));
}

/**
 * @brief カメラの視点位置取得。外部からの呼び出し用。
 * @return カメラの視点位置
 */
XMFLOAT3& C_Camera::GetPosition(void)
{
	return posCameraP;
}

/**
 * @brief カメラの視点位置を変更。と同時にカメラの焦点位置も同じだけ平行移動する。
 */
void C_Camera::Move(XMFLOAT3 pos)
{
	posCameraP.x += pos.x;
	posCameraP.y += pos.y;
	posCameraP.z += pos.z;
	posCameraR.x += pos.x;
	posCameraR.y += pos.y;
	posCameraR.z += pos.z;
}

void C_Camera::SetPositionP(DirectX::XMFLOAT3 pos) {
	posCameraP.x = pos.x;
	posCameraP.y = pos.y;
	posCameraP.z = pos.z;
}

void C_Camera::SetPositionR(DirectX::XMFLOAT3 pos) {
	posCameraR.x = pos.x;
	posCameraR.y = pos.y;
	posCameraR.z = pos.z;
}

/**
 * @brief カメラのビュー行列を取得する。
 * @return カメラのビュー行列
 */
XMFLOAT4X4& C_Camera::GetViewMatrix(void)
{
	return mtxView;
}

/**
 * @brief カメラのプロジェクション行列を取得する。
 * @return カメラのプロジェクション行列
 */
XMFLOAT4X4& C_Camera::GetProjMatrix(void)
{
	return mtxProjection;
}

/**
 * @brief カメラを回転
 * @param[in] rot 回転値
 */
void C_Camera::Rotate(XMFLOAT3 rot)
{
	XMFLOAT3 diff;
	diff.x = posCameraR.x - posCameraP.x;
	diff.y = posCameraR.y - posCameraP.y;
	diff.z = posCameraR.z - posCameraP.z;

	DirectX::XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&diff),XMMatrixRotationRollPitchYaw(rot.x,rot.y,rot.z));
	
	XMFLOAT3 res;
	XMStoreFloat3(&res, vec);

	posCameraR.x = posCameraP.x + res.x;
	posCameraR.y = posCameraP.y + res.y;
	posCameraR.z = posCameraP.z + res.z;
}


/**
 * @brief カメラの視線ベクトルを取得。ビルボード処理などに使用。
 * @return カメラの視線ベクトル
 */
XMFLOAT3 C_Camera::GetLookAt() {
	XMFLOAT3 diff;
	diff.x = posCameraR.x - posCameraP.x;
	diff.y = posCameraR.y - posCameraP.y;
	diff.z = posCameraR.z - posCameraP.z;
	XMVECTOR vec = XMVector3Normalize(XMLoadFloat3(&diff));
	XMStoreFloat3(&diff, vec);
	return diff;
}

/**
 * @brief カメラの振動の設定。現状は単一引数でただ縦に揺らすことにしている。
 */
void C_Camera::SetShake(float pow) {
	shakePower = pow;
}


