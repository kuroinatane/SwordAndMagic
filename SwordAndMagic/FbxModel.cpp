#include "FbxModel.h"
#include "GraphicsManager.h"
#include "Shader.h"
#include "GameObj.h"
#include "SceneController.h"
#include "ShaderLight.h"

using namespace DirectX;
//---------------------------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------------------------
C_FbxModel::C_FbxModel(LPCSTR pszFileName, LPCSTR vtxShader, LPCSTR pixShader)
{
	m_pRootMesh = nullptr;
	m_pSampleLinear = nullptr;
	m_pConstantBuffer0 = nullptr;
	m_pConstantBuffer1 = nullptr;
	m_pConstantBuffer2 = nullptr;
	m_pVertexLayout = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pVertexSubLayout = nullptr;
	m_pVertexSubShader = nullptr;
	m_pPixelSubShader = nullptr;
	XMStoreFloat4x4(&m_mView, XMMatrixIdentity());
	m_mProj = m_mView;
	m_mWorld = m_mView;
	m_mFinalWorld = m_mView;
	m_pSdkManager = nullptr;
	m_pImporter = nullptr;
	m_pScene = nullptr;
	m_vCamera = XMFLOAT3(0, 0, -1);
	m_nAnimFrame = 0;
	m_nAnimStack = 0;
	m_pMaterial = nullptr;
	m_light = new C_FbxLight();
	Init(pszFileName, vtxShader, pixShader);
}

//---------------------------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------------------------
C_FbxModel::~C_FbxModel()
{
	DestroyMesh(m_pRootMesh);
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexSubLayout);
	SAFE_RELEASE(m_pVertexSubShader);
	SAFE_RELEASE(m_pPixelSubShader);
	if (m_pSdkManager) m_pSdkManager->Destroy();
}

//---------------------------------------------------------------------------------------
// �K�w���b�V���ǂݍ���
//---------------------------------------------------------------------------------------
HRESULT C_FbxModel::LoadRecursive(FbxNode* pNode, C_FbxMesh* pFBXMesh)
{
	HRESULT hr = S_OK;

	pFBXMesh->m_pConstantBuffer0 = m_pConstantBuffer0;
	pFBXMesh->m_pConstantBuffer1 = m_pConstantBuffer1;
	pFBXMesh->m_pConstantBuffer2 = m_pConstantBuffer2;
	pFBXMesh->m_pSampleLinear = m_pSampleLinear;
	pFBXMesh->m_pFBXNode = pNode;

	FbxNodeAttribute *pAttr = pNode->GetNodeAttribute();
	if (pAttr) {
		FbxNodeAttribute::EType type = pAttr->GetAttributeType();
		switch (type) {
		case FbxNodeAttribute::eMesh:
			// ���b�V���쐬
			hr = pFBXMesh->CreateFromFBX(pNode->GetMesh());
			if (FAILED(hr)) {
				MessageBoxW(0, L"���b�V�������G���[", L"CFbxMesh::CreateFromFBX", MB_OK);
				return hr;
			}
			break;
		}
	}

	// �q��������A�q�ōċA
	int iNumChild = pNode->GetChildCount();
	pFBXMesh->m_dwNumChild = iNumChild;
	if (iNumChild > 0) {
		pFBXMesh->m_ppChild = new C_FbxMesh*[iNumChild];
		for (int i = 0; i < iNumChild; ++i) {
			pFBXMesh->m_ppChild[i] = new C_FbxMesh;
		}
		for (int i = 0; i < iNumChild; ++i) {
			FbxNode* ChildNode = pNode->GetChild(i);
			hr = LoadRecursive(ChildNode, pFBXMesh->m_ppChild[i]);
			if (FAILED(hr)) {
				return hr;
			}
		}
	}
	else {
		pFBXMesh->m_ppChild = nullptr;
	}
	return hr;
}

//---------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------
HRESULT C_FbxModel::Init(LPCSTR pszFileName, LPCSTR vtxShader, LPCSTR pixShader)
{

	HRESULT hr = InitShader(vtxShader, pixShader);
	if (FAILED(hr)) {
		MessageBoxW(0, L"���b�V���p�V�F�[�_�쐬���s", nullptr, MB_OK);
		return hr;
	}

	// �e�N�X�`���p�T���v���쐬
	CD3D11_DEFAULT def;
	CD3D11_SAMPLER_DESC sd(def);
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = GraphicsManager::GetInstance().GetDevice()->CreateSamplerState(&sd, &m_pSampleLinear);
	if (FAILED(hr)) {
		MessageBoxW(0, L"�e�N�X�`���p�T���v���쐬���s", nullptr, MB_OK);
		return hr;
	}

	char* pszFName = nullptr;
	size_t uFName = 0;
	FbxAnsiToUTF8(pszFileName, pszFName, &uFName);
	hr = CreateFromFBX(pszFName);
	if (pszFName) FbxFree(pszFName);
	if (FAILED(hr)) {
		MessageBoxW(0, L"���b�V���쐬���s", nullptr, MB_OK);
		return hr;
	}
	SetAnimStack(0);
	SetAnimFrame(0);
	return hr;
}

//---------------------------------------------------------------------------------------
// �V�F�[�_�ǂݍ���
//---------------------------------------------------------------------------------------
HRESULT C_FbxModel::InitShader(LPCSTR vtxShader, LPCSTR pixShader)
{
	HRESULT hr = S_OK;
	LPCSTR vS, pS;
	if (vtxShader != NULL) {
		vS = vtxShader;
	}
	else {
		vS = "FbxModelVertex";
	}
	if (pixShader != NULL) {
		pS = pixShader;
	}
	else {
		pS = "FbxModelPixel";
	}
	hr = LoadShader(vS, pS,
		&m_pVertexShader, &m_pVertexLayout, &m_pPixelShader);
	if (FAILED(hr)) {
		MessageBoxW(0, L"hlsl�ǂݍ��ݎ��s", nullptr, MB_OK);
		return hr;
	}
	

	// �R���X�^���g�o�b�t�@�쐬 �ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	ZeroMemory(&cb, sizeof(cb));
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_GLOBAL);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	hr = GraphicsManager::GetInstance().GetDevice()->CreateBuffer(&cb, nullptr, &m_pConstantBuffer0);
	if (FAILED(hr)) {
		return hr;
	}

	// �R���X�^���g�o�b�t�@�쐬 �}�e���A���n���p
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_MATERIAL);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	hr = GraphicsManager::GetInstance().GetDevice()->CreateBuffer(&cb, nullptr, &m_pConstantBuffer1);

	// �R���X�^���g�o�b�t�@�쐬 �}�e���A���n���p
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(LightInfo[100]) ;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	hr = GraphicsManager::GetInstance().GetDevice()->CreateBuffer(&cb, nullptr, &m_pConstantBuffer2);

	return hr;
}

//---------------------------------------------------------------------------------------
// FBX������
//---------------------------------------------------------------------------------------
HRESULT C_FbxModel::InitFBX(LPCSTR szFileName)
{
	m_pSdkManager = FbxManager::Create();
	m_pImporter = FbxImporter::Create(m_pSdkManager, "MyImporter");
	m_pImporter->Initialize(szFileName);
	m_pScene = FbxScene::Create(m_pSdkManager, "MyScene");
	m_pImporter->Import(m_pScene);

	// DirectX�n
	//FbxAxisSystem::DirectX.ConvertScene(m_pScene);

	// �O�p�`��
	FbxGeometryConverter lGeomConverter(m_pSdkManager);
	lGeomConverter.Triangulate(m_pScene, true);

	// �A�j���[�V�������
	m_pScene->FillAnimStackNameArray(m_strAnimStackName);
	if (m_strAnimStackName.GetCount() > 0) {
		SetAnimStack(0);
	}

	return S_OK;
}

//---------------------------------------------------------------------------------------
// FBX���烁�b�V�����쐬
//---------------------------------------------------------------------------------------
HRESULT C_FbxModel::CreateFromFBX(LPCSTR szFileName)
{
	// FBX���[�_������
	HRESULT hr = InitFBX(szFileName);
	if (FAILED(hr)) {
		MessageBoxW(0, L"FBX���[�_���������s", nullptr, MB_OK);
		return hr;
	}

	char szDir[_MAX_DIR];
	char szCurrentDir[_MAX_PATH];
	// �t�H���_���𒊏o
	_splitpath_s(szFileName, nullptr, 0, szDir, _countof(szDir),
		nullptr, 0, nullptr, 0);
	// �J�����g�f�B���N�g����ύX
	if (szDir[0]) {
		GetCurrentDirectoryA(_MAX_PATH, szCurrentDir);
		SetCurrentDirectoryA(szDir);
	}

	FbxNode* pNode = m_pScene->GetRootNode();
	m_pRootMesh = new C_FbxMesh;
	hr = LoadRecursive(pNode, m_pRootMesh);

	// �J�����g�f�B���N�g�������ɖ߂�
	if (szDir[0])
		SetCurrentDirectoryA(szCurrentDir);

	return hr;
}

//---------------------------------------------------------------------------------------
// �����_�����O
//---------------------------------------------------------------------------------------
void C_FbxModel::Render(DirectX::XMFLOAT4X4 mWorld, EByOpacity opacity)
{
	m_mWorld = mWorld;
	//XMStoreFloat4x4(&m_mWorld,DirectX::XMMatrixIdentity());
	m_mView = SceneController::GetInstance().GetCurrentScene()->GetCamera()->GetViewMatrix();
	m_mProj = SceneController::GetInstance().GetCurrentScene()->GetCamera()->GetProjMatrix();
	// �g�p����V�F�[�_�[�̓o�^	
	GraphicsManager::GetInstance().GetDeviceContext()->VSSetShader(m_pVertexShader, nullptr, 0);
	GraphicsManager::GetInstance().GetDeviceContext()->PSSetShader(m_pPixelShader, nullptr, 0);
	// ���_�C���v�b�g���C�A�E�g���Z�b�g
	GraphicsManager::GetInstance().GetDeviceContext()->IASetInputLayout(m_pVertexLayout);

	//�Ƃ肠�����S���`��
	RecursiveRender(m_pRootMesh, opacity);

	//�A�E�g���C���V�F�[�_�[�p�A�J�����O�؂�ւ��`��
	if (m_bOutline) {
		GraphicsManager::GetInstance().GetDeviceContext()->VSSetShader(m_pVertexSubShader, nullptr, 0);
		GraphicsManager::GetInstance().GetDeviceContext()->PSSetShader(m_pPixelSubShader, nullptr, 0);
		GraphicsManager::GetInstance().GetDeviceContext()->IASetInputLayout(m_pVertexSubLayout);
		GraphicsManager::GetInstance().SetCullMode(GraphicsManager::CULLMODE_CCW);
		RecursiveRender(m_pRootMesh, opacity);
		GraphicsManager::GetInstance().SetCullMode(GraphicsManager::CULLMODE_CW);
	}
}

void C_FbxModel::Update() {
	if (m_nAnimFrame < GetMaxAnimFrame()) {
		SetAnimFrame(m_nAnimFrame+1);
	}
	else {
		SetAnimFrame(0);
	}
}

//---------------------------------------------------------------------------------------
// ���b�V���`��
//---------------------------------------------------------------------------------------
void C_FbxModel::RenderMesh(C_FbxMesh* pMesh, EByOpacity byOpacity)
{
	pMesh->m_mView = m_mView;
	pMesh->m_mProj = m_mProj;
	pMesh->m_pLight = m_light;
	pMesh->m_pCamera = &m_vCamera;
	pMesh->m_pMateUsr = m_pMaterial;
	pMesh->RenderMesh(byOpacity);
}

//---------------------------------------------------------------------------------------
// �K�w�A�j���[�V�����Đ�
//---------------------------------------------------------------------------------------
void C_FbxModel::RecursiveRender(C_FbxMesh* pMesh, EByOpacity byOpacity)
{
	// ���̏u�Ԃ́A�����̎p���s��𓾂�
	FbxTime time;
	time.SetTime(0, 0, 0, m_nAnimFrame, 0, 0, FbxTime::eFrames60);//���̃t���[���ɑ΂��鎞��
	FbxAMatrix mCurrent = pMesh->m_pFBXNode->EvaluateGlobalTransform(time);

	// �s��R�s�[ (DirectX �� FBX)
	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {
			pMesh->m_mFBXOrientation(y, x) =
				static_cast<float>(mCurrent.Get(y, x));
		}
	}

	// �e�̎p���s��ƍ���
	pMesh->m_mParentOrientation = m_mWorld;
	XMStoreFloat4x4(&pMesh->m_mFinalWorld,
		XMLoadFloat4x4(&pMesh->m_mFBXOrientation) * XMLoadFloat4x4(&pMesh->m_mParentOrientation));
	RenderMesh(pMesh, byOpacity);
	for (DWORD i = 0; i < pMesh->m_dwNumChild; ++i) {
		RecursiveRender(pMesh->m_ppChild[i], byOpacity);
	}
}

//---------------------------------------------------------------------------------------
// �S���b�V���j��
//---------------------------------------------------------------------------------------
void C_FbxModel::DestroyMesh(C_FbxMesh* pMesh)
{
	if (!pMesh) return;
	for (DWORD i = 0; i < pMesh->m_dwNumChild; ++i) {
		if (pMesh->m_ppChild[i])
			DestroyMesh(pMesh->m_ppChild[i]);
	}
	delete pMesh;
}

//---------------------------------------------------------------------------------------
// �A�j���[�V���� �t���[���X�V
//---------------------------------------------------------------------------------------
void C_FbxModel::SetAnimFrame(int nFrame)
{
	m_nAnimFrame = nFrame;
	SetNewPoseMatrices(m_pRootMesh, nFrame);
}

//---------------------------------------------------------------------------------------
// �A�j���[�V���� �t���[���X�V
//---------------------------------------------------------------------------------------
void C_FbxModel::SetNewPoseMatrices(C_FbxMesh* pMesh, int nFrame)
{
	if (pMesh) {
		pMesh->SetNewPoseMatrices(nFrame);
		for (DWORD i = 0; i < pMesh->m_dwNumChild; ++i)
			SetNewPoseMatrices(pMesh->m_ppChild[i], nFrame);
	}
}

//---------------------------------------------------------------------------------------
// �A�j���[�V���� �t���[�����擾
//---------------------------------------------------------------------------------------
int C_FbxModel::GetMaxAnimFrame()
{
	return static_cast<int>(m_tStop.GetFrameCount(FbxTime::eFrames60));
}

//---------------------------------------------------------------------------------------
// �A�j���[�V�������擾
//---------------------------------------------------------------------------------------
int C_FbxModel::GetMaxAnimStack()
{
	return m_strAnimStackName.GetCount();
}

//---------------------------------------------------------------------------------------
// �A�j���[�V�����؊�
//---------------------------------------------------------------------------------------
void C_FbxModel::SetAnimStack(int nAnimStack)
{
	int nStackCount = GetMaxAnimStack();
	if (nAnimStack < 0 || nAnimStack >= nStackCount)
		return;
	m_nAnimStack = nAnimStack;
	FbxAnimStack *AnimationStack = m_pScene->FindMember<FbxAnimStack>(m_strAnimStackName[m_nAnimStack]->Buffer());
	m_pScene->SetCurrentAnimationStack(AnimationStack);
	FbxTakeInfo *takeInfo = m_pScene->GetTakeInfo(*m_strAnimStackName[m_nAnimStack]);
	m_tStart = takeInfo->mLocalTimeSpan.GetStart();
	m_tStop = takeInfo->mLocalTimeSpan.GetStop();
}

//---------------------------------------------------------------------------------------
// �����ݒ�
//---------------------------------------------------------------------------------------
void C_FbxModel::SetLight(C_FbxLight* light)
{
	m_light = light;
}

//---------------------------------------------------------------------------------------
// ���_���W�ݒ�
//---------------------------------------------------------------------------------------
void C_FbxModel::SetCamera(DirectX::XMFLOAT3& vCamera)
{
	m_vCamera = vCamera;
}

//---------------------------------------------------------------------------------------
// �}�e���A���ݒ�
//---------------------------------------------------------------------------------------
void C_FbxModel::SetMaterial(TFbxMaterial* pMaterial)
{
	if (pMaterial) {
		m_material = *pMaterial;
		m_pMaterial = &m_material;
	}
	else {
		m_pMaterial = nullptr;
	}
}

//---------------------------------------------------------------------------------------
//	�A�E�g���C���ݒ�
//---------------------------------------------------------------------------------------
void C_FbxModel::SetOutline() {
	m_bOutline = true;

	LoadShader("VertexOutline", "PixelOutline",
		&m_pVertexSubShader, &m_pVertexSubLayout, &m_pPixelSubShader);
}

//=======================================================================================
//	End of File
//=======================================================================================
