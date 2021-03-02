#pragma once
/**
 * @file BasicMesh.h
 * @author MIURA DAIKI
 * @date 2020/12/15
 */
#pragma once
#include "mesh.h"

class C_GameObject;

class BasicMeshComponent : public C_Mesh
{
private:
public:
	BasicMeshComponent(C_GameObject* obj, TCHAR* szTexture);
	BasicMeshComponent(C_GameObject* obj, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture);
	~BasicMeshComponent();
	void Update();
	void ChangeTexture(TCHAR* szTexture);
	void ChangeTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture);
};

