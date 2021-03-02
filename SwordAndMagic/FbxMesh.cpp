#include "FbxMesh.h"
#include "GraphicsManager.h"
#include "Texture.h"
#include "ShaderLight.h"

#pragma comment(lib, "libfbxsdk-mt")
#pragma comment(lib, "libxml2-mt")
#pragma comment(lib, "zlib-mt")

using namespace DirectX;
/**
 * @brief �R���X�g���N�^�B�e�ϐ����������B
 */
C_FbxMesh::C_FbxMesh()
{
	m_ppChild = nullptr;
	m_dwNumChild = 0;
	m_pSampleLinear = nullptr;
	m_pConstantBuffer0 = nullptr;
	m_pConstantBuffer1 = nullptr;
	m_pConstantBufferBone = nullptr;
	m_pFBXNode = nullptr;
	XMStoreFloat4x4(&m_mView, XMMatrixIdentity());
	m_mProj = m_mView;
	m_mParentOrientation = m_mView;
	m_mFBXOrientation = m_mView;
	m_mFinalWorld = m_mView;
	m_pMateUsr = nullptr;
	m_pLight = nullptr;
	m_pCamera = nullptr;
	m_dwNumVert = 0;
	m_dwNumFace = 0;
	m_dwNumUV = 0;
	m_pVertexBuffer = nullptr;
	m_ppIndexBuffer = nullptr;
	m_pMaterial = nullptr;
	m_dwNumMaterial = 0;
	m_nNumSkin = 0;
	m_pBoneTable = nullptr;
}

/**
 * @brief �f�X�g���N�^�B�}�e���A���Ȃǂ̂��߂ɗp�ӂ��ꂽ���|�C���^�̊Ǘ�
 *		  �ŏI�I�ɂ͂����̏����͂ł��邾���Ȃ��ď��Ȃ�����B
 */
C_FbxMesh::~C_FbxMesh()
{
	SAFE_DELETE_ARRAY(m_pMaterial);
	SAFE_RELEASE(m_pVertexBuffer);
	for (DWORD i = 0; i < m_dwNumMaterial; ++i) {
		SAFE_RELEASE(m_ppIndexBuffer[i]);
	}
	SAFE_DELETE_ARRAY(m_ppIndexBuffer);
	SAFE_DELETE_ARRAY(m_ppChild);
	SAFE_RELEASE(m_pConstantBufferBone);
}

/**
 * @brief ���b�V�������BFBX�̃��b�V���f�[�^�����ɕ`��p�̃��b�V���𐶐�����B
 * @param[in] pFbxMesh FBX�̃��b�V���f�[�^�ւ̃|�C���^�B
 */
HRESULT C_FbxMesh::CreateFromFBX(FbxMesh* pFbxMesh)
{
	HRESULT hr = S_OK;
	// ���O�Ƀ|���S�����A���_���A�e�N�X�`�����W���𒲂ׂ�
	m_dwNumFace = pFbxMesh->GetPolygonCount();
	m_dwNumVert = pFbxMesh->GetControlPointsCount();
	m_dwNumUV = pFbxMesh->GetTextureUVCount();
	if (m_dwNumFace == 0 || m_dwNumVert == 0)
		return hr;
	// �ꎞ�I�ȃ������m�� (���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@)
	TFbxVertex* pvVB = new TFbxVertex[(m_dwNumVert < m_dwNumUV) ? m_dwNumUV : m_dwNumVert];
	// �|���S�����Ƃɒ��_�ǂݍ��݁A�@���ǂݍ��݁AUV�ǂݍ���
	FbxVector4 Normal, Coord;
	int iStartIndex, iIndex0, iIndex1, iIndex2;
	int* piIndex;
	int index, UVindex;
	FbxVector2 v2;
	FbxGeometryElementUV* pUV = nullptr;
	FbxLayerElement::EMappingMode eMapMode = FbxLayerElement::eNone;
	FbxLayerElement::EReferenceMode eRefMode = FbxLayerElement::eDirect;
	const FbxLayerElementArrayTemplate<int>* pUVIndex = nullptr;
	const FbxLayerElementArrayTemplate<FbxVector2>* pUVDirect = nullptr;
	if (m_dwNumUV) {
		pUV = pFbxMesh->GetElementUV();
		eMapMode = pUV->GetMappingMode();
		eRefMode = pUV->GetReferenceMode();
		pUVIndex = &pUV->GetIndexArray();
		pUVDirect = &pUV->GetDirectArray();
	}
	bool bPolygonVertex = (m_dwNumUV && eMapMode == FbxLayerElement::eByPolygonVertex);
	for (DWORD i = 0; i < m_dwNumFace; ++i) {
		if (m_dwNumVert < m_dwNumUV) {	// UV���̂ق��������ꍇ�͖{����(���_�x�[�X)�C���f�b�N�X�𗘗p���Ȃ��BUV�C���f�b�N�X����ɂ���
			iIndex0 = pFbxMesh->GetTextureUVIndex(i, 0);
			iIndex1 = pFbxMesh->GetTextureUVIndex(i, 1);
			iIndex2 = pFbxMesh->GetTextureUVIndex(i, 2);
		}
		else {
			iStartIndex = pFbxMesh->GetPolygonVertexIndex(i);
			piIndex = pFbxMesh->GetPolygonVertices();	//�i���_�C���f�b�N�X�j�ǂݍ���
			iIndex0 = piIndex[iStartIndex + 0];
			iIndex1 = piIndex[iStartIndex + 1];
			iIndex2 = piIndex[iStartIndex + 2];
		}
		// ���_
		index = pFbxMesh->GetPolygonVertex(i, 0);
		Coord = pFbxMesh->GetControlPointAt(index);
		pvVB[iIndex0].vPos.x = static_cast<float>(-Coord[0]);
		pvVB[iIndex0].vPos.y = static_cast<float>(Coord[1]);
		pvVB[iIndex0].vPos.z = static_cast<float>(Coord[2]);

		index = pFbxMesh->GetPolygonVertex(i, 1);
		Coord = pFbxMesh->GetControlPointAt(index);
		pvVB[iIndex1].vPos.x = static_cast<float>(-Coord[0]);
		pvVB[iIndex1].vPos.y = static_cast<float>(Coord[1]);
		pvVB[iIndex1].vPos.z = static_cast<float>(Coord[2]);

		index = pFbxMesh->GetPolygonVertex(i, 2);
		Coord = pFbxMesh->GetControlPointAt(index);
		pvVB[iIndex2].vPos.x = static_cast<float>(-Coord[0]);
		pvVB[iIndex2].vPos.y = static_cast<float>(Coord[1]);
		pvVB[iIndex2].vPos.z = static_cast<float>(Coord[2]);
		// �@��
		pFbxMesh->GetPolygonVertexNormal(i, 0, Normal);
		pvVB[iIndex0].vNorm.x = static_cast<float>(-Normal[0]);
		pvVB[iIndex0].vNorm.y = static_cast<float>(Normal[1]);
		pvVB[iIndex0].vNorm.z = static_cast<float>(Normal[2]);

		pFbxMesh->GetPolygonVertexNormal(i, 1, Normal);
		pvVB[iIndex1].vNorm.x = static_cast<float>(-Normal[0]);
		pvVB[iIndex1].vNorm.y = static_cast<float>(Normal[1]);
		pvVB[iIndex1].vNorm.z = static_cast<float>(Normal[2]);

		pFbxMesh->GetPolygonVertexNormal(i, 2, Normal);
		pvVB[iIndex2].vNorm.x = static_cast<float>(-Normal[0]);
		pvVB[iIndex2].vNorm.y = static_cast<float>(Normal[1]);
		pvVB[iIndex2].vNorm.z = static_cast<float>(Normal[2]);
		// �e�N�X�`�����W
		if (bPolygonVertex) {
			// UV�}�b�s���O���[�h��ByPolygonVertex�̎�
			UVindex = pFbxMesh->GetTextureUVIndex(i, 0);
			v2 = pUVDirect->GetAt(UVindex);
			pvVB[iIndex0].vTex.x = static_cast<float>(v2[0]);
			pvVB[iIndex0].vTex.y = static_cast<float>(1.0 - v2[1]);

			UVindex = pFbxMesh->GetTextureUVIndex(i, 1);
			v2 = pUVDirect->GetAt(UVindex);
			pvVB[iIndex1].vTex.x = static_cast<float>(v2[0]);
			pvVB[iIndex1].vTex.y = static_cast<float>(1.0 - v2[1]);

			UVindex = pFbxMesh->GetTextureUVIndex(i, 2);
			v2 = pUVDirect->GetAt(UVindex);
			pvVB[iIndex2].vTex.x = static_cast<float>(v2[0]);
			pvVB[iIndex2].vTex.y = static_cast<float>(1.0 - v2[1]);
		}
	}

	// �e�N�X�`�����W
	if (m_dwNumUV && eMapMode == FbxLayerElement::eByControlPoint) {
		// UV�}�b�s���O���[�h��ByControlPoint�̎�
		for (DWORD k = 0; k < m_dwNumUV; ++k) {
			if (eRefMode == FbxLayerElement::eIndexToDirect) {
				index = pUVIndex->GetAt(k);
			}
			else {
				index = k;
			}
			v2 = pUVDirect->GetAt(index);
			pvVB[k].vTex.x = static_cast<float>(v2[0]);
			pvVB[k].vTex.y = static_cast<float>(1.0 - v2[1]);
		}
	}

	// �}�e���A���ǂݍ���
	FbxNode* pNode = pFbxMesh->GetNode();
	m_dwNumMaterial = pNode->GetMaterialCount();
	m_pMaterial = new TFbxMaterial[m_dwNumMaterial];
	// �}�e���A���̐������C���f�b�N�X�o�b�t�@���쐬
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterial];
	ZeroMemory(m_ppIndexBuffer, sizeof(ID3D11Buffer*) * m_dwNumMaterial);
	FbxDouble3 Color, Alpha;
	FbxDouble Factor, AlphaFactor;
	for (DWORD i = 0; i < m_dwNumMaterial; ++i) {
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		char name[_MAX_PATH];
		strcpy_s(name, _MAX_PATH, pMaterial->GetName());
		if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId)) {
			FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;
			// ����
			Color = pPhong->Ambient.Get();
			Factor = pPhong->AmbientFactor.Get();
			m_pMaterial[i].Ka.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Ka.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Ka.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Ka.w = 0.0f;	// �e�N�X�`����
			// �g�U���ˌ�
			Color = pPhong->Diffuse.Get();
			Factor = pPhong->DiffuseFactor.Get();
			Alpha = pPhong->TransparentColor.Get();
			AlphaFactor = pPhong->TransparencyFactor.Get();
			m_pMaterial[i].Kd.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Kd.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Kd.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Kd.w = static_cast<float>(1.0 - Alpha[0] * AlphaFactor);
			// ���ʔ��ˌ�
			Color = pPhong->Specular.Get();
			Factor = pPhong->SpecularFactor.Get();
			AlphaFactor = pPhong->Shininess.Get();
			m_pMaterial[i].Ks.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Ks.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Ks.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Ks.w = static_cast<float>(AlphaFactor);
			// �����F
			Color = pPhong->Emissive.Get();
			m_pMaterial[i].Ke.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Ke.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Ke.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Ka.w = 0.0f;	// �e�N�X�`����
		}
		else if (pMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
			FbxSurfaceLambert* pLambert = (FbxSurfaceLambert*)pMaterial;
			// ����
			Color = pLambert->Ambient.Get();
			Factor = pLambert->AmbientFactor.Get();
			m_pMaterial[i].Ka.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Ka.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Ka.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Ka.w = 0.0f;	// �e�N�X�`����
			// �g�U���ˌ�
			Color = pLambert->Diffuse.Get();
			Factor = pLambert->DiffuseFactor.Get();
			Alpha = pLambert->TransparentColor.Get();
			AlphaFactor = pLambert->TransparencyFactor.Get();
			m_pMaterial[i].Kd.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Kd.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Kd.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Kd.w = static_cast<float>(1.0 - Alpha[0] * AlphaFactor);
			// �����F
			Color = pLambert->Emissive.Get();
			Factor = pLambert->EmissiveFactor.Get();
			m_pMaterial[i].Ke.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Ke.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Ke.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Ka.w = 0.0f;	// �e�N�X�`����
		}
		// �f�B�t���[�Y �e�N�X�`�� (1���̂�)
		FbxProperty lProperty;
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
		FbxFileTexture* lFileTexture;
		char* pszFName = nullptr;
		size_t uFName = 0;
		for (int j = 0; j < lTextureCount; ++j) {
			lFileTexture = lProperty.GetSrcObject<FbxFileTexture>(j);
			if (!lFileTexture)
				continue;
			FbxUTF8ToAnsi(lFileTexture->GetRelativeFileName(), pszFName, &uFName);
			break;	// �擪��1�����ǂݍ���
		}
		if (pszFName) {
			// �e�N�X�`���쐬
			hr = CreateTextureFromFile(GraphicsManager::GetInstance().GetDevice(), pszFName, &m_pMaterial[i].pTexture);
			FbxFree(pszFName);
			pszFName = nullptr;
			if (FAILED(hr)) {
				uFName = 0;
				FbxUTF8ToAnsi(lFileTexture->GetFileName(), pszFName, &uFName);
				hr = CreateTextureFromFile(GraphicsManager::GetInstance().GetDevice(), pszFName, &m_pMaterial[i].pTexture);
				if (FAILED(hr)) {
					char fname[_MAX_PATH];
					char ext[_MAX_EXT];
					_splitpath_s(pszFName, nullptr, 0, nullptr, 0,
						fname, _countof(fname), ext, _countof(ext));
					strcat_s(fname, _countof(fname), ext);
					hr = CreateTextureFromFile(GraphicsManager::GetInstance().GetDevice(), fname, &m_pMaterial[i].pTexture);
					if (FAILED(hr)) {
						MessageBoxA(0, "�e�N�X�`���ǂݍ��ݎ��s", fname, MB_OK);
						delete[] pvVB;
						return hr;
					}
				}
				if (pszFName) {
					FbxFree(pszFName);
					pszFName = nullptr;
				}
			}
			m_pMaterial[i].Ka.w = 1.0f;	// �e�N�X�`���L
		}
		// �G�~�b�V�u �e�N�X�`�� (1���̂�)
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
		lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
		for (int j = 0; j < lTextureCount; ++j) {
			lFileTexture = lProperty.GetSrcObject<FbxFileTexture>(j);
			if (!lFileTexture)
				continue;
			FbxUTF8ToAnsi(lFileTexture->GetRelativeFileName(), pszFName, &uFName);
			break;	// �擪��1�����ǂݍ���
		}
		if (pszFName) {
			// �e�N�X�`���쐬
			hr = CreateTextureFromFile(GraphicsManager::GetInstance().GetDevice(), pszFName, &m_pMaterial[i].pTexEmmisive);
			FbxFree(pszFName);
			pszFName = nullptr;
			if (FAILED(hr)) {
				uFName = 0;
				FbxUTF8ToAnsi(lFileTexture->GetFileName(), pszFName, &uFName);
				hr = CreateTextureFromFile(GraphicsManager::GetInstance().GetDevice(), pszFName, &m_pMaterial[i].pTexEmmisive);
				if (FAILED(hr)) {
					char fname[_MAX_PATH];
					char ext[_MAX_EXT];
					_splitpath_s(pszFName, nullptr, 0, nullptr, 0,
						fname, _countof(fname), ext, _countof(ext));
					strcat_s(fname, _countof(fname), ext);
					hr = CreateTextureFromFile(GraphicsManager::GetInstance().GetDevice(), fname, &m_pMaterial[i].pTexEmmisive);
					if (FAILED(hr)) {
						MessageBoxA(0, "�G�~�b�V�u �e�N�X�`���ǂݍ��ݎ��s", fname, MB_OK);
						delete[] pvVB;
						return hr;
					}
				}
				if (pszFName) {
					FbxFree(pszFName);
					pszFName = nullptr;
				}
			}
			m_pMaterial[i].Ke.w = 1.0f;	// �e�N�X�`���L
		}
		// �}�e���A���̐������C���f�b�N�X�o�b�t�@���쐬
		int* pIndex = new int[m_dwNumFace * 3];	// �Ƃ肠�����A���b�V�����̃|���S�����Ń������m�� (�X�̃|���S���O���[�v�͂��Ȃ炸����ȉ��ɂȂ�)
		int iCount = 0;							// ���̃}�e���A���ł���C���f�b�N�X�z����̊J�n�C���f�b�N�X�𒲂ׂ�@����ɃC���f�b�N�X�̌������ׂ�
		for (DWORD k = 0; k < m_dwNumFace; ++k) {
			FbxLayerElementMaterial* mat = pFbxMesh->GetLayer(0)->GetMaterials();	// ���C���[��1��������z��
			int matId = mat->GetIndexArray().GetAt(k);
			if (matId == i) {
				if (m_dwNumVert < m_dwNumUV) {
					pIndex[iCount] = pFbxMesh->GetTextureUVIndex(k, 0, FbxLayerElement::eTextureDiffuse);
					pIndex[iCount + 1] = pFbxMesh->GetTextureUVIndex(k, 1, FbxLayerElement::eTextureDiffuse);
					pIndex[iCount + 2] = pFbxMesh->GetTextureUVIndex(k, 2, FbxLayerElement::eTextureDiffuse);
				}
				else {
					pIndex[iCount] = pFbxMesh->GetPolygonVertex(k, 0);
					pIndex[iCount + 1] = pFbxMesh->GetPolygonVertex(k, 1);
					pIndex[iCount + 2] = pFbxMesh->GetPolygonVertex(k, 2);
				}
				iCount += 3;
			}
		}
		if (iCount > 0) {
			CreateIndexBuffer(iCount * sizeof(int), pIndex, &m_ppIndexBuffer[i]);
		}
		m_pMaterial[i].dwNumFace = iCount / 3;	// ���̃}�e���A�����̃|���S����		
		delete[] pIndex;
	}

	// ���_����|���S�����t�������邽�߂̋t�����e�[�u������� 
	TPolyTable* PolyTable = new TPolyTable[m_dwNumVert];
	for (DWORD k = 0; k < m_dwNumFace; ++k) {
		for (int m = 0; m < 3; ++m) {
			int i = pFbxMesh->GetPolygonVertex(k, m);
			if (PolyTable[i].nNumRef >= MAX_REF_POLY) {
				delete[] PolyTable;
				delete[] pvVB;
				return E_FAIL;
			}
			PolyTable[i].nPolyIndex[PolyTable[i].nNumRef] = k;
			PolyTable[i].nIndex123[PolyTable[i].nNumRef] = m;
			PolyTable[i].nNumRef++;
		}
	}

	// �X�L����� (�W���C���g�A�E�F�C�g) �ǂݍ���
	ReadSkinInfo(pFbxMesh, pvVB, PolyTable);

	// ���_�o�b�t�@�쐬
	if (m_dwNumVert < m_dwNumUV) m_dwNumVert = m_dwNumUV;
	CD3D11_BUFFER_DESC bd(sizeof(TFbxVertex) * m_dwNumVert, D3D11_BIND_VERTEX_BUFFER);
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = pvVB;
	hr = GraphicsManager::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_pVertexBuffer);

	// ���[�N�폜
	delete[] PolyTable;
	delete[] pvVB;

	// �R���X�^���g�o�b�t�@ �{�[���p �쐬
	D3D11_BUFFER_DESC cb;
	ZeroMemory(&cb, sizeof(cb));
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_BONE);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	hr = GraphicsManager::GetInstance().GetDevice()->CreateBuffer(&cb, nullptr, m_pConstantBufferBone.GetAddressOf());

	return hr;
}

/**
 * @brief �C���f�b�N�X�o�b�t�@�����B
 * @param[in] dwSize �o�b�t�@�̃T�C�Y
 * @param[in] pIndex �C���f�b�N�X�z��̐擪�A�h���X
 * @param[out] ppIndexBuffer �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
 */
HRESULT C_FbxMesh::CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer)
{
	CD3D11_BUFFER_DESC bd(dwSize, D3D11_BIND_INDEX_BUFFER);
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = pIndex;
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;
	return GraphicsManager::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, ppIndexBuffer);
}

//---------------------------------------------------------------------------------------
// ���b�V���`��
//---------------------------------------------------------------------------------------
void C_FbxMesh::RenderMesh(EByOpacity byOpacity)
{
	// �萔�̈�X�V
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(GraphicsManager::GetInstance().GetDeviceContext()->Map(m_pConstantBuffer0.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		SHADER_GLOBAL sg;
		XMMATRIX mtxWorld = XMLoadFloat4x4(&m_mFinalWorld);
		XMMATRIX mtxView = XMLoadFloat4x4(&m_mView);
		XMMATRIX mtxProj = XMLoadFloat4x4(&m_mProj);
		sg.mW = XMMatrixTranspose(mtxWorld);
		sg.mWVP = mtxWorld * mtxView * mtxProj;
		sg.mWVP = XMMatrixTranspose(sg.mWVP);
		sg.vEye = XMLoadFloat3(m_pCamera);
		sg.vLightDir = XMLoadFloat3(&m_pLight->m_direction);
		sg.vLd = XMLoadFloat4(&m_pLight->m_diffuse);
		sg.vLa = XMLoadFloat4(&m_pLight->m_ambient);
		sg.vLs = XMLoadFloat4(&m_pLight->m_specular);
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(sg));
		GraphicsManager::GetInstance().GetDeviceContext()->Unmap(m_pConstantBuffer0.Get(), 0);
	}
	GraphicsManager::GetInstance().GetDeviceContext()->VSSetConstantBuffers(0, 1, m_pConstantBuffer0.GetAddressOf());
	GraphicsManager::GetInstance().GetDeviceContext()->PSSetConstantBuffers(0, 1, m_pConstantBuffer0.GetAddressOf());

	// ���_�o�b�t�@���Z�b�g (���_�o�b�t�@��1��)
	UINT stride = sizeof(TFbxVertex);
	UINT offset = 0;
	GraphicsManager::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// �v���~�e�B�u�`����Z�b�g
	GraphicsManager::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �}�e���A���̐������`��
	for (DWORD i = 0; i < m_dwNumMaterial; ++i) {
		// �g�p����Ă��Ȃ��}�e���A���΍�
		if (m_pMaterial[i].dwNumFace == 0) {
			continue;
		}
		// ���[�U��`�}�e���A��
		TFbxMaterial* pMaterial = &m_pMaterial[i];
		if (m_pMateUsr)
			pMaterial = m_pMateUsr;
		// �����x�ɂ��`��
		switch (byOpacity) {
		case eOpacityOnly:
			if (pMaterial->Kd.w < 1.0f) continue;
			break;
		case eTransparentOnly:
			if (pMaterial->Kd.w >= 1.0f) continue;
			break;
		default:
			break;
		}
		// �C���f�b�N�X�o�b�t�@���Z�b�g
		stride = sizeof(int);
		offset = 0;
		GraphicsManager::GetInstance().GetDeviceContext()->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		// �}�e���A���̊e�v�f���V�F�[�_�ɓn��
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(GraphicsManager::GetInstance().GetDeviceContext()->Map(m_pConstantBuffer1.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
			SHADER_MATERIAL sg;
			sg.vAmbient = XMLoadFloat4(&pMaterial->Ka);
			sg.vDiffuse = XMLoadFloat4(&pMaterial->Kd);
			sg.vSpecular = XMLoadFloat4(&pMaterial->Ks);
			sg.vEmissive = XMLoadFloat4(&pMaterial->Ke);
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(sg));
			GraphicsManager::GetInstance().GetDeviceContext()->Unmap(m_pConstantBuffer1.Get(), 0);
		}
		GraphicsManager::GetInstance().GetDeviceContext()->VSSetConstantBuffers(1, 1, m_pConstantBuffer1.GetAddressOf());
		GraphicsManager::GetInstance().GetDeviceContext()->PSSetConstantBuffers(1, 1, m_pConstantBuffer1.GetAddressOf());
		// �e�N�X�`�����V�F�[�_�ɓn��
		GraphicsManager::GetInstance().GetDeviceContext()->PSSetSamplers(0, 1, m_pSampleLinear.GetAddressOf());
		if (m_pMaterial[i].pTexture || m_pMaterial[i].pTexEmmisive) {
			if (m_pMaterial[i].pTexture)
				GraphicsManager::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, m_pMaterial[i].pTexture.GetAddressOf());
			if (m_pMaterial[i].pTexEmmisive)
				GraphicsManager::GetInstance().GetDeviceContext()->PSSetShaderResources(1, 1, m_pMaterial[i].pTexEmmisive.GetAddressOf());
		}
		// �{�[�����V�F�[�_�ɓn��
		GraphicsManager::GetInstance().GetDeviceContext()->VSSetConstantBuffers(2, 1, m_pConstantBufferBone.GetAddressOf());
		GraphicsManager::GetInstance().GetDeviceContext()->PSSetConstantBuffers(2, 1, m_pConstantBufferBone.GetAddressOf());
		ShaderLightBuffer::GetInstance().UpdateLight(m_pConstantBuffer2.Get(), m_pConstantBuffer2.GetAddressOf());

		// �`��
		GraphicsManager::GetInstance().GetDeviceContext()->DrawIndexed(m_pMaterial[i].dwNumFace * 3, 0, 0);
	}
}

//---------------------------------------------------------------------------------------
// FBX����X�L���֘A�̏���ǂݏo���@
//---------------------------------------------------------------------------------------
HRESULT C_FbxMesh::ReadSkinInfo(FbxMesh* pFbxMesh, TFbxVertex* pvVB, TPolyTable* PolyTable)
{
	// ���_���Ƃ̃{�[���C���f�b�N�X�A���_���Ƃ̃{�[���E�F�C�g�A�o�C���h�s��A�|�[�Y�s�� ��4���ڂ𒊏o
	int nSkinCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (nSkinCount <= 0) return S_OK;
	m_pBoneTable = new TSkinInfo[nSkinCount];
	m_nNumSkin = nSkinCount;
	int offset = 0;
	for (int nSkin = 0; nSkin < nSkinCount; ++nSkin) {
		TSkinInfo& bt = m_pBoneTable[nSkin];
		// �f�t�H�[�}�擾
		FbxDeformer* pDeformer = pFbxMesh->GetDeformer(nSkin);
		if (!pDeformer) {
			continue;
		}
		FbxSkin* pSkinInfo = static_cast<FbxSkin*>(pDeformer);

		// �{�[���擾
		int nNumBone = pSkinInfo->GetClusterCount();
		bt.ppCluster = new FbxCluster*[nNumBone];
		for (int i = 0; i < nNumBone; ++i) {
			bt.ppCluster[i] = pSkinInfo->GetCluster(i);
		}

		// �ʏ�̏ꍇ (���_����UV���@pvVB�����_�C���f�b�N�X �x�[�X�̏ꍇ)
		if (m_dwNumVert >= m_dwNumUV) {
			// ���ꂼ��̃{�[���ɉe�����󂯂钸�_�𒲂ׂ�@��������t�ɁA���_�x�[�X�Ń{�[���C���f�b�N�X�E�d�݂𐮓ڂ���
			for (int i = 0; i < nNumBone; ++i) {
				int nNumIndex = bt.ppCluster[i]->GetControlPointIndicesCount();//���̃{�[���ɉe�����󂯂钸�_��
				int* piIndex = bt.ppCluster[i]->GetControlPointIndices();
				double* pdWeight = bt.ppCluster[i]->GetControlPointWeights();
				// ���_������C���f�b�N�X�����ǂ��āA���_�T�C�h�Ő�������
				for (int k = 0; k < nNumIndex; ++k) {
					int m;
					for (m = 0; m < 4; ++m) {
						if (pvVB[piIndex[k]].uBoneIndex[m] == i + offset) {
							if (i + offset == 0 && pvVB[piIndex[k]].fBoneWeight[m] <= 0.0f) {
								continue;
							}
							break;
						}
					}
					if (m < 4) continue;	// �ݒ�ςȂ�X�L�b�v
					for (m = 0; m < 4; ++m) {	// 5�{�ȏ�̏ꍇ�́A�d�݂̑傫������4�{�ɍi��
						if (pdWeight[k] > pvVB[piIndex[k]].fBoneWeight[m]) {
							for (int n = 4 - 1; n > m; --n) {
								pvVB[piIndex[k]].uBoneIndex[n] = pvVB[piIndex[k]].uBoneIndex[n - 1];
								pvVB[piIndex[k]].fBoneWeight[n] = pvVB[piIndex[k]].fBoneWeight[n - 1];
							}
							pvVB[piIndex[k]].uBoneIndex[m] = i + offset;
							pvVB[piIndex[k]].fBoneWeight[m] = static_cast<float>(pdWeight[k]);
							break;
						}
					}
				}
			}
		}
		else {
			// UV�C���f�b�N�X�x�[�X�̏ꍇ (���_��<UV��)
			int PolyIndex = 0;
			int UVIndex = 0;
			// ���ꂼ��̃{�[���ɉe�����󂯂钸�_�𒲂ׂ�@��������t�ɁA���_�x�[�X�Ń{�[���C���f�b�N�X�E�d�݂𐮓ڂ���
			for (int i = 0; i < nNumBone; ++i) {
				int nNumIndex = bt.ppCluster[i]->GetControlPointIndicesCount();//���̃{�[���ɉe�����󂯂钸�_��
				int* piIndex = bt.ppCluster[i]->GetControlPointIndices();
				double* pdWeight = bt.ppCluster[i]->GetControlPointWeights();
				// ���_������C���f�b�N�X�����ǂ��āA���_�T�C�h�Ő�������
				for (int k = 0; k < nNumIndex; ++k) {
					// ���̒��_���܂�ł���|���S�����ׂĂɁA���̃{�[���ƃE�F�C�g��K�p
					for (int p = 0; p < PolyTable[piIndex[k]].nNumRef; ++p) {
						// ���_�������|���S�������̃|���S����UV�C���f�b�N�X �Ƌt����
						PolyIndex = PolyTable[piIndex[k]].nPolyIndex[p];
						UVIndex = pFbxMesh->GetTextureUVIndex(PolyIndex, PolyTable[piIndex[k]].nIndex123[p], FbxLayerElement::eTextureDiffuse);
						int m;
						for (m = 0; m < 4; ++m) {
							if (pvVB[UVIndex].uBoneIndex[m] == i + offset) {
								if (i + offset == 0 && pvVB[UVIndex].fBoneWeight[m] <= 0.0f) {
									continue;
								}
								break;
							}
						}
						if (m < 4) continue;	// �ݒ�ςȂ�X�L�b�v
						for (m = 0; m < 4; ++m) {	// 5�{�ȏ�̏ꍇ�́A�d�݂̑傫������4�{�ɍi��
							if (pdWeight[k] > pvVB[UVIndex].fBoneWeight[m]) {
								for (int n = 4 - 1; n > m; --n) {
									pvVB[UVIndex].uBoneIndex[n] = pvVB[UVIndex].uBoneIndex[n - 1];
									pvVB[UVIndex].fBoneWeight[n] = pvVB[UVIndex].fBoneWeight[n - 1];
								}
								pvVB[UVIndex].uBoneIndex[m] = i + offset;
								pvVB[UVIndex].fBoneWeight[m] = static_cast<float>(pdWeight[k]);
								break;
							}
						}
					}
				}
			}
		}

		// �{�[���𐶐�
		bt.nNumBone = nNumBone;
		bt.pBoneArray = new TBone[nNumBone];
		XMFLOAT4X4 m(
			-1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		for (int i = 0; i < nNumBone; ++i) {
			FbxAMatrix mtx;
			bt.ppCluster[i]->GetTransformLinkMatrix(mtx);
			for (int x = 0; x < 4; ++x) {
				for (int y = 0; y < 4; ++y) {
					bt.pBoneArray[i].mBindPose(y, x) = static_cast<float>(mtx.Get(y, x));
				}
			}
			XMStoreFloat4x4(&bt.pBoneArray[i].mBindPose,
				XMLoadFloat4x4(&bt.pBoneArray[i].mBindPose) * XMLoadFloat4x4(&m));
		}

		offset += nNumBone;
	}
	return S_OK;
}

//---------------------------------------------------------------------------------------
// �{�[�������̃|�[�Y�ʒu�ɃZ�b�g����
//---------------------------------------------------------------------------------------
void C_FbxMesh::SetNewPoseMatrices(int nFrame)
{
	if (m_nNumSkin <= 0) return;

	XMFLOAT4X4 m(
		-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	FbxTime time;
	time.SetTime(0, 0, 0, nFrame, 0, 0, FbxTime::eFrames60);	// 60�t���[��/�b �Ɛ��� �����ɂ͏󋵂��Ƃɒ��ׂ�K�v����
	for (int nSkinIndex = 0; nSkinIndex < m_nNumSkin; ++nSkinIndex) {
		TSkinInfo& bt = m_pBoneTable[nSkinIndex];
		for (int i = 0; i < bt.nNumBone; ++i) {
			FbxMatrix mtx = bt.ppCluster[i]->GetLink()->EvaluateGlobalTransform(time);
			for (int x = 0; x < 4; ++x) {
				for (int y = 0; y < 4; ++y) {
					bt.pBoneArray[i].mNewPose(y, x) = static_cast<float>(mtx.Get(y, x));
				}
			}
			XMStoreFloat4x4(&bt.pBoneArray[i].mNewPose,
				XMLoadFloat4x4(&bt.pBoneArray[i].mNewPose) * XMLoadFloat4x4(&m));//FBX�E����W�n�Ȃ̂ł����t�@�␳����	
		}
	}
	// �t���[����i�߂����Ƃɂ��ω������|�[�Y�i�{�[���̍s��j���V�F�[�_�[�ɓn��
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(GraphicsManager::GetInstance().GetDeviceContext()->Map(m_pConstantBufferBone.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		SHADER_BONE sg;
		int j = 0;
		for (int nSkin = 0; nSkin < m_nNumSkin; ++nSkin) {
			for (int i = 0; i < m_pBoneTable[nSkin].nNumBone; ++i) {
				XMFLOAT4X4 mtx = GetCurrentPoseMatrix(nSkin, i);
				if (j >= 256) {
					nSkin = m_nNumSkin;
					break;
				}
				sg.mBone[j++] = XMMatrixTranspose(XMLoadFloat4x4(&mtx));
			}
		}
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_BONE));
		GraphicsManager::GetInstance().GetDeviceContext()->Unmap(m_pConstantBufferBone.Get(), 0);
	}
}

//---------------------------------------------------------------------------------------
// ��(����)�̃|�[�Y�s���Ԃ�
//---------------------------------------------------------------------------------------
XMFLOAT4X4 C_FbxMesh::GetCurrentPoseMatrix(int nSkin, int nIndex)
{
	XMMATRIX inv;
	inv = XMMatrixInverse(0, XMLoadFloat4x4(&m_pBoneTable[nSkin].pBoneArray[nIndex].mBindPose));	// FBX�̃o�C���h�|�[�Y�͏����p���i��΍��W�j
	XMFLOAT4X4 ret;
	XMStoreFloat4x4(&ret, inv * XMLoadFloat4x4(&m_pBoneTable[nSkin].pBoneArray[nIndex].mNewPose));	// �o�C���h�|�[�Y�̋t�s��ƃt���[���p���s���������B�Ȃ��A�o�C���h�|�[�Y���̂����ɋt�s��ł���Ƃ���l��������B�iFBX�̏ꍇ�͈Ⴄ���j
	return ret;
}
