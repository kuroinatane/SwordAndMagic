/**
 * @file Mesh.h
 * @author MIURA DAIKI
 * @date 2020/12/15
 */
#pragma once
#include "main.h"
#include "Component.h"
#include "Utility.h"
#include <wrl/client.h>

 // ���b�V������V�F�[�_�ɓn���l
struct MESH_SHADER {
	DirectX::XMMATRIX	mWVP;		// ���[���h�~�r���[�~�ˉe�s��(�]�u�s��)
	DirectX::XMMATRIX	mW;			// ���[���h�s��(�]�u�s��)
	DirectX::XMMATRIX	mTex;		// �e�N�X�`���s��(�]�u�s��)
};
struct MESH_SHADER_2 {
	DirectX::XMVECTOR	vEye;		// ���_���W
	// ����
	DirectX::XMVECTOR	vLightDir;	// ��������
	DirectX::XMVECTOR	vLa;		// �����F(�A���r�G���g)
	DirectX::XMVECTOR	vLd;		// �����F(�f�B�t���[�Y)
	DirectX::XMVECTOR	vLs;		// �����F(�X�y�L����)
	// �}�e���A��
	DirectX::XMVECTOR	vAmbient;	// �A���r�G���g�F(+�e�N�X�`���L��)
	DirectX::XMVECTOR	vDiffuse;	// �f�B�t���[�Y�F
	DirectX::XMVECTOR	vSpecular;	// �X�y�L�����F(+�X�y�L�������x)
	DirectX::XMVECTOR	vEmissive;	// �G�~�b�V�u�F
};

typedef struct {
	DirectX::XMFLOAT4	Diffuse;		// Diffuse color RGBA
	DirectX::XMFLOAT4	Ambient;		// Ambient color RGB
	DirectX::XMFLOAT4	Specular;		// Specular 'shininess'
	DirectX::XMFLOAT4	Emissive;		// Emissive color RGB
	float		Power;			// Sharpness if specular highlight
} MATERIAL;


class C_GameObject;
 /**
  * @class C_Mesh
  * @brief ���b�V�����v���O�������Ő������Ďg�����߂̃x�[�X�N���X�B�R���X�g���N�^��MakeMeshVertex���Ăяo���Ďg�p����B
  */
class C_Mesh : public C_Component
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_pVertexBuffer;	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_pIndexBuffer;	// �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_pConstantBuffer[3];	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			m_pSamplerState;		// �e�N�X�`�� �T���v��
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			m_pVertexShader;		// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_pInputLayout;			// ���_�t�H�[�}�b�g
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			m_pPixelShader;			// �s�N�Z���V�F�[�_
	DirectX::XMFLOAT4X4 m_mtxTexture;					// �e�N�X�`�� �}�g���b�N�X
	DirectX::XMFLOAT4X4 m_mtxWorld;					// ���[���h�s��
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_pTexture;		// �e�N�X�`��
	C_GameObject* m_pObj;
protected:
	HRESULT MakeMeshVertex(VERTEX_3D* vtxWk, int* idxWk); //�q�N���X����Ăяo����p
	void SetTexture(const TCHAR* szFileName);
	void SetTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture);
	int m_nNumVertex;							// �����_��	
	int m_nNumIndex;							// ���C���f�b�N�X��
	MATERIAL						m_material;
public:
	C_Mesh(C_GameObject* obj);
	~C_Mesh();
	HRESULT Init(void);
	void Uninit(void);
	void Update();
	void Draw();
	
};

