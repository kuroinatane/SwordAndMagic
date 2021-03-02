/**
 * @file Mesh.h
 * @author MIURA DAIKI
 * @date 2020/12/15
 */
#pragma once
#include "main.h"
#include "Component.h"
#include "Utility.h"
#include <wrl/client.h>

 // メッシュからシェーダに渡す値
struct MESH_SHADER {
	DirectX::XMMATRIX	mWVP;		// ワールド×ビュー×射影行列(転置行列)
	DirectX::XMMATRIX	mW;			// ワールド行列(転置行列)
	DirectX::XMMATRIX	mTex;		// テクスチャ行列(転置行列)
};
struct MESH_SHADER_2 {
	DirectX::XMVECTOR	vEye;		// 視点座標
	// 光源
	DirectX::XMVECTOR	vLightDir;	// 光源方向
	DirectX::XMVECTOR	vLa;		// 光源色(アンビエント)
	DirectX::XMVECTOR	vLd;		// 光源色(ディフューズ)
	DirectX::XMVECTOR	vLs;		// 光源色(スペキュラ)
	// マテリアル
	DirectX::XMVECTOR	vAmbient;	// アンビエント色(+テクスチャ有無)
	DirectX::XMVECTOR	vDiffuse;	// ディフューズ色
	DirectX::XMVECTOR	vSpecular;	// スペキュラ色(+スペキュラ強度)
	DirectX::XMVECTOR	vEmissive;	// エミッシブ色
};

typedef struct {
	DirectX::XMFLOAT4	Diffuse;		// Diffuse color RGBA
	DirectX::XMFLOAT4	Ambient;		// Ambient color RGB
	DirectX::XMFLOAT4	Specular;		// Specular 'shininess'
	DirectX::XMFLOAT4	Emissive;		// Emissive color RGB
	float		Power;			// Sharpness if specular highlight
} MATERIAL;


class C_GameObject;
 /**
  * @class C_Mesh
  * @brief メッシュをプログラム側で生成して使うためのベースクラス。コンストラクタでMakeMeshVertexを呼び出して使用する。
  */
class C_Mesh : public C_Component
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_pVertexBuffer;	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_pIndexBuffer;	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_pConstantBuffer[3];	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			m_pSamplerState;		// テクスチャ サンプラ
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			m_pVertexShader;		// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_pInputLayout;			// 頂点フォーマット
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			m_pPixelShader;			// ピクセルシェーダ
	DirectX::XMFLOAT4X4 m_mtxTexture;					// テクスチャ マトリックス
	DirectX::XMFLOAT4X4 m_mtxWorld;					// ワールド行列
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_pTexture;		// テクスチャ
	C_GameObject* m_pObj;
protected:
	HRESULT MakeMeshVertex(VERTEX_3D* vtxWk, int* idxWk); //子クラスから呼び出し専用
	void SetTexture(const TCHAR* szFileName);
	void SetTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture);
	int m_nNumVertex;							// 総頂点数	
	int m_nNumIndex;							// 総インデックス数
	MATERIAL						m_material;
public:
	C_Mesh(C_GameObject* obj);
	~C_Mesh();
	HRESULT Init(void);
	void Uninit(void);
	void Update();
	void Draw();
	
};

