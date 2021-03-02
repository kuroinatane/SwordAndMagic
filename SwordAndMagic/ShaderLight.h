/**
 * @file ShaderLight.h
 * @author MIURA DAIKI
 * @date 2020/02/20
 */
#pragma once
#include "main.h"
#include "Singleton.h"

constexpr int LIGHT_NUM = 100;


struct LightInfo {
	float pos[3];
	float type;
	float direction[3];
	float power;
	float value[4];
	LightInfo() {
		pos[0] = 0;
		pos[1] = 0;
		pos[2] = 0;
		type = 0;
		direction[0] = 0;
		direction[1] = -1;
		direction[2] = 0;
		power = 0;
		value[0] = 0;
		value[1] = 0;
		value[2] = 0;
		value[3] = 0;
	}
	LightInfo(float p[3],
		float tp,
		float dir[3],
		float pow,
		float val[4]) {
		pos[0] = p[0];
		pos[1] = p[1];
		pos[2] = p[2];
		type = tp;
		direction[0] = dir[0];
		direction[1] = dir[1];
		direction[2] = dir[2];
		power = pow;
		value[0] = val[0];
		value[1] = val[1];
		value[2] = val[2];
		value[3] = val[3];
	}
};

/**
 * @class ShaderLight
 * @brief シェーダに送る光源データを作成するためのクラス。
 */
class ShaderLightBuffer : public Singleton<ShaderLightBuffer>
{
private:
	LightInfo m_Lights[LIGHT_NUM];
	bool UsingNumber[LIGHT_NUM];
	int	m_RegCount = 0;
public:
	ShaderLightBuffer();
	~ShaderLightBuffer();
	int Register(LightInfo light);
	void Unregister(int lightNo);
	void UpdateLight(ID3D11Buffer *pConstantBuffers, ID3D11Buffer** ppConstantBuffers);
	LightInfo& GetLight(int lightNo);
};

