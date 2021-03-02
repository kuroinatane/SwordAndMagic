//=======================================================================================
//
//	  DirectX11 Graphics �N���X [Graphics.cpp]
//
//=======================================================================================
#include "GraphicsManager.h"
#include "WindowManager.h"
#include "Scene.h"
#include "ImGuiHelper.h"
#include "ImNodes.h"
#include <Effekseer.h>

//-------- ���C�u�����̃����N
#pragma comment(lib, "d3d11")

/**
 * @brief �O���t�B�b�N���̏�����
 * @param[in] hWnd �E�B���h�E�n���h��
 * @param[in] nWidth �E�B���h�E��
 * @param[in] nWidth �E�B���h�E����
 * @param[in] bWindow �E�B���h�E���[�h
 */
HRESULT GraphicsManager::Init(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	HRESULT hr = S_OK;

	m_nWidth = (float)nWidth;
	m_nHeight = (float)nHeight;

	// �f�o�C�X�A�X���b�v�`�F�[���̍쐬
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

	// �o�b�N�o�b�t�@����
	hr = CreateBackBuffer();
	if (FAILED(hr)) {
		return hr;
	}

	// ���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;	// �J�����O����(���ʕ`��)
	m_pDevice->CreateRasterizerState(&rd, m_pRs[0].GetAddressOf());
	rd.CullMode = D3D11_CULL_FRONT;	// �O�ʃJ�����O(���ʕ`��)
	m_pDevice->CreateRasterizerState(&rd, m_pRs[1].GetAddressOf());
	rd.CullMode = D3D11_CULL_BACK;	// �w�ʃJ�����O(�\�ʕ`��)
	m_pDevice->CreateRasterizerState(&rd, m_pRs[2].GetAddressOf());
	SetCullMode(CULLMODE_CCW);

	// �u�����h �X�e�[�g����
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
	// �u�����h �X�e�[�g���� (�A���t�@ �u�����h�p)
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	m_pDevice->CreateBlendState(&BlendDesc, m_pBlendState[1].GetAddressOf());
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	m_pDevice->CreateBlendState(&BlendDesc, m_pBlendState[2].GetAddressOf());
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	m_pDevice->CreateBlendState(&BlendDesc, m_pBlendState[3].GetAddressOf());
	SetBlendState(BS_ALPHABLEND);

	// �[�x�X�e���V���X�e�[�g����
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pDevice->CreateDepthStencilState(&dsd, m_pDSS[0].GetAddressOf());
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	m_pDevice->CreateDepthStencilState(&dsd2, m_pDSS[1].GetAddressOf());


	// �|���S���\��������
	//hr = InitPolygon(g_pDevice);
	//if (FAILED(hr))
	//	return hr;

	// �f�o�b�O������\��������
	//hr = InitDebugProc();
	//if (FAILED(hr))
	//	return hr;

	//ImGui������
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
 * @brief �O���t�B�b�N���̃N���[���A�b�v
 */
void GraphicsManager::Uninit()
{
	//ImGui�I������
	imnodes::Shutdown();
	ImGui::DestroyContext();

	// �|���S���\���I������
	//UninitPolygon();
	if (m_pDeviceContext) {
		m_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}
}

/**
 * @brief �o�b�N�o�b�t�@����
 * @return ��������
 */
HRESULT GraphicsManager::CreateBackBuffer()
{
	HRESULT hr = S_OK;

	// �����_�[�^�[�Q�b�g�r���[����
	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, m_pRenderTargetView.GetAddressOf());
	SAFE_RELEASE(pBackBuffer);
	if (FAILED(hr)) {
		return hr;
	}

	// Z�o�b�t�@�p�e�N�X�`������
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

	// Z�o�b�t�@�^�[�Q�b�g�r���[����
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture.Get(), &dsvd, m_pDepthStencilView.GetAddressOf());
	if (FAILED(hr)) {
		return hr;
	}

	// �e�^�[�Q�b�g�r���[�������_�[�^�[�Q�b�g�ɐݒ�
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	// �r���[�|�[�g�ݒ�
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
 * @brief �X�V����
 */
void GraphicsManager::Update()
{

}

/**
 * @brief �`�揈��
 * @param[in] pScene �V�[�����
 */
void GraphicsManager::Draw(C_Scene* pScene)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	//float ClearColor[4] = { 1.0f, 0.254902f, 0.352941f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	SetZBuffer(true);
	SetZWrite(true);

	GraphicsManager::GetInstance().SetCullMode(GraphicsManager::CULLMODE_CW);

	// �V�[���̕`��
	if (pScene) {
		pScene->Draw();
	}
	GraphicsManager::GetInstance().SetCullMode(GraphicsManager::CULLMODE_NONE);

	// Z�o�b�t�@����
	SetZBuffer(false);

	if (pScene) {
		pScene->Draw2D();
	}

	ImGuiWindowManager::GetInstance().Render();

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pSwapChain->Present(m_uSyncInterval, 0);
}

/**
 * @brief �[�x�o�b�t�@�L����������
 * @param[in] bEnable �[�x�o�b�t�@�L��/����
 */
void GraphicsManager::SetZBuffer(bool bEnable)
{
	m_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : m_pDSS[1].Get(), 0);
}

/**
 * @brief �[�x�o�b�t�@�X�V�L����������
 * @param[in] bEnable �[�x�o�b�t�@�X�V�L��/����
 */
void GraphicsManager::SetZWrite(bool bEnable)
{
	m_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : m_pDSS[0].Get(), 0);
}

/**
 * @brief �u�����h�X�e�[�g�ݒ�
 * param[in] nBlendState �ݒ肷��u�����h�X�e�[�g
 */
void GraphicsManager::SetBlendState(int nBlendState)
{
	if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_pDeviceContext->OMSetBlendState(m_pBlendState[nBlendState].Get(), blendFactor, 0xffffffff);
	}
}

/**
 * @brief �J�����O�ݒ�
 * param[in] nCullMode �ݒ肷��J�����O���[�h
 */
void GraphicsManager::SetCullMode(int nCullMode)
{
	if (nCullMode >= 0 && nCullMode < MAX_CULLMODE) {
		m_pDeviceContext->RSSetState(m_pRs[nCullMode].Get());
	}
}
