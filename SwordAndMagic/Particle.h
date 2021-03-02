/**
 * @file Particle.h
 * @author MIURA DAIKI
 * @date 2021/1/18
 */
#pragma once
#include "Component.h"

class C_GameObject;

enum PARTICLE_COMPONENT_TYPE {
	PCT_SPHERELOCATION,
	PCT_CONSTANTMOVE,
	PCT_RANDOMMOVE,
	PCT_VORTEXMOVE,
};

/**
 * @class ParticleComponent
 * @brief パーティクルの内部データ用クラス
 */
class ParticleComponent : public C_Component
{
private:
	C_GameObject* m_pObj;
	int m_nLife = 50;
	DirectX::XMFLOAT3 moveSpeed;
	DirectX::XMFLOAT3 rotateSpeed;
public:
	ParticleComponent(C_GameObject* obj);
	~ParticleComponent();
	void SetLife(int life);
	void SetSpeed(DirectX::XMFLOAT3 speed);
	void SetRotate(DirectX::XMFLOAT3 rot);
	void Update();
};

