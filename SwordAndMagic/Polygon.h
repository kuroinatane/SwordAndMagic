/**
 * @file Curtain.h
 * @author MIURA DAIKI
 * @date 2020/12/15
 */
#pragma once

#include "main.h"
#include "Component.h"
#include "Utility.h"
#include <wrl/client.h>

 // シェーダに渡す値
struct POLYGON_SHADER {
	DirectX::XMMATRIX	mWorld;		// ワールド変換行列(転置行列)
	DirectX::XMMATRIX	mView;		// ビュー変換行列(転置行列)
	DirectX::XMMATRIX	mProj;		// 射影変換行列(転置行列)
	DirectX::XMMATRIX	mTex;		// テクスチャ変換行列(転置行列)
};

class C_GameObject;
 /**
  * @class PolygonComponent
  * @brief 2Dポリゴン描画用のクラス。UI用。
  */
class PolygonComponent : public C_Component {
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pTexture;				// テクスチャへのポインタ
	VERTEX_2D					m_vertexWk[4];	// 頂点情報格納ワーク

	DirectX::XMFLOAT4						m_colPolygon;			// ポリゴンの頂点カラー
	bool							m_bInvalidate;			// 頂点データ更新フラグ

	DirectX::XMFLOAT2						m_posTexFrame;			// UV座標
	DirectX::XMFLOAT2						m_sizTexFrame;			// テクスチャ抽出サイズ

	Microsoft::WRL::ComPtr <ID3D11Buffer>			m_pConstantBuffer;		// 定数バッファ
	Microsoft::WRL::ComPtr <ID3D11Buffer>				m_pVertexBuffer;		// 頂点バッファ
	Microsoft::WRL::ComPtr <ID3D11SamplerState>			m_pSamplerState;		// テクスチャ サンプラ
	Microsoft::WRL::ComPtr <ID3D11VertexShader>			m_pVertexShader;		// 頂点シェーダ
	Microsoft::WRL::ComPtr < ID3D11InputLayout>			m_pInputLayout;			// 頂点フォーマット
	Microsoft::WRL::ComPtr < ID3D11PixelShader>			m_pPixelShader;			// ピクセルシェーダ

	DirectX::XMFLOAT4X4					m_mTex;					// テクスチャ変換行列
	C_GameObject*					m_pObj;					// オブジェクトへのポインタ
public:
	PolygonComponent(const wchar_t* szTexture, C_GameObject* obj);
	~PolygonComponent();
	void Update(void);
	void Draw();
	void SetPolygonTexture(const wchar_t* szTexture);
	void SetPolygonUV(float fU, float fV);
	void SetPolygonFrameSize(float fWidth, float fHeight);
	void SetPolygonColor(float fRed, float fGreen, float fBlue);
	void SetPolygonAlpha(float fAlpha);
	void MakeVertexPolygon();
	void SetVertexPolygon();
};
