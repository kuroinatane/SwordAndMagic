#include "BasicMesh.h"

BasicMeshComponent::BasicMeshComponent(C_GameObject* obj, TCHAR* szTexture) : C_Mesh(obj) {

	SetTexture(szTexture);
	m_material.Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_nNumVertex = 4;
	m_nNumIndex = 6;

	// �I�u�W�F�N�g�̒��_�z��𐶐�
	VERTEX_3D pVertexWk[4];

	// ���_���W�̐ݒ�
	pVertexWk[0].vtx = DirectX::XMFLOAT3(-1.0f / 2, 1.0f / 2, 0.0f); //����
	pVertexWk[1].vtx = DirectX::XMFLOAT3(1.0f / 2, 1.0f / 2, 0.0f);//�E��
	pVertexWk[2].vtx = DirectX::XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);//����
	pVertexWk[3].vtx = DirectX::XMFLOAT3(1.0f / 2, -1.0f / 2, 0.0f);//�E��

	// �@���̐ݒ�
	pVertexWk[0].nor =DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor =DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor =DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor =DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

	// ���ˌ��̐ݒ�
	pVertexWk[0].diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVertexWk[0].tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = DirectX::XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = DirectX::XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = DirectX::XMFLOAT2(1.0f, 1.0f);

	// �C���f�b�N�X�z��𐶐�
	int pIndexWk[6];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 2;
	pIndexWk[4] = 1;
	pIndexWk[5] = 3;

	// ���_�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pVertexWk, pIndexWk);

}

BasicMeshComponent::BasicMeshComponent(C_GameObject* obj, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture) : C_Mesh(obj) {

	SetTexture(pTexture);
	m_material.Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_nNumVertex = 4;
	m_nNumIndex = 6;

	// �I�u�W�F�N�g�̒��_�z��𐶐�
	VERTEX_3D pVertexWk[4];

	// ���_���W�̐ݒ�
	pVertexWk[0].vtx = DirectX::XMFLOAT3(-1.0f / 2, 1.0f / 2, 0.0f); //����
	pVertexWk[1].vtx = DirectX::XMFLOAT3(1.0f / 2, 1.0f / 2, 0.0f);//�E��
	pVertexWk[2].vtx = DirectX::XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);//����
	pVertexWk[3].vtx = DirectX::XMFLOAT3(1.0f / 2, -1.0f / 2, 0.0f);//�E��

	// �@���̐ݒ�
	pVertexWk[0].nor = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

	// ���ˌ��̐ݒ�
	pVertexWk[0].diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVertexWk[0].tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = DirectX::XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = DirectX::XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = DirectX::XMFLOAT2(1.0f, 1.0f);

	// �C���f�b�N�X�z��𐶐�
	int pIndexWk[6];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 2;
	pIndexWk[4] = 1;
	pIndexWk[5] = 3;

	// ���_�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pVertexWk, pIndexWk);

}
BasicMeshComponent::~BasicMeshComponent(){

}
void BasicMeshComponent::Update() {
	C_Mesh::Update();
}

void BasicMeshComponent::ChangeTexture(TCHAR* szTexture) {
	SetTexture(szTexture);
}

void BasicMeshComponent::ChangeTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture) {
	SetTexture(pTexture);
}