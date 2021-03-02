#pragma once
#include "FbxMesh.h"
#include "Component.h"
//
// FBX���f�� �N���X
//
class C_GameObject;

class C_FbxModel
{
public:
	C_FbxModel(LPCSTR pszFileName, LPCSTR vtxShader = NULL, LPCSTR pixShader = NULL);
	~C_FbxModel();
	void Update();
	// ���\�b�h
	//void Render(DirectX::XMFLOAT4X4& mWorld, DirectX::XMFLOAT4X4& mView, DirectX::XMFLOAT4X4& mProj, EByOpacity byOpacity = eNoAffect);
	void Render(DirectX::XMFLOAT4X4 mWorld, EByOpacity opacity);
	HRESULT Init(LPCSTR pszFileName, LPCSTR vtxShader = NULL,LPCSTR pixShader = NULL);
	void SetLight(C_FbxLight* light);
	void SetCamera(DirectX::XMFLOAT3& vCamera);
	void SetAnimFrame(int nFrame);
	int GetMaxAnimFrame();
	void SetAnimStack(int nAnimStack);
	int GetMaxAnimStack();
	void SetMaterial(TFbxMaterial* pMaterial = nullptr);
	void SetOutline();

private:
	C_FbxMesh* m_pRootMesh;
	// �O���̃f�o�C�X�����
	ID3D11SamplerState* m_pSampleLinear;
	ID3D11Buffer* m_pConstantBuffer0;
	ID3D11Buffer* m_pConstantBuffer1;
	ID3D11Buffer* m_pConstantBuffer2;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexSubShader;
	ID3D11PixelShader* m_pPixelSubShader;
	ID3D11InputLayout* m_pVertexSubLayout;
	DirectX::XMFLOAT4X4 m_mView;
	DirectX::XMFLOAT4X4 m_mProj;
	DirectX::XMFLOAT4X4 m_mWorld;
	// FBX
	FbxManager* m_pSdkManager;
	FbxImporter* m_pImporter;
	FbxScene* m_pScene;
	DirectX::XMFLOAT4X4 m_mFinalWorld;//�ŏI�I�ȃ��[���h�s��i���̎p���Ń����_�����O����j
	C_FbxLight* m_light;
	DirectX::XMFLOAT3 m_vCamera;
	TFbxMaterial* m_pMaterial;
	TFbxMaterial m_material;
	bool m_bOutline = false;
private:
	// �A�j���[�V�����t���[��
	int m_nAnimFrame;
	FbxTime m_tStart;
	FbxTime m_tStop;
	int m_nAnimStack;
	int nCnt = 0;
	FbxArray<FbxString*> m_strAnimStackName;

	HRESULT InitShader(LPCSTR vtxShader, LPCSTR pixShader);
	HRESULT InitFBX(LPCSTR szFileName);
	HRESULT CreateFromFBX(LPCSTR szFileName);
	HRESULT LoadRecursive(FbxNode* pNode, C_FbxMesh* pFBXMesh);
	void RenderMesh(C_FbxMesh* pMesh, EByOpacity byOpacity);
	void RecursiveRender(C_FbxMesh* pMesh, EByOpacity byOpacity);
	void SetNewPoseMatrices(C_FbxMesh* pMesh, int nFrame);
	void DestroyMesh(C_FbxMesh* pMesh);
};
