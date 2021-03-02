//=======================================================================================
//
//	  DirectX11 Graphics クラス [Graphics.cpp]
//
//=======================================================================================
#include "GraphicsManager.h"
#include "WindowManager.h"
#include "Scene.h"
#include "ImGuiHelper.h"
#include "ImNodes.h"
#include <Effekseer.h>

//-------- ライブラリのリンク
#pragma comment(lib, "d3d11")

/**
 * @brief グラフィック環境の初期化
 * @param[in] hWnd ウィンドウハンドル
 * @param[in] nWidth ウィンドウ幅
 * @param[in] nWidth ウィンドウ高さ
 * @param[in] bWindow ウィンドウモード
 */
HRESULT GraphicsManager::Init(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	HRESULT hr = S_OK;

	m_nWidth = (float)nWidth;
	m_nHeight = (float)nHeight;

	// デバイス、スワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = (UINT)nWidth;
	scd.BufferDesc.Height = (UINT)nHeight;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = bWindow;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &scd,
		m_pSwapChain.GetAddressOf(), m_pDevice.GetAddressOf(), nullptr, m_pDeviceContext.GetAddressOf());
	if (FAILED(hr)) {
		return hr;
	}

	// バックバッファ生成
	hr = CreateBackBuffer();
	if (FAILED(hr)) {
		return hr;
	}

	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;	// カリング無し(両面描画)
	m_pDevice->CreateRasterizerState(&rd, m_pRs[0].GetAddressOf());
	rd.CullMode = D3D11_CULL_FRONT;	// 前面カリング(裏面描画)
	m_pDevice->CreateRasterizerState(&rd, m_pRs[1].GetAddressOf());
	rd.CullMode = D3D11_CULL_BACK;	// 背面カリング(表面描画)
	m_pDevice->CreateRasterizerState(&rd, m_pRs[2].GetAddressOf());
	SetCullMode(CULLMODE_CCW);

	// ブレンド ステート生成
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pDevice->CreateBlendState(&BlendDesc, m_pBlendState[0].GetAddressOf());
	// ブレンド ステート生成 (アルファ ブレンド用)
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	m_pDevice->CreateBlendState(&BlendDesc, m_pBlendState[1].GetAddressOf());
	// ブレンド ステート生成 (加算合成用)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	m_pDevice->CreateBlendState(&BlendDesc, m_pBlendState[2].GetAddressOf());
	// ブレンド ステート生成 (減算合成用)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	m_pDevice->CreateBlendState(&BlendDesc, m_pBlendState[3].GetAddressOf());
	SetBlendState(BS_ALPHABLEND);

	// 深度ステンシルステート生成
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pDevice->CreateDepthStencilState(&dsd, m_pDSS[0].GetAddressOf());
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	m_pDevice->CreateDepthStencilState(&dsd2, m_pDSS[1].GetAddressOf());


	// ポリゴン表示初期化
	//hr = InitPolygon(g_pDevice);
	//if (FAILED(hr))
	//	return hr;

	// デバッグ文字列表示初期化
	//hr = InitDebugProc();
	//if (FAILED(hr))
	//	return hr;

	//ImGui初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(WindowManager::GetInstance().GetMainHwnd());
	ImGui_ImplDX11_Init(m_pDevice.Get(), m_pDeviceContext.Get());

	imnodes::Initialize();

	return hr;
}

/**
 * @brief グラフィック環境のクリーンアップ
 */
void GraphicsManager::Uninit()
{
	//ImGui終了処理
	imnodes::Shutdown();
	ImGui::DestroyContext();

	// ポリゴン表示終了処理
	//UninitPolygon();
	if (m_pDeviceContext) {
		m_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}
}

/**
 * @brief バックバッファ生成
 * @return 処理結果
 */
HRESULT GraphicsManager::CreateBackBuffer()
{
	HRESULT hr = S_OK;

	// レンダーターゲットビュー生成
	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, m_pRenderTargetView.GetAddressOf());
	SAFE_RELEASE(pBackBuffer);
	if (FAILED(hr)) {
		return hr;
	}

	// Zバッファ用テクスチャ生成
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = (UINT)m_nWidth;
	td.Height = (UINT)m_nHeight;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = m_pDevice->CreateTexture2D(&td, nullptr, m_pDepthStencilTexture.GetAddressOf());
	if (FAILED(hr)) {
		return hr;
	}

	// Zバッファターゲットビュー生成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture.Get(), &dsvd, m_pDepthStencilView.GetAddressOf());
	if (FAILED(hr)) {
		return hr;
	}

	// 各ターゲットビューをレンダーターゲットに設定
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (float)m_nWidth;
	vp.Height = (float)m_nHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);

	return hr;
}

/**
 * @brief 更新処理
 */
void GraphicsManager::Update()
{

}

/**
 * @brief 描画処理
 * @param[in] pScene シーン情報
 */
void GraphicsManager::Draw(C_Scene* pScene)
{
	// バックバッファ＆Ｚバッファのクリア
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	//float ClearColor[4] = { 1.0f, 0.254902f, 0.352941f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	SetZBuffer(true);
	SetZWrite(true);

	GraphicsManager::GetInstance().SetCullMode(GraphicsManager::CULLMODE_CW);

	// シーンの描画
	if (pScene) {
		pScene->Draw();
	}
	GraphicsManager::GetInstance().SetCullMode(GraphicsManager::CULLMODE_NONE);

	// Zバッファ無効
	SetZBuffer(false);

	if (pScene) {
		pScene->Draw2D();
	}

	ImGuiWindowManager::GetInstance().Render();

	// バックバッファとフロントバッファの入れ替え
	m_pSwapChain->Present(m_uSyncInterval, 0);
}

/**
 * @brief 深度バッファ有効無効制御
 * @param[in] bEnable 深度バッファ有効/無効
 */
void GraphicsManager::SetZBuffer(bool bEnable)
{
	m_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : m_pDSS[1].Get(), 0);
}

/**
 * @brief 深度バッファ更新有効無効制御
 * @param[in] bEnable 深度バッファ更新有効/無効
 */
void GraphicsManager::SetZWrite(bool bEnable)
{
	m_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : m_pDSS[0].Get(), 0);
}

/**
 * @brief ブレンドステート設定
 * param[in] nBlendState 設定するブレンドステート
 */
void GraphicsManager::SetBlendState(int nBlendState)
{
	if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_pDeviceContext->OMSetBlendState(m_pBlendState[nBlendState].Get(), blendFactor, 0xffffffff);
	}
}

/**
 * @brief カリング設定
 * param[in] nCullMode 設定するカリングモード
 */
void GraphicsManager::SetCullMode(int nCullMode)
{
	if (nCullMode >= 0 && nCullMode < MAX_CULLMODE) {
		m_pDeviceContext->RSSetState(m_pRs[nCullMode].Get());
	}
}
