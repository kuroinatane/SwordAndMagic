/**
 * @file FbxMesh.h
 * @author MIURA DAIKI
 * @date 2020/12/14
 */
#pragma once
#include "FbxUtility.h"
#include "FbxLight.h"

 /**
  * @class C_FbxMesh
  * @brief FBX�\���p�̊e���b�V���̒�`�A�e�폈�����s���N���X�B
  */
class C_FbxMesh
{
public:
	C_FbxMesh();
	~C_FbxMesh();

	C_FbxMesh** m_ppChild;	// �����̎q�ւ̃|�C���^
	DWORD m_dwNumChild;		// �q�̐�

public:
	// �O���̃f�o�C�X�����
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampleLinear;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer0;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer1;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer2;
	FbxNode* m_pFBXNode;						// FBX����p���s������o���ۂɎg��FBX�|�C���^
	DirectX::XMFLOAT4X4 m_mView;
	DirectX::XMFLOAT4X4 m_mProj;
	C_FbxLight* m_pLight;
	DirectX::XMFLOAT3* m_pCamera;
	// �A�j���[�V�����֘A
	DirectX::XMFLOAT4X4 m_mParentOrientation;	// �e�̎p���s��
	DirectX::XMFLOAT4X4 m_mFBXOrientation;		// �����̎p���s�� (�e���猩�����Ύp��)
	DirectX::XMFLOAT4X4 m_mFinalWorld;			// �ŏI�I�ȃ��[���h�s�� (���̎p���Ń����_�����O����)
	TFbxMaterial* m_pMateUsr;

private:
	// ���b�V���֘A
	DWORD m_dwNumVert;
	DWORD m_dwNumFace;
	DWORD m_dwNumUV;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer** m_ppIndexBuffer;
	TFbxMaterial* m_pMaterial;
	DWORD m_dwNumMaterial;
	// �{�[��
	int m_nNumSkin;
	TSkinInfo* m_pBoneTable;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBufferBone;

public:
	// ���\�b�h
	HRESULT CreateFromFBX(FbxMesh* pFbxMesh);
	void RenderMesh(EByOpacity byOpacity = eNoAffect);
	void SetNewPoseMatrices(int nFrame);

private:
	HRESULT CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer);
	HRESULT ReadSkinInfo(FbxMesh* pFbxMesh, TFbxVertex* pvVB, TPolyTable* PolyTable);
	DirectX::XMFLOAT4X4 GetCurrentPoseMatrix(int nSkin, int nIndex);
};