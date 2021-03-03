#include "Polygon.h"
#include "Shader.h"
#include "GraphicsManager.h"
#include "SceneController.h"
#include "GameObj.h"
#include "Texture.h"

//=============================================================================
// 初期化処理
//=============================================================================
PolygonComponent::PolygonComponent(const wchar_t* szTexture, C_GameObject* obj)
{
	m_pObj = obj;
	SetPolygonTexture(szTexture);

	// シェーダ初期化
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	LoadShader("Vertex2D", "Pixel2D",
	m_pVertexShader.GetAddressOf(), m_pInputLayout.GetAddressOf(), m_pPixelShader.GetAddressOf(), layout, _countof(layout));
	

	// 定数バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(POLYGON_SHADER);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	GraphicsManager::GetInstance().GetDevice()->CreateBuffer(&bd, nullptr, m_pConstantBuffer.GetAddressOf());

	// テクスチャ サンプラ生成
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	GraphicsManager::GetInstance().GetDevice()->CreateSamplerState(&sd, m_pSamplerState.GetAddressOf());

	// 変換行列初期化
	XMStoreFloat4x4(&m_mTex, DirectX::XMMatrixIdentity());
	m_mTex._44 = 0.0f;

	m_colPolygon = DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f);

	m_posTexFrame = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_sizTexFrame = DirectX::XMFLOAT2(1.0f,1.0f);

	MakeVertexPolygon();

}

//=============================================================================
// 終了処理
//=============================================================================
PolygonComponent::~PolygonComponent()
{

}

//=============================================================================
// 更新処理
//=============================================================================
void PolygonComponent::Update(void)
{
	// (何もしない)
}

//=============================================================================
// 描画処理
//=============================================================================
void PolygonComponent::Draw()
{
	// 拡縮
	DirectX::XMFLOAT4X4 mWorld;
	DirectX::XMMATRIX mtxWorld;

	mWorld = m_pObj->transform.getWorldMatrix();

	mtxWorld = DirectX::XMLoadFloat4x4(&mWorld);


	if (m_pTexture) {
		// 拡縮
		mtxWorld = DirectX::XMMatrixScaling(m_sizTexFrame.x, m_sizTexFrame.y, 1.0f);
		// 移動
		mtxWorld *= DirectX::XMMatrixTranslation(m_posTexFrame.x, m_posTexFrame.y, 0.0f);
		// テクスチャ マトリックスに設定
		XMStoreFloat4x4(&m_mTex, mtxWorld);
	}
	else {
		// テクスチャ無し
		m_mTex._44 = 0.0f;
	}

	// 頂点バッファ更新
	SetVertexPolygon();

	GraphicsManager::GetInstance().GetDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	GraphicsManager::GetInstance().GetDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	GraphicsManager::GetInstance().GetDeviceContext()->IASetInputLayout(m_pInputLayout.Get());

	UINT stride = sizeof(VERTEX_2D);
	UINT offset = 0;
	GraphicsManager::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	GraphicsManager::GetInstance().GetDeviceContext()->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
	GraphicsManager::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());

	POLYGON_SHADER cb;
	cb.mProj = DirectX::XMMatrixTranspose(XMLoadFloat4x4(&(SceneController::GetInstance().GetCurrentScene()->GetCamera()->GetProjMatrix())));
	cb.mView = DirectX::XMMatrixTranspose(XMLoadFloat4x4(&(SceneController::GetInstance().GetCurrentScene()->GetCamera()->GetViewMatrix())));
	cb.mWorld = DirectX::XMMatrixTranspose(XMLoadFloat4x4(&mWorld));
	cb.mTex = DirectX::XMMatrixTranspose(XMLoadFloat4x4(&m_mTex));
	GraphicsManager::GetInstance().GetDeviceContext()->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
	GraphicsManager::GetInstance().GetDeviceContext()->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	GraphicsManager::GetInstance().GetDeviceContext()->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

	// プリミティブ形状をセット
	GraphicsManager::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴンの描画
	GraphicsManager::GetInstance().GetDeviceContext()->Draw(4, 0);
}

//=============================================================================
// 頂点の作成
//=============================================================================
void PolygonComponent::MakeVertexPolygon()
{
	// 頂点座標の設定
	m_vertexWk[0].vtx = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
	m_vertexWk[1].vtx = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
	m_vertexWk[2].vtx = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f);
	m_vertexWk[3].vtx = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);

	// 拡散反射光の設定
	m_vertexWk[0].diffuse = m_colPolygon;
	m_vertexWk[1].diffuse = m_colPolygon;
	m_vertexWk[2].diffuse = m_colPolygon;
	m_vertexWk[3].diffuse = m_colPolygon;

	// テクスチャ座標の設定
	m_vertexWk[0].tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_vertexWk[1].tex = DirectX::XMFLOAT2(1.0f, 0.0f);
	m_vertexWk[2].tex = DirectX::XMFLOAT2(0.0f, 1.0f);
	m_vertexWk[3].tex = DirectX::XMFLOAT2(1.0f, 1.0f);

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(m_vertexWk);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &m_vertexWk[0];

	GraphicsManager::GetInstance().GetDevice()->CreateBuffer(&vbd, &initData, m_pVertexBuffer.GetAddressOf());
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void PolygonComponent::SetVertexPolygon()
{
	//頂点バッファの中身を埋める
	ID3D11DeviceContext* pDeviceContext = GraphicsManager::GetInstance().GetDeviceContext();
	HRESULT hr = S_OK;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3D11_MAPPED_SUBRESOURCE msr;
	hr = pDeviceContext->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (SUCCEEDED(hr)) {
		// 拡散反射光の設定
		m_vertexWk[0].diffuse = m_colPolygon;
		m_vertexWk[1].diffuse = m_colPolygon;
		m_vertexWk[2].diffuse = m_colPolygon;
		m_vertexWk[3].diffuse = m_colPolygon;
		// 頂点データを上書き
		memcpy_s(msr.pData, sizeof(m_vertexWk), m_vertexWk, sizeof(m_vertexWk));
		// 頂点データをアンロックする
		pDeviceContext->Unmap(m_pVertexBuffer.Get(), 0);
		// フラグをクリア
	}
}

//=============================================================================
// テクスチャの設定
//=============================================================================
void PolygonComponent::SetPolygonTexture(const wchar_t* szTexture)
{
	HRESULT hr = CreateTextureFromFile(GraphicsManager::GetInstance().GetDevice(), szTexture, m_pTexture.GetAddressOf());
	m_mTex._44 = (m_pTexture.Get()) ? 1.0f : 0.0f;

}

//=============================================================================
// 左上テクスチャ座標の設定 (0.0≦fU＜1.0, 0.0≦fV＜1.0)
//=============================================================================
void PolygonComponent::SetPolygonUV(float fU, float fV)
{
	m_posTexFrame.x = fU;
	m_posTexFrame.y = fV;
}

//=============================================================================
// テクスチャフレームサイズの設定 (0.0＜fWidth≦1.0, 0.0＜fHeight≦1.0)
//=============================================================================
void PolygonComponent::SetPolygonFrameSize(float fWidth, float fHeight)
{
	m_sizTexFrame.x = fWidth;
	m_sizTexFrame.y = fHeight;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void PolygonComponent::SetPolygonColor(float fRed, float fGreen, float fBlue)
{
	if (fRed != m_colPolygon.x || fGreen != m_colPolygon.y || fBlue != m_colPolygon.z) {
		m_colPolygon.x = fRed;
		m_colPolygon.y = fGreen;
		m_colPolygon.z = fBlue;
		m_bInvalidate = true;
	}
}

//=============================================================================
// 不透明度の設定
//=============================================================================
void PolygonComponent::SetPolygonAlpha(float fAlpha)
{
	if (fAlpha != m_colPolygon.w) {
		m_colPolygon.w = fAlpha;
		m_bInvalidate = true;
	}
}
