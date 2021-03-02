/**
 * @file FbxUtility.h
 * @brief FBXSDK�n�R���|�[�l���g�ŌĂяo���p�̍\���̂�萔��`�A�C���N���[�h�B
 * @author MIURA DAIKI
 * @date 2020/12/14
 */
#pragma once
#include "main.h"
#include <fbxsdk.h>
#include <wrl/client.h>


constexpr int MAX_REF_POLY = 60;	// �ő勤�L���_�Q�Ɛ�
constexpr int MAX_BONE_MATRIX = 256;

/*! @enum EByOpacity
	@brief �����x�ɂ��`�敪���Ɏg�p(�s�����A�����ŕ����ĕ`�悷��)�B  
*/
enum EByOpacity {
	eNoAffect = 0,		//!< @brief �S�ĕ`��
	eOpacityOnly,		//!< @brief �s�����̂ݕ`��
	eTransparentOnly,	//!< @brief �����̂ݕ`��
};

/*! @struct TFbxMaterial
	@brief FBX�̃}�e���A�����`�B
*/
struct TFbxMaterial {
	DirectX::XMFLOAT4	Ka;		//!< @brief  �A���r�G���g+�e�N�X�`���L��
	DirectX::XMFLOAT4	Kd;		//!< @brief  �f�B�t���[�Y
	DirectX::XMFLOAT4	Ks;		//!< @brief  �X�y�L����+�X�y�L�������x
	DirectX::XMFLOAT4	Ke;		//!< @brief  �G�~�b�V�u
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pTexture;		//!< @brief  �g�U�e�N�X�`��
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView>	pTexEmmisive;	//!< @brief  �����e�N�X�`��
	DWORD		dwNumFace;		//!< @brief  ���̃}�e���A���̃|���S����
	/*! 
	@brief �R���X�g���N�^�B�e�v�f���������B
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
	@brief FBX�̒��_�����`�B
*/
struct TFbxVertex {
	DirectX::XMFLOAT3	vPos;	//!< @brief ���_�ʒu
	DirectX::XMFLOAT3	vNorm;	//!< @brief ���_�@��
	DirectX::XMFLOAT2	vTex;	//!< @brief UV���W
	UINT		uBoneIndex[4];	//!< @brief �{�[���ԍ�
	float		fBoneWeight[4];	//!< @brief �{�[���d��
	/*!
	@brief �R���X�g���N�^�B�e�v�f���������B
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
	@brief �{�[���̏����`�B
*/
struct TBone {
	DirectX::XMFLOAT4X4 mBindPose;	//!< @brief �����|�[�Y (�����ƕς��Ȃ�)
	DirectX::XMFLOAT4X4 mNewPose;	//!< @brief ���݂̃|�[�Y (���̓s�x�ς��)
	/*!
	@brief �R���X�g���N�^�B�e�v�f���������B
	*/
	TBone()
	{
		XMStoreFloat4x4(&mBindPose, DirectX::XMMatrixIdentity());
		mNewPose = mBindPose;
	}
};

/*! @struct TSkinInfo {
	@brief �X�L�����b�V���p�̏���`�B
*/
struct TSkinInfo {
	int				nNumBone; //!< @brief �{�[���̐�
	TBone*			pBoneArray; //!< @brief �{�[���̔z��p�|�C���^
	FbxCluster**	ppCluster; //!< @brief �N���X�^�[�����N�̔z��p�|�C���^

	/*!
	@brief �R���X�g���N�^�B�e�v�f���������B
	*/
	TSkinInfo()
	{
		nNumBone = 0;
		pBoneArray = nullptr;
		ppCluster = nullptr;
	}
};

/*! @struct TPolyTable {
	@brief 1���_�̋��L (�ő�MAX_REF_POLY�|���S��)
*/
struct TPolyTable {
	int nPolyIndex[MAX_REF_POLY];	//!< @brief �|���S���ԍ� 
	int nIndex123[MAX_REF_POLY];	//!< @brief 3�̒��_�̂����A���Ԗڂ�
	int nNumRef;					//!< @brief �����Ă���|���S����

	/*!
	@brief �R���X�g���N�^�B�e�v�f���������B
	*/
	TPolyTable()
	{
		ZeroMemory(nPolyIndex, sizeof(nPolyIndex));
		ZeroMemory(nIndex123, sizeof(nIndex123));
		nNumRef = 0;
	}
};

/*! @struct SHADER_GLOBAL
	@brief �V�F�[�_�ɓn���l
*/
struct SHADER_GLOBAL {
	DirectX::XMMATRIX	mWVP;		// ���[���h�~�r���[�~�ˉe�s��(�]�u�s��)
	DirectX::XMMATRIX	mW;			// ���[���h�s��(�]�u�s��)
	DirectX::XMVECTOR	vEye;		// ���_���W
	DirectX::XMVECTOR	vLightDir;	// ��������
	DirectX::XMVECTOR	vLa;		// �����F(�A���r�G���g)
	DirectX::XMVECTOR	vLd;		// �����F(�f�B�t���[�Y)
	DirectX::XMVECTOR	vLs;		// �����F(�X�y�L����)
};

// 
/*! @struct SHADER_MATERIAL
	@brief �}�e���A�� (�V�F�[�_�p)
*/
struct SHADER_MATERIAL {
	DirectX::XMVECTOR	vAmbient;	// �A���r�G���g�F
	DirectX::XMVECTOR	vDiffuse;	// �f�B�t���[�Y�F
	DirectX::XMVECTOR	vSpecular;	// �X�y�L�����F
	DirectX::XMVECTOR	vEmissive;	// �G�~�b�V�u�F
};

/*! @struct SHADER_BONE
	@brief �V�F�[�_�ɓn���{�[���s��z��
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