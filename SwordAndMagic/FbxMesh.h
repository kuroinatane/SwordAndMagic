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
  * @brief FBX表示用の各メッシュの定義、各種処理を行うクラス。
  */
class C_FbxMesh
{
public:
	C_FbxMesh();
	~C_FbxMesh();

	C_FbxMesh** m_ppChild;	// 自分の子へのポインタ
	DWORD m_dwNumChild;		// 子の数

public:
	// 外部のデバイス等情報
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampleLinear;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer0;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer1;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer2;
	FbxNode* m_pFBXNode;						// FBXから姿勢行列を取り出す際に使うFBXポインタ
	DirectX::XMFLOAT4X4 m_mView;
	DirectX::XMFLOAT4X4 m_mProj;
	C_FbxLight* m_pLight;
	DirectX::XMFLOAT3* m_pCamera;
	// アニメーション関連
	DirectX::XMFLOAT4X4 m_mParentOrientation;	// 親の姿勢行列
	DirectX::XMFLOAT4X4 m_mFBXOrientation;		// 自分の姿勢行列 (親から見た相対姿勢)
	DirectX::XMFLOAT4X4 m_mFinalWorld;			// 最終的なワールド行列 (この姿勢でレンダリングする)
	TFbxMaterial* m_pMateUsr;

private:
	// メッシュ関連
	DWORD m_dwNumVert;
	DWORD m_dwNumFace;
	DWORD m_dwNumUV;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer** m_ppIndexBuffer;
	TFbxMaterial* m_pMaterial;
	DWORD m_dwNumMaterial;
	// ボーン
	int m_nNumSkin;
	TSkinInfo* m_pBoneTable;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBufferBone;

public:
	// メソッド
	HRESULT CreateFromFBX(FbxMesh* pFbxMesh);
	void RenderMesh(EByOpacity byOpacity = eNoAffect);
	void SetNewPoseMatrices(int nFrame);

private:
	HRESULT CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer);
	HRESULT ReadSkinInfo(FbxMesh* pFbxMesh, TFbxVertex* pvVB, TPolyTable* PolyTable);
	DirectX::XMFLOAT4X4 GetCurrentPoseMatrix(int nSkin, int nIndex);
};