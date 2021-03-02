/**
 * @file FbxUtility.h
 * @brief FBXSDK系コンポーネントで呼び出す用の構造体や定数定義、インクルード。
 * @author MIURA DAIKI
 * @date 2020/12/14
 */
#pragma once
#include "main.h"
#include <fbxsdk.h>
#include <wrl/client.h>


constexpr int MAX_REF_POLY = 60;	// 最大共有頂点参照数
constexpr int MAX_BONE_MATRIX = 256;

/*! @enum EByOpacity
	@brief 透明度による描画分けに使用(不透明、透明で分けて描画する)。  
*/
enum EByOpacity {
	eNoAffect = 0,		//!< @brief 全て描画
	eOpacityOnly,		//!< @brief 不透明のみ描画
	eTransparentOnly,	//!< @brief 透明のみ描画
};

/*! @struct TFbxMaterial
	@brief FBXのマテリアルを定義。
*/
struct TFbxMaterial {
	DirectX::XMFLOAT4	Ka;		//!< @brief  アンビエント+テクスチャ有無
	DirectX::XMFLOAT4	Kd;		//!< @brief  ディフューズ
	DirectX::XMFLOAT4	Ks;		//!< @brief  スペキュラ+スペキュラ強度
	DirectX::XMFLOAT4	Ke;		//!< @brief  エミッシブ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pTexture;		//!< @brief  拡散テクスチャ
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView>	pTexEmmisive;	//!< @brief  発光テクスチャ
	DWORD		dwNumFace;		//!< @brief  このマテリアルのポリゴン数
	/*! 
	@brief コンストラクタ。各要素を初期化。
	*/
	TFbxMaterial()
	{
		Ka = DirectX::XMFLOAT4(0, 0, 0, 0);
		Kd = DirectX::XMFLOAT4(1, 1, 1, 1);
		Ks = DirectX::XMFLOAT4(0, 0, 0, 0);
		Ke = DirectX::XMFLOAT4(0, 0, 0, 0);
		pTexture = nullptr;
		pTexEmmisive = nullptr;
		dwNumFace = 0;
	}
};

/*! @struct TFbxVertex
	@brief FBXの頂点情報を定義。
*/
struct TFbxVertex {
	DirectX::XMFLOAT3	vPos;	//!< @brief 頂点位置
	DirectX::XMFLOAT3	vNorm;	//!< @brief 頂点法線
	DirectX::XMFLOAT2	vTex;	//!< @brief UV座標
	UINT		uBoneIndex[4];	//!< @brief ボーン番号
	float		fBoneWeight[4];	//!< @brief ボーン重み
	/*!
	@brief コンストラクタ。各要素を初期化。
	*/
	TFbxVertex()
	{
		vPos = DirectX::XMFLOAT3(0, 0, 0);
		vNorm = DirectX::XMFLOAT3(0, 0, 0);
		vTex = DirectX::XMFLOAT2(0, 0);
		uBoneIndex[0] = uBoneIndex[1] = uBoneIndex[2] = uBoneIndex[3] = 0;
		fBoneWeight[0] = fBoneWeight[1] = fBoneWeight[2] = fBoneWeight[3] = 0.0f;
	}
};

/*! @struct TBone
	@brief ボーンの情報を定義。
*/
struct TBone {
	DirectX::XMFLOAT4X4 mBindPose;	//!< @brief 初期ポーズ (ずっと変わらない)
	DirectX::XMFLOAT4X4 mNewPose;	//!< @brief 現在のポーズ (その都度変わる)
	/*!
	@brief コンストラクタ。各要素を初期化。
	*/
	TBone()
	{
		XMStoreFloat4x4(&mBindPose, DirectX::XMMatrixIdentity());
		mNewPose = mBindPose;
	}
};

/*! @struct TSkinInfo {
	@brief スキンメッシュ用の情報定義。
*/
struct TSkinInfo {
	int				nNumBone; //!< @brief ボーンの数
	TBone*			pBoneArray; //!< @brief ボーンの配列用ポインタ
	FbxCluster**	ppCluster; //!< @brief クラスターリンクの配列用ポインタ

	/*!
	@brief コンストラクタ。各要素を初期化。
	*/
	TSkinInfo()
	{
		nNumBone = 0;
		pBoneArray = nullptr;
		ppCluster = nullptr;
	}
};

/*! @struct TPolyTable {
	@brief 1頂点の共有 (最大MAX_REF_POLYポリゴン)
*/
struct TPolyTable {
	int nPolyIndex[MAX_REF_POLY];	//!< @brief ポリゴン番号 
	int nIndex123[MAX_REF_POLY];	//!< @brief 3つの頂点のうち、何番目か
	int nNumRef;					//!< @brief 属しているポリゴン数

	/*!
	@brief コンストラクタ。各要素を初期化。
	*/
	TPolyTable()
	{
		ZeroMemory(nPolyIndex, sizeof(nPolyIndex));
		ZeroMemory(nIndex123, sizeof(nIndex123));
		nNumRef = 0;
	}
};

/*! @struct SHADER_GLOBAL
	@brief シェーダに渡す値
*/
struct SHADER_GLOBAL {
	DirectX::XMMATRIX	mWVP;		// ワールド×ビュー×射影行列(転置行列)
	DirectX::XMMATRIX	mW;			// ワールド行列(転置行列)
	DirectX::XMVECTOR	vEye;		// 視点座標
	DirectX::XMVECTOR	vLightDir;	// 光源方向
	DirectX::XMVECTOR	vLa;		// 光源色(アンビエント)
	DirectX::XMVECTOR	vLd;		// 光源色(ディフューズ)
	DirectX::XMVECTOR	vLs;		// 光源色(スペキュラ)
};

// 
/*! @struct SHADER_MATERIAL
	@brief マテリアル (シェーダ用)
*/
struct SHADER_MATERIAL {
	DirectX::XMVECTOR	vAmbient;	// アンビエント色
	DirectX::XMVECTOR	vDiffuse;	// ディフューズ色
	DirectX::XMVECTOR	vSpecular;	// スペキュラ色
	DirectX::XMVECTOR	vEmissive;	// エミッシブ色
};

/*! @struct SHADER_BONE
	@brief シェーダに渡すボーン行列配列
*/
struct SHADER_BONE {
	DirectX::XMMATRIX mBone[MAX_BONE_MATRIX];
	SHADER_BONE()
	{
		for (int i = 0; i < MAX_BONE_MATRIX; i++) {
			mBone[i] = DirectX::XMMatrixIdentity();
		}
	}
};