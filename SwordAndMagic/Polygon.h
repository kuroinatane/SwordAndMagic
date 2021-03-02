/**
 * @file Curtain.h
 * @author MIURA DAIKI
 * @date 2020/12/15
 */
#pragma once

#include "main.h"
#include "Component.h"
#include "Utility.h"
#include <wrl/client.h>

 // �V�F�[�_�ɓn���l
struct POLYGON_SHADER {
	DirectX::XMMATRIX	mWorld;		// ���[���h�ϊ��s��(�]�u�s��)
	DirectX::XMMATRIX	mView;		// �r���[�ϊ��s��(�]�u�s��)
	DirectX::XMMATRIX	mProj;		// �ˉe�ϊ��s��(�]�u�s��)
	DirectX::XMMATRIX	mTex;		// �e�N�X�`���ϊ��s��(�]�u�s��)
};

class C_GameObject;
 /**
  * @class PolygonComponent
  * @brief 2D�|���S���`��p�̃N���X�BUI�p�B
  */
class PolygonComponent : public C_Component {
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pTexture;				// �e�N�X�`���ւ̃|�C���^
	VERTEX_2D					m_vertexWk[4];	// ���_���i�[���[�N

	DirectX::XMFLOAT4						m_colPolygon;			// �|���S���̒��_�J���[
	bool							m_bInvalidate;			// ���_�f�[�^�X�V�t���O

	DirectX::XMFLOAT2						m_posTexFrame;			// UV���W
	DirectX::XMFLOAT2						m_sizTexFrame;			// �e�N�X�`�����o�T�C�Y

	Microsoft::WRL::ComPtr <ID3D11Buffer>			m_pConstantBuffer;		// �萔�o�b�t�@
	Microsoft::WRL::ComPtr <ID3D11Buffer>				m_pVertexBuffer;		// ���_�o�b�t�@
	Microsoft::WRL::ComPtr <ID3D11SamplerState>			m_pSamplerState;		// �e�N�X�`�� �T���v��
	Microsoft::WRL::ComPtr <ID3D11VertexShader>			m_pVertexShader;		// ���_�V�F�[�_
	Microsoft::WRL::ComPtr < ID3D11InputLayout>			m_pInputLayout;			// ���_�t�H�[�}�b�g
	Microsoft::WRL::ComPtr < ID3D11PixelShader>			m_pPixelShader;			// �s�N�Z���V�F�[�_

	DirectX::XMFLOAT4X4					m_mTex;					// �e�N�X�`���ϊ��s��
	C_GameObject*					m_pObj;					// �I�u�W�F�N�g�ւ̃|�C���^
public:
	PolygonComponent(const wchar_t* szTexture, C_GameObject* obj);
	~PolygonComponent();
	void Update(void);
	void Draw();
	void SetPolygonTexture(const wchar_t* szTexture);
	void SetPolygonUV(float fU, float fV);
	void SetPolygonFrameSize(float fWidth, float fHeight);
	void SetPolygonColor(float fRed, float fGreen, float fBlue);
	void SetPolygonAlpha(float fAlpha);
	void MakeVertexPolygon();
	void SetVertexPolygon();
};
