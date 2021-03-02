#include "EffectGenerator.h"
#include "GraphicsManager.h"
#include "SoundManager.h"
#include "SceneController.h"
#include <XAudio2.h>

/**
* @brief �R���X�g���N�^�B�G�t�F�N�gManager��ݒ肵�ăG�t�F�N�g���쐬�\�ɂ��Ă����B
*/
EffectGenerator::EffectGenerator() {
	m_Renderer = ::EffekseerRendererDX11::Renderer::Create(GraphicsManager::GetInstance().GetDevice(), GraphicsManager::GetInstance().GetDeviceContext(), 8000);

	// Create a manager of effects
	// �G�t�F�N�g�̃}�l�[�W���[�̍쐬
	m_Manager = ::Effekseer::Manager::Create(4000);

	// Sprcify rendering modules
	// �`�惂�W���[���̐ݒ�
	m_Manager->SetSpriteRenderer(m_Renderer->CreateSpriteRenderer());
	m_Manager->SetRibbonRenderer(m_Renderer->CreateRibbonRenderer());
	m_Manager->SetRingRenderer(m_Renderer->CreateRingRenderer());
	m_Manager->SetTrackRenderer(m_Renderer->CreateTrackRenderer());
	m_Manager->SetModelRenderer(m_Renderer->CreateModelRenderer());

	// Specify a texture, model and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// �e�N�X�`���A���f���A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	m_Manager->SetTextureLoader(m_Renderer->CreateTextureLoader());
	m_Manager->SetModelLoader(m_Renderer->CreateModelLoader());
	m_Manager->SetMaterialLoader(m_Renderer->CreateMaterialLoader());

	// Specify sound modules
	// �T�E���h���W���[���̐ݒ�
	IXAudio2* xAudio2 = SoundManager::GetInstance().GetAudio();
	::EffekseerSound::Sound* sound = ::EffekseerSound::Sound::Create(xAudio2, 16, 16);

	// Specify a metho to play sound from an instance of sound
	// ���Đ��p�C���X�^���X����Đ��@�\���w��
	m_Manager->SetSoundPlayer(sound->CreateSoundPlayer());

	// Specify a sound data loader
	// It can be extended by yourself. It is loaded from a file on now.
	// �T�E���h�f�[�^�̓Ǎ��@�\��ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	m_Manager->SetSoundLoader(sound->CreateSoundLoader());
}

/**
* @brief �f�X�g���N�^�B�}�l�[�W���[�ƃ����_���̔j��
*/
EffectGenerator::~EffectGenerator() {
	m_Manager->Destroy();
	m_Renderer->Destroy();
}

/**
* @brief �X�V�֐��B�r���[���e�s���ݒ肷��
*/
void EffectGenerator::Update() {

	//���e�s��A�r���[�s��̓]�u�s����쐬���Đݒ�

	// Specify a projection matrix
	// ���e�s���ݒ�
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
	// �J�����s���ݒ�
	m_Renderer->SetCameraMatrix(mat44V);

	m_Manager->Update();
}

/**
* @brief �`��֐�
*/
void EffectGenerator::Draw() {
	m_Renderer->BeginRendering();
	m_Manager->Draw();
	m_Renderer->EndRendering();
}

/**
* @brief �G�t�F�N�g���w�肵�Ē�~������
* @param[in] handle �~�߂����G�t�F�N�g�̃n���h��
*/
void EffectGenerator::Stop(Effekseer::Handle handle) {
	m_Manager->StopEffect(handle);
}

/**
* @brief �G�t�F�N�g�̍Đ�
* @param[in] effect �Đ��������G�t�F�N�g�ւ̃|�C���^
*/
Effekseer::Handle EffectGenerator::Play(Effekseer::Effect* effect) {
	return m_Manager->Play(effect, 0, 0, 0);
}

/**
* @brief �ʒu�A��]�A�g�k���w�肵�ăG�t�F�N�g���Đ�
* @param[in] effect �Đ��������G�t�F�N�g�ւ̃|�C���^
* @param[in] pos �ʒu
* @param[in] rot ��]
* @param[in] scl �g�k
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
* @brief �G�t�F�N�g�̍쐬
* @param[in] path �G�t�F�N�g�f�[�^�̃p�X
* @return �쐬�����G�t�F�N�g�f�[�^
*/
Effekseer::Effect* EffectGenerator::CreateEffect(const EFK_CHAR* path) {
	return Effekseer::Effect::Create(m_Manager, path);
}