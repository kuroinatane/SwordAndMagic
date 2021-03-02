#pragma once
/**
 * @file Utility.h
 * @brief 汎用的な関数、定数を定義して使う。
 *		  インクルードファイルの増加、循環参照を避けるために使用。
 * @author MIURA DAIKI
 * @date 2020/12/07
 */

#include "Main.h"


 // 頂点フォーマット( 頂点座標[3D] / 法線ベクトル / 反射光 / テクスチャ座標 )
typedef struct {
	DirectX::XMFLOAT3 vtx;		// 頂点座標
	DirectX::XMFLOAT3 nor;		// 法線ベクトル
	DirectX::XMFLOAT4 diffuse;	// 拡散反射光
	DirectX::XMFLOAT2 tex;		// テクスチャ座標
} VERTEX_3D;

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
typedef struct {
	DirectX::XMFLOAT3 vtx;		// 頂点座標
	DirectX::XMFLOAT4 diffuse;	// 拡散反射光
	DirectX::XMFLOAT2 tex;		// テクスチャ座標
} VERTEX_2D;

DirectX::XMFLOAT2 GetScreenSize();
DirectX::XMFLOAT2 GetScreenAspect();