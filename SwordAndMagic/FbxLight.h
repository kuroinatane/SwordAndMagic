/**
 * @file FbxLight.h
 * @author MIURA DAIKI
 * @date 2020/12/14
 */
#pragma once
#include "Main.h"

 /**
  * @class C_FbxLight
  * @brief FBX�p�̃��C�g���`����N���X�B
  */
class C_FbxLight
{
public:
	DirectX::XMFLOAT4 m_diffuse;
	DirectX::XMFLOAT4 m_ambient;
	DirectX::XMFLOAT4 m_specular;
	DirectX::XMFLOAT3 m_direction;

	/**
	* @brief ���������R���X�g���N�^�B�f�t�H���g�ł͔������������O���牜�ւ̌��B
	*/
	C_FbxLight() : m_diffuse(1.f, 1.f, 1.f, 1.f),
		m_ambient(0.8f, 0.8f, 0.8f, 1.f),
		m_specular(0.8f, 0.8f, 0.8f, 1.f),
		m_direction(0.0f, -0.5f, 0.5f)
	{
	}

	/**
	* @brief �����t���R���X�g���N�^�B
	@param[in] diffuse �g�U��
	@param[in] ambient ����
	@param[in] specular ���ˌ�
	@param[in] direction ���̌���
	*/
	C_FbxLight(DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 direction) : m_diffuse(diffuse),
		m_ambient(ambient),
		m_specular(specular),
		m_direction(direction)
	{
	}
};