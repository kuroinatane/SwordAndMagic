#include "EffectGenerator.h"
#include "GraphicsManager.h"
#include "SoundManager.h"
#include "SceneController.h"
#include <XAudio2.h>

/**
* @brief コンストラクタ。エフェクトManagerを設定してエフェクトを作成可能にしておく。
*/
EffectGenerator::EffectGenerator() {
	m_Renderer = ::EffekseerRendererDX11::Renderer::Create(GraphicsManager::GetInstance().GetDevice(), GraphicsManager::GetInstance().GetDeviceContext(), 8000);

	// Create a manager of effects
	// エフェクトのマネージャーの作成
	m_Manager = ::Effekseer::Manager::Create(4000);

	// Sprcify rendering modules
	// 描画モジュールの設定
	m_Manager->SetSpriteRenderer(m_Renderer->CreateSpriteRenderer());
	m_Manager->SetRibbonRenderer(m_Renderer->CreateRibbonRenderer());
	m_Manager->SetRingRenderer(m_Renderer->CreateRingRenderer());
	m_Manager->SetTrackRenderer(m_Renderer->CreateTrackRenderer());
	m_Manager->SetModelRenderer(m_Renderer->CreateModelRenderer());

	// Specify a texture, model and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// テクスチャ、モデル、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	m_Manager->SetTextureLoader(m_Renderer->CreateTextureLoader());
	m_Manager->SetModelLoader(m_Renderer->CreateModelLoader());
	m_Manager->SetMaterialLoader(m_Renderer->CreateMaterialLoader());

	// Specify sound modules
	// サウンドモジュールの設定
	IXAudio2* xAudio2 = SoundManager::GetInstance().GetAudio();
	::EffekseerSound::Sound* sound = ::EffekseerSound::Sound::Create(xAudio2, 16, 16);

	// Specify a metho to play sound from an instance of sound
	// 音再生用インスタンスから再生機能を指定
	m_Manager->SetSoundPlayer(sound->CreateSoundPlayer());

	// Specify a sound data loader
	// It can be extended by yourself. It is loaded from a file on now.
	// サウンドデータの読込機能を設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	m_Manager->SetSoundLoader(sound->CreateSoundLoader());
}

/**
* @brief デストラクタ。マネージャーとレンダラの破壊
*/
EffectGenerator::~EffectGenerator() {
	m_Manager->Destroy();
	m_Renderer->Destroy();
}

/**
* @brief 更新関数。ビュー投影行列を設定する
*/
void EffectGenerator::Update() {

	//投影行列、ビュー行列の転置行列を作成して設定

	// Specify a projection matrix
	// 投影行列を設定
	Effekseer::Matrix44 mat44P;
	DirectX::XMFLOAT4X4 mat4x4P = SceneController::GetInstance().GetCurrentScene()->GetCamera()->GetProjMatrix();

	mat44P.Values[0][0] = mat4x4P._11;
	mat44P.Values[0][1] = mat4x4P._12;
	mat44P.Values[0][2] = mat4x4P._13;
	mat44P.Values[0][3] = mat4x4P._14;
	mat44P.Values[1][0] = mat4x4P._21;
	mat44P.Values[1][1] = mat4x4P._22;
	mat44P.Values[1][2] = mat4x4P._23;
	mat44P.Values[1][3] = mat4x4P._24;
	mat44P.Values[2][0] = mat4x4P._31;
	mat44P.Values[2][1] = mat4x4P._32;
	mat44P.Values[2][2] = mat4x4P._33;
	mat44P.Values[2][3] = mat4x4P._34;
	mat44P.Values[3][0] = mat4x4P._41;
	mat44P.Values[3][1] = mat4x4P._42;
	mat44P.Values[3][2] = mat4x4P._43;
	mat44P.Values[3][3] = mat4x4P._44;

	m_Renderer->SetProjectionMatrix(mat44P);

	Effekseer::Matrix44 mat44V;
	DirectX::XMFLOAT4X4 mat4x4V = SceneController::GetInstance().GetCurrentScene()->GetCamera()->GetViewMatrix();
	mat44V.Values[0][0] = mat4x4V._11;
	mat44V.Values[0][1] = mat4x4V._12;
	mat44V.Values[0][2] = mat4x4V._13;
	mat44V.Values[0][3] = mat4x4V._14;
	mat44V.Values[1][0] = mat4x4V._21;
	mat44V.Values[1][1] = mat4x4V._22;
	mat44V.Values[1][2] = mat4x4V._23;
	mat44V.Values[1][3] = mat4x4V._24;
	mat44V.Values[2][0] = mat4x4V._31;
	mat44V.Values[2][1] = mat4x4V._32;
	mat44V.Values[2][2] = mat4x4V._33;
	mat44V.Values[2][3] = mat4x4V._34;
	mat44V.Values[3][0] = mat4x4V._41;
	mat44V.Values[3][1] = mat4x4V._42;
	mat44V.Values[3][2] = mat4x4V._43;
	mat44V.Values[3][3] = mat4x4V._44;
	// Specify a camera matrix
	// カメラ行列を設定
	m_Renderer->SetCameraMatrix(mat44V);

	m_Manager->Update();
}

/**
* @brief 描画関数
*/
void EffectGenerator::Draw() {
	m_Renderer->BeginRendering();
	m_Manager->Draw();
	m_Renderer->EndRendering();
}

/**
* @brief エフェクトを指定して停止させる
* @param[in] handle 止めたいエフェクトのハンドル
*/
void EffectGenerator::Stop(Effekseer::Handle handle) {
	m_Manager->StopEffect(handle);
}

/**
* @brief エフェクトの再生
* @param[in] effect 再生したいエフェクトへのポインタ
*/
Effekseer::Handle EffectGenerator::Play(Effekseer::Effect* effect) {
	return m_Manager->Play(effect, 0, 0, 0);
}

/**
* @brief 位置、回転、拡縮を指定してエフェクトを再生
* @param[in] effect 再生したいエフェクトへのポインタ
* @param[in] pos 位置
* @param[in] rot 回転
* @param[in] scl 拡縮
*/
Effekseer::Handle EffectGenerator::Play(Effekseer::Effect* effect, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scl) {
	Effekseer::Handle handle;
	handle = m_Manager->Play(effect, 0, 0, 0);
	Effekseer::Matrix43 mat43;
	mat43.Indentity();
	Effekseer::Vector3D t = Effekseer::Vector3D(pos.x, pos.y, pos.z);
	Effekseer::Matrix43 r;
	r.Indentity();
	r.RotationXYZ(rot.x,rot.y,rot.z);
	Effekseer::Vector3D s = Effekseer::Vector3D(scl.x, scl.y, scl.z);
	mat43.SetSRT(s,r,t);

	m_Manager->SetMatrix(handle,mat43);
	return handle;
}

/**
* @brief エフェクトの作成
* @param[in] path エフェクトデータのパス
* @return 作成したエフェクトデータ
*/
Effekseer::Effect* EffectGenerator::CreateEffect(const EFK_CHAR* path) {
	return Effekseer::Effect::Create(m_Manager, path);
}