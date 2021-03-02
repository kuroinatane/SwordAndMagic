/**
 * @file FbxLight.h
 * @author MIURA DAIKI
 * @date 2020/12/14
 */
#pragma once
#include "Main.h"

 /**
  * @class C_FbxLight
  * @brief FBX用のライトを定義するクラス。
  */
class C_FbxLight
{
public:
	DirectX::XMFLOAT4 m_diffuse;
	DirectX::XMFLOAT4 m_ambient;
	DirectX::XMFLOAT4 m_specular;
	DirectX::XMFLOAT3 m_direction;

	/**
	* @brief 引数無しコンストラクタ。デフォルトでは白く発光する手前から奥への光。
	*/
	C_FbxLight() : m_diffuse(1.f, 1.f, 1.f, 1.f),
		m_ambient(0.8f, 0.8f, 0.8f, 1.f),
		m_specular(0.8f, 0.8f, 0.8f, 1.f),
		m_direction(0.0f, -0.5f, 0.5f)
	{
	}

	/**
	* @brief 引数付きコンストラクタ。
	@param[in] diffuse 拡散光
	@param[in] ambient 環境光
	@param[in] specular 反射光
	@param[in] direction 光の向き
	*/
	C_FbxLight(DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 direction) : m_diffuse(diffuse),
		m_ambient(ambient),
		m_specular(specular),
		m_direction(direction)
	{
	}
};