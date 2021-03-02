#include "FbxMesh.h"
#include "GraphicsManager.h"
#include "Texture.h"
#include "ShaderLight.h"

#pragma comment(lib, "libfbxsdk-mt")
#pragma comment(lib, "libxml2-mt")
#pragma comment(lib, "zlib-mt")

using namespace DirectX;
/**
 * @brief コンストラクタ。各変数を初期化。
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
 * @brief デストラクタ。マテリアルなどのために用意された生ポインタの管理
 *		  最終的にはここの処理はできるだけ省いて少なくする。
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
 * @brief メッシュ生成。FBXのメッシュデータを元に描画用のメッシュを生成する。
 * @param[in] pFbxMesh FBXのメッシュデータへのポインタ。
 */
HRESULT C_FbxMesh::CreateFromFBX(FbxMesh* pFbxMesh)
{
	HRESULT hr = S_OK;
	// 事前にポリゴン数、頂点数、テクスチャ座標数を調べる
	m_dwNumFace = pFbxMesh->GetPolygonCount();
	m_dwNumVert = pFbxMesh->GetControlPointsCount();
	m_dwNumUV = pFbxMesh->GetTextureUVCount();
	if (m_dwNumFace == 0 || m_dwNumVert == 0)
		return hr;
	// 一時的なメモリ確保 (頂点バッファとインデックスバッファ)
	TFbxVertex* pvVB = new TFbxVertex[(m_dwNumVert < m_dwNumUV) ? m_dwNumUV : m_dwNumVert];
	// ポリゴンごとに頂点読み込み、法線読み込み、UV読み込み
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
		if (m_dwNumVert < m_dwNumUV) {	// UV数のほうが多い場合は本来の(頂点ベース)インデックスを利用しない。UVインデックスを基準にする
			iIndex0 = pFbxMesh->GetTextureUVIndex(i, 0);
			iIndex1 = pFbxMesh->GetTextureUVIndex(i, 1);
			iIndex2 = pFbxMesh->GetTextureUVIndex(i, 2);
		}
		else {
			iStartIndex = pFbxMesh->GetPolygonVertexIndex(i);
			piIndex = pFbxMesh->GetPolygonVertices();	//（頂点インデックス）読み込み
			iIndex0 = piIndex[iStartIndex + 0];
			iIndex1 = piIndex[iStartIndex + 1];
			iIndex2 = piIndex[iStartIndex + 2];
		}
		// 頂点
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
		// 法線
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
		// テクスチャ座標
		if (bPolygonVertex) {
			// UVマッピングモードがByPolygonVertexの時
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

	// テクスチャ座標
	if (m_dwNumUV && eMapMode == FbxLayerElement::eByControlPoint) {
		// UVマッピングモードがByControlPointの時
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

	// マテリアル読み込み
	FbxNode* pNode = pFbxMesh->GetNode();
	m_dwNumMaterial = pNode->GetMaterialCount();
	m_pMaterial = new TFbxMaterial[m_dwNumMaterial];
	// マテリアルの数だけインデックスバッファを作成
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
			// 環境光
			Color = pPhong->Ambient.Get();
			Factor = pPhong->AmbientFactor.Get();
			m_pMaterial[i].Ka.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Ka.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Ka.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Ka.w = 0.0f;	// テクスチャ無
			// 拡散反射光
			Color = pPhong->Diffuse.Get();
			Factor = pPhong->DiffuseFactor.Get();
			Alpha = pPhong->TransparentColor.Get();
			AlphaFactor = pPhong->TransparencyFactor.Get();
			m_pMaterial[i].Kd.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Kd.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Kd.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Kd.w = static_cast<float>(1.0 - Alpha[0] * AlphaFactor);
			// 鏡面反射光
			Color = pPhong->Specular.Get();
			Factor = pPhong->SpecularFactor.Get();
			AlphaFactor = pPhong->Shininess.Get();
			m_pMaterial[i].Ks.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Ks.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Ks.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Ks.w = static_cast<float>(AlphaFactor);
			// 発光色
			Color = pPhong->Emissive.Get();
			m_pMaterial[i].Ke.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Ke.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Ke.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Ka.w = 0.0f;	// テクスチャ無
		}
		else if (pMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
			FbxSurfaceLambert* pLambert = (FbxSurfaceLambert*)pMaterial;
			// 環境光
			Color = pLambert->Ambient.Get();
			Factor = pLambert->AmbientFactor.Get();
			m_pMaterial[i].Ka.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Ka.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Ka.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Ka.w = 0.0f;	// テクスチャ無
			// 拡散反射光
			Color = pLambert->Diffuse.Get();
			Factor = pLambert->DiffuseFactor.Get();
			Alpha = pLambert->TransparentColor.Get();
			AlphaFactor = pLambert->TransparencyFactor.Get();
			m_pMaterial[i].Kd.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Kd.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Kd.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Kd.w = static_cast<float>(1.0 - Alpha[0] * AlphaFactor);
			// 発光色
			Color = pLambert->Emissive.Get();
			Factor = pLambert->EmissiveFactor.Get();
			m_pMaterial[i].Ke.x = static_cast<float>(Color[0] * Factor);
			m_pMaterial[i].Ke.y = static_cast<float>(Color[1] * Factor);
			m_pMaterial[i].Ke.z = static_cast<float>(Color[2] * Factor);
			m_pMaterial[i].Ka.w = 0.0f;	// テクスチャ無
		}
		// ディフューズ テクスチャ (1枚のみ)
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
			break;	// 先頭の1つだけ読み込む
		}
		if (pszFName) {
			// テクスチャ作成
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
						MessageBoxA(0, "テクスチャ読み込み失敗", fname, MB_OK);
						delete[] pvVB;
						return hr;
					}
				}
				if (pszFName) {
					FbxFree(pszFName);
					pszFName = nullptr;
				}
			}
			m_pMaterial[i].Ka.w = 1.0f;	// テクスチャ有
		}
		// エミッシブ テクスチャ (1枚のみ)
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
		lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
		for (int j = 0; j < lTextureCount; ++j) {
			lFileTexture = lProperty.GetSrcObject<FbxFileTexture>(j);
			if (!lFileTexture)
				continue;
			FbxUTF8ToAnsi(lFileTexture->GetRelativeFileName(), pszFName, &uFName);
			break;	// 先頭の1つだけ読み込む
		}
		if (pszFName) {
			// テクスチャ作成
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
						MessageBoxA(0, "エミッシブ テクスチャ読み込み失敗", fname, MB_OK);
						delete[] pvVB;
						return hr;
					}
				}
				if (pszFName) {
					FbxFree(pszFName);
					pszFName = nullptr;
				}
			}
			m_pMaterial[i].Ke.w = 1.0f;	// テクスチャ有
		}
		// マテリアルの数だけインデックスバッファを作成
		int* pIndex = new int[m_dwNumFace * 3];	// とりあえず、メッシュ内のポリゴン数でメモリ確保 (個々のポリゴングループはかならずこれ以下になる)
		int iCount = 0;							// そのマテリアルであるインデックス配列内の開始インデックスを調べる　さらにインデックスの個数も調べる
		for (DWORD k = 0; k < m_dwNumFace; ++k) {
			FbxLayerElementMaterial* mat = pFbxMesh->GetLayer(0)->GetMaterials();	// レイヤーが1枚だけを想定
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
		m_pMaterial[i].dwNumFace = iCount / 3;	// そのマテリアル内のポリゴン数		
		delete[] pIndex;
	}

	// 頂点からポリゴンを逆引きするための逆引きテーブルを作る 
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

	// スキン情報 (ジョイント、ウェイト) 読み込み
	ReadSkinInfo(pFbxMesh, pvVB, PolyTable);

	// 頂点バッファ作成
	if (m_dwNumVert < m_dwNumUV) m_dwNumVert = m_dwNumUV;
	CD3D11_BUFFER_DESC bd(sizeof(TFbxVertex) * m_dwNumVert, D3D11_BIND_VERTEX_BUFFER);
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = pvVB;
	hr = GraphicsManager::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_pVertexBuffer);

	// ワーク削除
	delete[] PolyTable;
	delete[] pvVB;

	// コンスタントバッファ ボーン用 作成
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
 * @brief インデックスバッファ生成。
 * @param[in] dwSize バッファのサイズ
 * @param[in] pIndex インデックス配列の先頭アドレス
 * @param[out] ppIndexBuffer インデックスバッファへのポインタ
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
// メッシュ描画
//---------------------------------------------------------------------------------------
void C_FbxMesh::RenderMesh(EByOpacity byOpacity)
{
	// 定数領域更新
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

	// 頂点バッファをセット (頂点バッファは1つ)
	UINT stride = sizeof(TFbxVertex);
	UINT offset = 0;
	GraphicsManager::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// プリミティブ形状をセット
	GraphicsManager::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアルの数だけ描画
	for (DWORD i = 0; i < m_dwNumMaterial; ++i) {
		// 使用されていないマテリアル対策
		if (m_pMaterial[i].dwNumFace == 0) {
			continue;
		}
		// ユーザ定義マテリアル
		TFbxMaterial* pMaterial = &m_pMaterial[i];
		if (m_pMateUsr)
			pMaterial = m_pMateUsr;
		// 透明度による描画
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
		// インデックスバッファをセット
		stride = sizeof(int);
		offset = 0;
		GraphicsManager::GetInstance().GetDeviceContext()->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		// マテリアルの各要素をシェーダに渡す
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
		// テクスチャをシェーダに渡す
		GraphicsManager::GetInstance().GetDeviceContext()->PSSetSamplers(0, 1, m_pSampleLinear.GetAddressOf());
		if (m_pMaterial[i].pTexture || m_pMaterial[i].pTexEmmisive) {
			if (m_pMaterial[i].pTexture)
				GraphicsManager::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, m_pMaterial[i].pTexture.GetAddressOf());
			if (m_pMaterial[i].pTexEmmisive)
				GraphicsManager::GetInstance().GetDeviceContext()->PSSetShaderResources(1, 1, m_pMaterial[i].pTexEmmisive.GetAddressOf());
		}
		// ボーンをシェーダに渡す
		GraphicsManager::GetInstance().GetDeviceContext()->VSSetConstantBuffers(2, 1, m_pConstantBufferBone.GetAddressOf());
		GraphicsManager::GetInstance().GetDeviceContext()->PSSetConstantBuffers(2, 1, m_pConstantBufferBone.GetAddressOf());
		ShaderLightBuffer::GetInstance().UpdateLight(m_pConstantBuffer2.Get(), m_pConstantBuffer2.GetAddressOf());

		// 描画
		GraphicsManager::GetInstance().GetDeviceContext()->DrawIndexed(m_pMaterial[i].dwNumFace * 3, 0, 0);
	}
}

//---------------------------------------------------------------------------------------
// FBXからスキン関連の情報を読み出す　
//---------------------------------------------------------------------------------------
HRESULT C_FbxMesh::ReadSkinInfo(FbxMesh* pFbxMesh, TFbxVertex* pvVB, TPolyTable* PolyTable)
{
	// 頂点ごとのボーンインデックス、頂点ごとのボーンウェイト、バインド行列、ポーズ行列 の4項目を抽出
	int nSkinCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (nSkinCount <= 0) return S_OK;
	m_pBoneTable = new TSkinInfo[nSkinCount];
	m_nNumSkin = nSkinCount;
	int offset = 0;
	for (int nSkin = 0; nSkin < nSkinCount; ++nSkin) {
		TSkinInfo& bt = m_pBoneTable[nSkin];
		// デフォーマ取得
		FbxDeformer* pDeformer = pFbxMesh->GetDeformer(nSkin);
		if (!pDeformer) {
			continue;
		}
		FbxSkin* pSkinInfo = static_cast<FbxSkin*>(pDeformer);

		// ボーン取得
		int nNumBone = pSkinInfo->GetClusterCount();
		bt.ppCluster = new FbxCluster*[nNumBone];
		for (int i = 0; i < nNumBone; ++i) {
			bt.ppCluster[i] = pSkinInfo->GetCluster(i);
		}

		// 通常の場合 (頂点数≧UV数　pvVBが頂点インデックス ベースの場合)
		if (m_dwNumVert >= m_dwNumUV) {
			// それぞれのボーンに影響を受ける頂点を調べる　そこから逆に、頂点ベースでボーンインデックス・重みを整頓する
			for (int i = 0; i < nNumBone; ++i) {
				int nNumIndex = bt.ppCluster[i]->GetControlPointIndicesCount();//このボーンに影響を受ける頂点数
				int* piIndex = bt.ppCluster[i]->GetControlPointIndices();
				double* pdWeight = bt.ppCluster[i]->GetControlPointWeights();
				// 頂点側からインデックスをたどって、頂点サイドで整理する
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
					if (m < 4) continue;	// 設定済ならスキップ
					for (m = 0; m < 4; ++m) {	// 5本以上の場合は、重みの大きい順に4本に絞る
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
			// UVインデックスベースの場合 (頂点数<UV数)
			int PolyIndex = 0;
			int UVIndex = 0;
			// それぞれのボーンに影響を受ける頂点を調べる　そこから逆に、頂点ベースでボーンインデックス・重みを整頓する
			for (int i = 0; i < nNumBone; ++i) {
				int nNumIndex = bt.ppCluster[i]->GetControlPointIndicesCount();//このボーンに影響を受ける頂点数
				int* piIndex = bt.ppCluster[i]->GetControlPointIndices();
				double* pdWeight = bt.ppCluster[i]->GetControlPointWeights();
				// 頂点側からインデックスをたどって、頂点サイドで整理する
				for (int k = 0; k < nNumIndex; ++k) {
					// その頂点を含んでいるポリゴンすべてに、このボーンとウェイトを適用
					for (int p = 0; p < PolyTable[piIndex[k]].nNumRef; ++p) {
						// 頂点→属すポリゴン→そのポリゴンのUVインデックス と逆引き
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
						if (m < 4) continue;	// 設定済ならスキップ
						for (m = 0; m < 4; ++m) {	// 5本以上の場合は、重みの大きい順に4本に絞る
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

		// ボーンを生成
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
// ボーンを次のポーズ位置にセットする
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
	time.SetTime(0, 0, 0, nFrame, 0, 0, FbxTime::eFrames60);	// 60フレーム/秒 と推定 厳密には状況ごとに調べる必要あり
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
				XMLoadFloat4x4(&bt.pBoneArray[i].mNewPose) * XMLoadFloat4x4(&m));//FBX右手座標系なのでｘが逆　補正する	
		}
	}
	// フレームを進めたことにより変化したポーズ（ボーンの行列）をシェーダーに渡す
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
// 次(現在)のポーズ行列を返す
//---------------------------------------------------------------------------------------
XMFLOAT4X4 C_FbxMesh::GetCurrentPoseMatrix(int nSkin, int nIndex)
{
	XMMATRIX inv;
	inv = XMMatrixInverse(0, XMLoadFloat4x4(&m_pBoneTable[nSkin].pBoneArray[nIndex].mBindPose));	// FBXのバインドポーズは初期姿勢（絶対座標）
	XMFLOAT4X4 ret;
	XMStoreFloat4x4(&ret, inv * XMLoadFloat4x4(&m_pBoneTable[nSkin].pBoneArray[nIndex].mNewPose));	// バインドポーズの逆行列とフレーム姿勢行列をかける。なお、バインドポーズ自体が既に逆行列であるとする考えもある。（FBXの場合は違うが）
	return ret;
}
