/**
 * @file EffectGenerator.h
 * @author MIURA DAIKI
 * @date 2020/02/01
 */
#pragma once
#include "Main.h"
#include "Singleton.h"
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>

class C_Scene;
/**
 * @class EffectGenerator
 * @brief Effekseer連携、エフェクト生成用のクラス。
 */
class EffectGenerator : public Singleton<EffectGenerator>
{
private:
	::EffekseerRendererDX11::Renderer* m_Renderer;
	::Effekseer::Manager* m_Manager;
public:
	EffectGenerator();
	~EffectGenerator();
	void Update();
	void Draw();
	Effekseer::Effect* CreateEffect(const EFK_CHAR* path);
	void Stop(Effekseer::Handle handle);
	Effekseer::Handle Play(Effekseer::Effect* effect);
	Effekseer::Handle Play(Effekseer::Effect* effect, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scl);
};
