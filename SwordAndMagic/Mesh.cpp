#include "Mesh.h"
#include "GraphicsManager.h"
#include "Shader.h"
#include "GameObj.h"
#include "FbxLight.h"
#include "Texture.h"
#include "SceneController.h"


C_Mesh::C_Mesh(C_GameObject* obj) {
	m_pObj = obj;
	Init();
}

C_Mesh::~C_Mesh() {

}

void C_Mesh::SetTexture(const TCHAR* szFileName) {
	CreateTextureFromFile(GraphicsManager::GetInstance().GetDevice(),						// デバイスへのポインタ
		szFileName,				// ファイルの名前
		m_pTexture.GetAddressOf());		// 読み込むメモリー
	DirectX::XMStoreFloat4x4(&m_mtxTexture, DirectX::XMMatrixIdentity());
}

void C_Mesh::SetTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture) {
	m_pTexture.Reset();
	m_pTexture = pTexture;
	DirectX::XMStoreFloat4x4(&m_mtxTexture, DirectX::XMMatrixIdentity());
}

HRESULT C_Mesh::Init(void) {
	ID3D11Device* pDevice = GraphicsManager::GetInstance().GetDevice();
	HRESULT hr;
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hr = LoadShader("Vertex", "Pixel",
		m_pVertexShader.GetAddressOf(), m_pInputLayout.GetAddressOf(), m_pPixelShader.GetAddressOf(), layout, _countof(layout));
	if (FAILED(hr)) {
		return hr;
	}

	// 定数バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MESH_SHADER);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pDevice->CreateBuffer(&bd, nullptr, m_pConstantBuffer[0].GetAddressOf());
	if (FAILED(hr)) return hr;
	bd.ByteWidth = sizeof(MESH_SHADER_2);
	hr = pDevice->CreateBuffer(&bd, nullptr, m_pConstantBuffer[1].GetAddressOf());
	if (FAILED(hr)) return hr;

	// テクスチャ サンプラ生成
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = pDevice->CreateSamplerState(&sd, m_pSamplerState.GetAddressOf());
	if (FAILED(hr)) {
		return hr;
	}

	// マテリアルの初期設定
	m_material.Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_material.Ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_material.Specular = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	m_material.Power = 0.0f;
	m_material.Emissive = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	return hr;
}

void C_Mesh::Uninit(void) {

}
void C_Mesh::Update() {
	// ワールドマトリックスの設定
	m_mtxWorld = m_pObj->transform.getWorldMatrix();
}

//オーバーライド
void C_Mesh::Draw() {
	ID3D11DeviceContext* pDeviceContext = GraphicsManager::GetInstance().GetDeviceContext();

	// シェーダ設定
	pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	pDeviceContext->IASetInputLayout(m_pInputLayout.Get());

	// 頂点バッファをセット
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	// インデックスバッファをセット
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
	pDeviceContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());

	MESH_SHADER cb;
	C_Camera* camera = SceneController::GetInstance().GetCurrentScene()->GetCamera();
	C_FbxLight* fbxlight = SceneController::GetInstance().GetCurrentScene()->GetLight();
	DirectX::XMMATRIX mtxWorld = DirectX::XMLoadFloat4x4(&m_mtxWorld);
	cb.mWVP = DirectX::XMMatrixTranspose(mtxWorld *
		DirectX::XMLoadFloat4x4(&camera->GetViewMatrix()) * DirectX::XMLoadFloat4x4(&camera->GetProjMatrix()));
	cb.mW = DirectX::XMMatrixTranspose(mtxWorld);
	cb.mTex = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_mtxTexture));
	pDeviceContext->UpdateSubresource(m_pConstantBuffer[0].Get(), 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer[0].GetAddressOf());
	MESH_SHADER_2 cb2;
	cb2.vEye = DirectX::XMLoadFloat3(&camera->GetPosition());
	C_FbxLight* light = fbxlight;
	cb2.vLightDir = DirectX::XMVectorSet(light->m_direction.x, light->m_direction.y, light->m_direction.z, 0.f);
	cb2.vLa = DirectX::XMLoadFloat4(&light->m_ambient);
	cb2.vLd = DirectX::XMLoadFloat4(&light->m_diffuse);
	cb2.vLs = DirectX::XMLoadFloat4(&light->m_specular);
	MATERIAL* pMaterial = &m_material;
	cb2.vDiffuse = DirectX::XMLoadFloat4(&pMaterial->Diffuse);
	cb2.vAmbient = DirectX::XMVectorSet(pMaterial->Ambient.x, pMaterial->Ambient.y, pMaterial->Ambient.z,
		(m_pTexture != nullptr) ? 1.f : 0.f);
	cb2.vSpecular = DirectX::XMVectorSet(pMaterial->Specular.x, pMaterial->Specular.y, pMaterial->Specular.z, pMaterial->Power);
	cb2.vEmissive = DirectX::XMLoadFloat4(&pMaterial->Emissive);
	pDeviceContext->UpdateSubresource(m_pConstantBuffer[1].Get(), 0, nullptr, &cb2, 0, 0);
	pDeviceContext->PSSetConstantBuffers(1, 1, m_pConstantBuffer[1].GetAddressOf());

	//// プリミティブ形状をセット(今の所固定)
	//static const D3D11_PRIMITIVE_TOPOLOGY pt[] = {
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,	// 0なら三角形ストリップ
	//	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	//	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	//	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	//};
	//pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ポリゴンの描画
	pDeviceContext->DrawIndexed(m_nNumIndex, 0, 0);
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT C_Mesh::MakeMeshVertex(VERTEX_3D* vtxWk, int* idxWk)
{
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VERTEX_3D) * m_nNumVertex;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vtxWk;
	HRESULT hr = GraphicsManager::GetInstance().GetDevice()->CreateBuffer(&vbd, &initData, m_pVertexBuffer.GetAddressOf());
	if (FAILED(hr)) {
		return hr;
	}

	CD3D11_BUFFER_DESC ibd(m_nNumIndex * sizeof(int), D3D11_BIND_INDEX_BUFFER);
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = idxWk;
	hr = GraphicsManager::GetInstance().GetDevice()->CreateBuffer(&ibd, &initData, m_pIndexBuffer.GetAddressOf());

	return hr;
}
