#include "Curtain.h"
#include "Texture.h"
#include <time.h>
#include "input.h"
#include "GameObj.h"
#include "GraphicsManager.h"
#include "SceneController.h"


CurtainComponent::CurtainComponent(C_GameObject* obj, int dir, bool isTitle) : C_Mesh(obj){
	//SetTexture(L"data/texture/HeightMap.png");
	m_isTitle = isTitle;
	obj->SetSortOrder(1);
	m_dir = dir;
	m_material.Diffuse = DirectX::XMFLOAT4(1.0f,0.0f,0.0f,1.0f);
	m_WindForce.force = DirectX::XMFLOAT3(0,0,-5.0f);
	m_bWind = true;
	for (int j = 0; j <= PARTICLE_VERTICAL; j++) {
		for (int i = 0; i <= PARTICLE_HORIZONTAL; i++) {
			// ���W��ݒ�
			m_Cloth.Particle[j][i].position.x = SPRING_DEFAULT_LENGTH * (i - PARTICLE_HORIZONTAL / 2);
			m_Cloth.Particle[j][i].position.y = SPRING_DEFAULT_LENGTH * (j - PARTICLE_VERTICAL / 2);
			m_Cloth.Particle[j][i].position.z = 0.0f;
			if (i % 5 == 0 && i % 10 != 0) {
				m_Cloth.Particle[j][i].position.z = -10.0f;
			}
			m_Cloth.Particle[j][i].velocity = DirectX::XMFLOAT3(0, 0, 0);
			m_Cloth.Particle[j][i].acceleration = DirectX::XMFLOAT3(0, 0, 0);
			m_Cloth.Particle[j][i].resultant = DirectX::XMFLOAT3(0, 0, 0);
			m_Cloth.Particle[j][i].gravity = DirectX::XMFLOAT3(0, SPRING_MASS * 2.8f, 0);
			/*if (j == 0 && (i != 0)) {
				m_Cloth.Particle[j][i].velocity.x = -i * 2.0f;
			}*/
			// ����̗��q�̓��b�N��Ԃ�
			m_Cloth.Particle[j][i].onLockX = (((j == 0) && (i % 5 == 0)));	// ���b�N
			m_Cloth.Particle[j][i].onLockY = (((j == 0) && (i % 5 == 0)));	// ���b�N
			m_Cloth.Particle[j][i].onLockZ = (((j == 0) && (i % 5 == 0)));	// ���b�N
		}
	}

	// �o�l�̏�����
	int count = 0;
	float xx, yy, zz;
	for (int j = 0; j <= PARTICLE_VERTICAL; j++) {
		for (int i = 0; i <= PARTICLE_HORIZONTAL; i++) {
			// �o�l�Q�ƃC���f�b�N�X�̐ݒ�
			if (i < PARTICLE_HORIZONTAL) {
				m_Cloth.Spring[count].p1.horz = i;		// �P�ڂ̎Q�Ɨ��q�C���f�b�N�X��ݒ�
				m_Cloth.Spring[count].p1.vert = j;
				m_Cloth.Spring[count].p2.horz = i + 1;	// �Q�ڂ̎Q�Ɨ��q�C���f�b�N�X��ݒ�
				m_Cloth.Spring[count].p2.vert = j;
				// �Q�̗��q�Ԃ̋��������߁A�o�l�̎��R���Ƃ���
				xx = m_Cloth.Particle[j][i].position.x - m_Cloth.Particle[j][i + 1].position.x;
				yy = m_Cloth.Particle[j][i].position.y - m_Cloth.Particle[j][i + 1].position.y;
				zz = m_Cloth.Particle[j][i].position.z - m_Cloth.Particle[j][i + 1].position.z;
				m_Cloth.Spring[count].length = sqrtf(xx*xx + yy * yy + zz * zz);
				count++;
			}
			if (j < PARTICLE_VERTICAL) {
				m_Cloth.Spring[count].p1.horz = i;
				m_Cloth.Spring[count].p1.vert = j;
				m_Cloth.Spring[count].p2.horz = i;
				m_Cloth.Spring[count].p2.vert = j + 1;
				xx = m_Cloth.Particle[j][i].position.x - m_Cloth.Particle[j + 1][i].position.x;
				yy = m_Cloth.Particle[j][i].position.y - m_Cloth.Particle[j + 1][i].position.y;
				zz = m_Cloth.Particle[j][i].position.z - m_Cloth.Particle[j + 1][i].position.z;
				m_Cloth.Spring[count].length = sqrtf(xx*xx + yy * yy + zz * zz);
				count++;
			}
			if (i < PARTICLE_HORIZONTAL && j < PARTICLE_VERTICAL) {
				m_Cloth.Spring[count].p1.horz = i;
				m_Cloth.Spring[count].p1.vert = j;
				m_Cloth.Spring[count].p2.horz = i + 1;
				m_Cloth.Spring[count].p2.vert = j + 1;
				xx = m_Cloth.Particle[j][i].position.x - m_Cloth.Particle[j + 1][i + 1].position.x;
				yy = m_Cloth.Particle[j][i].position.y - m_Cloth.Particle[j + 1][i + 1].position.y;
				zz = m_Cloth.Particle[j][i].position.z - m_Cloth.Particle[j + 1][i + 1].position.z;
				m_Cloth.Spring[count].length = sqrtf(xx*xx + yy * yy + zz * zz);
				count++;
			}
			if (i > 0 && j < PARTICLE_VERTICAL) {
				m_Cloth.Spring[count].p1.horz = i;
				m_Cloth.Spring[count].p1.vert = j;
				m_Cloth.Spring[count].p2.horz = i - 1;
				m_Cloth.Spring[count].p2.vert = j + 1;
				xx = m_Cloth.Particle[j][i].position.x - m_Cloth.Particle[j + 1][i - 1].position.x;
				yy = m_Cloth.Particle[j][i].position.y - m_Cloth.Particle[j + 1][i - 1].position.y;
				zz = m_Cloth.Particle[j][i].position.z - m_Cloth.Particle[j + 1][i - 1].position.z;
				m_Cloth.Spring[count].length = sqrtf(xx*xx + yy * yy + zz * zz);
				count++;
			}
		}
	}
	m_Cloth.status = 1;
	VERTEX_3D m_pVertexWk[PARTICLE_VERTICAL + 1][PARTICLE_HORIZONTAL + 1];
	m_nNumVertex = (PARTICLE_VERTICAL + 1) * (PARTICLE_HORIZONTAL + 1);
	for (int j = 0; j <= PARTICLE_VERTICAL; j++) {
		for (int i = 0; i <= PARTICLE_HORIZONTAL; i++) {
			m_pVertexWk[j][i].vtx = DirectX::XMFLOAT3(m_Cloth.Particle[j][i].position.x, m_Cloth.Particle[j][i].position.y, m_Cloth.Particle[j][i].position.z);
			m_pVertexWk[j][i].nor = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_pVertexWk[j][i].diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f);
			m_pVertexWk[j][i].tex = DirectX::XMFLOAT2((float)i/ PARTICLE_HORIZONTAL,(float)j/ PARTICLE_VERTICAL);
		}
	}
	m_nNumIndex = (PARTICLE_HORIZONTAL)*(PARTICLE_VERTICAL) * 6;
	int m_pIndexWk[PARTICLE_HORIZONTAL][PARTICLE_VERTICAL][6];
	for (int j = 0; j < PARTICLE_VERTICAL; j++) {
		for (int i = 0; i < PARTICLE_HORIZONTAL; i++) {
			m_pIndexWk[j][i][0] = (j* (PARTICLE_HORIZONTAL + 1)) + i;
			m_pIndexWk[j][i][1] = (j* (PARTICLE_HORIZONTAL + 1)) + i + 1;
			m_pIndexWk[j][i][2] = (j + 1) * (PARTICLE_HORIZONTAL + 1) + i;
			m_pIndexWk[j][i][3] = (j + 1) * (PARTICLE_HORIZONTAL + 1) + i;
			m_pIndexWk[j][i][4] = (j* (PARTICLE_HORIZONTAL + 1)) + i + 1;
			m_pIndexWk[j][i][5] = (j + 1) * (PARTICLE_HORIZONTAL + 1) + i + 1;
		}
	}
	HRESULT hr = MakeMeshVertex(&(**m_pVertexWk),&(***m_pIndexWk));
	if (!hr) {
		hr = S_OK;
	}
	for (int i = 0; i < 2000; i++) {
		//���R����������
		CurtainUpdate();
	}
	if (!m_isTitle) {
		StartOpen();
	}
}

CurtainComponent::~CurtainComponent() {

}

void CurtainComponent::StartOpen() {
	m_moveCnt = 0;
	if (m_curtainState == CURTAIN_CLOSED) {
		m_curtainState = CURTAIN_OPENING;
	}
	m_isMoving = true;
}

void CurtainComponent::StartClose() {
	if (m_curtainState == CURTAIN_OPENED) {
		m_moveCnt = 0;
		m_curtainState = CURTAIN_CLOSING;
		m_isMoving = true;
	}

}

void CurtainComponent::Update() {
	if (m_isTitle) {
		if (InputManager::GetInstance().GetKeyPress(VK_RETURN)) {
			m_isMoving = true;
			m_curtainState = CURTAIN_OPENING;
		}
		if (InputManager::GetInstance().GetKeyPress(VK_A)) {
			StartClose();
		}
	}
	CurtainUpdate();
	VERTEX_3D m_pVertexWk[PARTICLE_VERTICAL + 1][PARTICLE_HORIZONTAL + 1];
	m_nNumVertex = (PARTICLE_VERTICAL + 1) * (PARTICLE_HORIZONTAL + 1);
	for (int j = 0; j <= PARTICLE_VERTICAL; j++) {
		for (int i = 0; i <= PARTICLE_HORIZONTAL; i++) {
			m_pVertexWk[j][i].vtx = DirectX::XMFLOAT3(m_Cloth.Particle[j][i].position.x, m_Cloth.Particle[j][i].position.y, m_Cloth.Particle[j][i].position.z);
			m_pVertexWk[j][i].nor = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_pVertexWk[j][i].diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			m_pVertexWk[j][i].tex = DirectX::XMFLOAT2((float)i / PARTICLE_HORIZONTAL, (float)j / PARTICLE_VERTICAL);
		}
	}
	//���X�Ɩ@�����v�Z����
	for (int j = 0; j <= PARTICLE_VERTICAL; j++) {
		for (int i = 0; i <= PARTICLE_HORIZONTAL; i++) {
			if (j == 0) {
				if (i == 0) {
					DirectX::XMVECTOR vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i + 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i].position.z - m_Cloth.Particle[j][i].position.z))));
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&m_pVertexWk[j][i].nor, vec);
				}else if (i == PARTICLE_HORIZONTAL) {
					DirectX::XMFLOAT3 vec1;
					DirectX::XMFLOAT3 vec2;
					DirectX::XMVECTOR vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i - 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i - 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i - 1].position.z - m_Cloth.Particle[j][i].position.z)))
						);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec1, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i - 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i - 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i - 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i - 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i - 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i - 1].position.z - m_Cloth.Particle[j][i].position.z)))
						
						
						);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec2, vec);
					vec1.x += vec2.x;
					vec1.y += vec2.y;
					vec1.z += vec2.z;
					vec = DirectX::XMVector3Normalize(XMLoadFloat3(&vec1));
					DirectX::XMStoreFloat3(&m_pVertexWk[j][i].nor, vec);
				}
				else {
					DirectX::XMFLOAT3 vec1;
					DirectX::XMFLOAT3 vec2;
					DirectX::XMFLOAT3 vec3;
					DirectX::XMVECTOR vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i + 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i].position.z - m_Cloth.Particle[j][i].position.z))));
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec1, vec);

					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i - 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i - 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i - 1].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec2, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i - 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i - 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i - 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i - 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i - 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i - 1].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec3, vec);
					vec1.x += vec2.x;
					vec1.y += vec2.y;
					vec1.z += vec2.z;
					vec1.x += vec3.x;
					vec1.y += vec3.y;
					vec1.z += vec3.z;
					vec = DirectX::XMVector3Normalize(XMLoadFloat3(&vec1));
					DirectX::XMStoreFloat3(&m_pVertexWk[j][i].nor, vec);
				}
			}else if (j == PARTICLE_VERTICAL) {
				//�Ō�̗�
				if (i == PARTICLE_HORIZONTAL) {
					DirectX::XMVECTOR vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i - 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i - 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j-1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j-1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j-1][i].position.z - m_Cloth.Particle[j][i].position.z)))
						);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&m_pVertexWk[j][i].nor, vec);
				}
				else if (i == 0) {
					DirectX::XMFLOAT3 vec1;
					DirectX::XMFLOAT3 vec2;
					DirectX::XMVECTOR vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i + 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i + 1].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec1, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i + 1].position.z - m_Cloth.Particle[j][i].position.z)))

					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec2, vec);
					vec1.x += vec2.x;
					vec1.y += vec2.y;
					vec1.z += vec2.z;
					vec = DirectX::XMVector3Normalize(XMLoadFloat3(&vec1));
					DirectX::XMStoreFloat3(&m_pVertexWk[j][i].nor, vec);
				}
				else {
					DirectX::XMFLOAT3 vec1;
					DirectX::XMFLOAT3 vec2;
					DirectX::XMFLOAT3 vec3;
					DirectX::XMVECTOR vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i - 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i - 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec1, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i + 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i + 1].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec2, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i + 1].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec3, vec);
					vec1.x += vec2.x;
					vec1.y += vec2.y;
					vec1.z += vec2.z;
					vec1.x += vec3.x;
					vec1.y += vec3.y;
					vec1.z += vec3.z;
					vec = DirectX::XMVector3Normalize(XMLoadFloat3(&vec1));
					DirectX::XMStoreFloat3(&m_pVertexWk[j][i].nor, vec);
				}
			}
			else {
				if (i == 0) {
					DirectX::XMFLOAT3 vec1;
					DirectX::XMFLOAT3 vec2;
					DirectX::XMFLOAT3 vec3;
					DirectX::XMVECTOR vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i + 1].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec1, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i + 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i + 1].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec2, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i+1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i+1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i+1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec3, vec);
					vec1.x += vec2.x;
					vec1.y += vec2.y;
					vec1.z += vec2.z;
					vec1.x += vec3.x;
					vec1.y += vec3.y;
					vec1.z += vec3.z;
					vec = DirectX::XMVector3Normalize(XMLoadFloat3(&vec1));
					DirectX::XMStoreFloat3(&m_pVertexWk[j][i].nor, vec);
				}
				else if (i == PARTICLE_HORIZONTAL) {
					DirectX::XMFLOAT3 vec1;
					DirectX::XMFLOAT3 vec2;
					DirectX::XMFLOAT3 vec3;
					DirectX::XMVECTOR vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i - 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i - 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i - 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec1, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i - 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i - 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i - 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i - 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i - 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i - 1].position.z - m_Cloth.Particle[j][i].position.z)))
						
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec2, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i - 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i - 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i - 1].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec3, vec);
					vec1.x += vec2.x;
					vec1.y += vec2.y;
					vec1.z += vec2.z;
					vec1.x += vec3.x;
					vec1.y += vec3.y;
					vec1.z += vec3.z;
					vec = DirectX::XMVector3Normalize(XMLoadFloat3(&vec1));
					DirectX::XMStoreFloat3(&m_pVertexWk[j][i].nor, vec);
				}
				else {
					DirectX::XMFLOAT3 vec1;
					DirectX::XMFLOAT3 vec2;
					DirectX::XMFLOAT3 vec3;
					DirectX::XMFLOAT3 vec4;
					DirectX::XMFLOAT3 vec5;
					DirectX::XMFLOAT3 vec6;

					DirectX::XMVECTOR vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i + 1].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec1, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i + 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i + 1].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec2, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i + 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec3, vec);

					DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i + 1].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec4, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j - 1][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j - 1][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j - 1][i + 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i + 1].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec5, vec);
					vec = DirectX::XMVector3Cross(
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j][i + 1].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j][i + 1].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j][i + 1].position.z - m_Cloth.Particle[j][i].position.z))),
						DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(m_Cloth.Particle[j + 1][i].position.x - m_Cloth.Particle[j][i].position.x,
							m_Cloth.Particle[j + 1][i].position.y - m_Cloth.Particle[j][i].position.y,
							m_Cloth.Particle[j + 1][i].position.z - m_Cloth.Particle[j][i].position.z)))
					);
					vec = DirectX::XMVector3Normalize(vec);
					DirectX::XMStoreFloat3(&vec6, vec);

					vec1.x += vec2.x;
					vec1.y += vec2.y;
					vec1.z += vec2.z;
					vec1.x += vec3.x;
					vec1.y += vec3.y;
					vec1.z += vec3.z;
					vec1.x += vec4.x;
					vec1.y += vec4.y;
					vec1.z += vec4.z;
					vec1.x += vec5.x;
					vec1.y += vec5.y;
					vec1.z += vec5.z;
					vec1.x += vec6.x;
					vec1.y += vec6.y;
					vec1.z += vec6.z;
					vec = DirectX::XMVector3Normalize(XMLoadFloat3(&vec1));
					DirectX::XMStoreFloat3(&m_pVertexWk[j][i].nor, vec);
				}
			}
		}
	}
	m_nNumIndex = (PARTICLE_HORIZONTAL)*(PARTICLE_VERTICAL) * 6;
	//int* m_pIndexWk = new int[(PARTICLE_HORIZONTAL)*(PARTICLE_VERTICAL) * 6];
	int m_pIndexWk[PARTICLE_HORIZONTAL][PARTICLE_VERTICAL][6];
	for (int j = 0; j < PARTICLE_VERTICAL; j++) {
		for (int i = 0; i < PARTICLE_HORIZONTAL; i++) {
			m_pIndexWk[j][i][0] = (j* (PARTICLE_HORIZONTAL + 1)) + i;
			m_pIndexWk[j][i][1] = (j* (PARTICLE_HORIZONTAL + 1)) + i + 1;
			m_pIndexWk[j][i][2] = (j + 1) * (PARTICLE_HORIZONTAL + 1) + i;
			m_pIndexWk[j][i][3] = (j + 1) * (PARTICLE_HORIZONTAL + 1) + i;
			m_pIndexWk[j][i][4] = (j* (PARTICLE_HORIZONTAL + 1)) + i + 1;
			m_pIndexWk[j][i][5] = (j + 1) * (PARTICLE_HORIZONTAL + 1) + i + 1;
		}
	}
	HRESULT hr = MakeMeshVertex(&(**m_pVertexWk), &(***m_pIndexWk));

	if(m_curtainState == CURTAIN_CLOSING || m_curtainState == CURTAIN_OPENING) m_moveCnt++;

	C_Mesh::Update();
}

void CurtainComponent::CurtainUpdate() {
	// ���q�̏���
	DirectX::XMFLOAT3 resistance;
	DirectX::XMFLOAT3 windforce;
	for (int j = 0; j <= PARTICLE_VERTICAL; j++) {
		for (int i = 0; i <= PARTICLE_HORIZONTAL; i++) {
			// ���͂��O�ɂ���
			m_Cloth.Particle[j][i].resultant.x = 0;
			m_Cloth.Particle[j][i].resultant.y = 0;
			m_Cloth.Particle[j][i].resultant.z = 0;

			if (true) {
				// �d�͂����߂�
				m_Cloth.Particle[j][i].gravity = DirectX::XMFLOAT3(0.0f, SPRING_MASS * -2.8f, 0.0f);

				// ��R�͂����߂�
				resistance.x = -m_Cloth.Particle[j][i].velocity.x * ATT_COEFFICIENT;
				resistance.y = -m_Cloth.Particle[j][i].velocity.y * ATT_COEFFICIENT;
				resistance.z = -m_Cloth.Particle[j][i].velocity.z * ATT_COEFFICIENT;

				// ���͂����߂�
				if (m_bWind) {
					windforce.x = m_WindForce.force.x + (((rand() % 100) / 99.0f) - 0.25f) * m_WindForce.force.x;		// �w����
					windforce.y = m_WindForce.force.y + (((rand() % 100) / 99.0f) - 0.25f) * m_WindForce.force.y;		// �x����
					windforce.z = m_WindForce.force.z + (((rand() % 100) / 99.0f) - 0.25f)* m_WindForce.force.z;		// �y����
				}
				else {
					windforce.x = 0;
					windforce.y = 0;
					windforce.z = 0;
				}

				// ���́��d�́{��R�́{����
				m_Cloth.Particle[j][i].resultant.x = m_Cloth.Particle[j][i].gravity.x + resistance.x + windforce.x;
				m_Cloth.Particle[j][i].resultant.y = m_Cloth.Particle[j][i].gravity.y + resistance.y + windforce.y;
				m_Cloth.Particle[j][i].resultant.z = m_Cloth.Particle[j][i].gravity.z + resistance.z + windforce.z;
			}
		}
	}

	// �o�l�̏���
	int h1, v1, h2, v2;
	float f1, f2, f1x, f1y, f1z, f2x, f2y, f2z;
	DirectX::XMFLOAT3 vec_spr;
	float length;
	for (int i = 0; i < SPRING_NUMS; i++) {
		// �o�l�̎Q�Ɨ��q�i�Q�j���擾
		h1 = m_Cloth.Spring[i].p1.horz;
		v1 = m_Cloth.Spring[i].p1.vert;
		h2 = m_Cloth.Spring[i].p2.horz;
		v2 = m_Cloth.Spring[i].p2.vert;
		// �Q�̗��q�Ԃ̃x�N�g�������߂�
		vec_spr.x = m_Cloth.Particle[v1][h1].position.x - m_Cloth.Particle[v2][h2].position.x;
		vec_spr.y = m_Cloth.Particle[v1][h1].position.y - m_Cloth.Particle[v2][h2].position.y;
		vec_spr.z = m_Cloth.Particle[v1][h1].position.z - m_Cloth.Particle[v2][h2].position.z;
		// ���q�Ԃ̋��������߂�
		length = sqrtf(vec_spr.x * vec_spr.x + vec_spr.y * vec_spr.y + vec_spr.z * vec_spr.z);
		// �����A���R���A�o�l�W�����炩����͂����߂�i�Q�ڂ͋t�����j
		f1 = -SPRING_COEFFICIENT * (length - m_Cloth.Spring[i].length);
		f2 = -f1;
		// �x�N�g���̐����ɗ͂�������
		f1x = f1 * (vec_spr.x / length);
		f1y = f1 * (vec_spr.y / length);
		f1z = f1 * (vec_spr.z / length);
		f2x = f2 * (vec_spr.x / length);
		f2y = f2 * (vec_spr.y / length);
		f2z = f2 * (vec_spr.z / length);
		// ���߂��͂����͂ɉ�����
		m_Cloth.Particle[v1][h1].resultant.x += f1x;
		m_Cloth.Particle[v1][h1].resultant.y += f1y;
		m_Cloth.Particle[v1][h1].resultant.z += f1z;
		m_Cloth.Particle[v2][h2].resultant.x += f2x;
		m_Cloth.Particle[v2][h2].resultant.y += f2y;
		m_Cloth.Particle[v2][h2].resultant.z += f2z;
	}
	for (int j = 0; j <= PARTICLE_VERTICAL; j++) {
		for (int i = 0; i <= PARTICLE_HORIZONTAL; i++) {
			// �Œ肳��Ă���Γ��ꏈ�������ăX�L�b�v
			if (m_moveCnt < 250) {
				if (m_Cloth.Particle[j][i].onLockX && m_Cloth.Particle[j][i].onLockY && m_Cloth.Particle[j][i].onLockZ) {
					//�Ȃɂ�����
					if (m_isMoving && m_moveCnt < 250) {
						if ((m_dir > 0 && m_curtainState == CURTAIN_OPENING)) {
							m_Cloth.Particle[j][i].position.x -= i * 0.2f * DELTA_TIME;
						}
						else if(m_dir < 0 && m_curtainState == CURTAIN_OPENING){
							m_Cloth.Particle[j][i].position.x += (PARTICLE_HORIZONTAL - i) * 0.2f * DELTA_TIME;
						}
						else if (m_dir > 0 && m_curtainState == CURTAIN_CLOSING) {
							m_Cloth.Particle[j][i].position.x += i * 0.2f * DELTA_TIME;
						}
						else if (m_dir < 0 && m_curtainState == CURTAIN_CLOSING) {
							m_Cloth.Particle[j][i].position.x -= (PARTICLE_HORIZONTAL - i) * 0.2f * DELTA_TIME;
						}

					}
					continue;
				}
			}
			else {
				if (m_curtainState == CURTAIN_OPENING) {
					m_curtainState = CURTAIN_OPENED;
				}
				if (m_curtainState == CURTAIN_CLOSING) {
					m_curtainState = CURTAIN_CLOSED;
					if (m_isTitle) {
						SceneController::GetInstance().ChangeScene(SceneController::Scene_Game);
					}
				}
			}
			


			// ���͂Ǝ��ʂ�������x�����߂�
			if (!m_Cloth.Particle[j][i].onLockX) {
				m_Cloth.Particle[j][i].acceleration.x = m_Cloth.Particle[j][i].resultant.x / SPRING_MASS;
			}
			if (!m_Cloth.Particle[j][i].onLockY) {
				m_Cloth.Particle[j][i].acceleration.y = m_Cloth.Particle[j][i].resultant.y / SPRING_MASS;
			}
			if (!m_Cloth.Particle[j][i].onLockZ) {
				m_Cloth.Particle[j][i].acceleration.z = m_Cloth.Particle[j][i].resultant.z / SPRING_MASS;
			}
			// ���x�ɉ����x��������
			if (!m_Cloth.Particle[j][i].onLockX) {
				m_Cloth.Particle[j][i].velocity.x += m_Cloth.Particle[j][i].acceleration.x * DELTA_TIME;
			}
			if (!m_Cloth.Particle[j][i].onLockY) {
				m_Cloth.Particle[j][i].velocity.y += m_Cloth.Particle[j][i].acceleration.y * DELTA_TIME;
			}
			if (!m_Cloth.Particle[j][i].onLockZ) {
				m_Cloth.Particle[j][i].velocity.z += m_Cloth.Particle[j][i].acceleration.z * DELTA_TIME;
			}

			// ���x������W���ړ�
			if (!m_Cloth.Particle[j][i].onLockX) {
				m_Cloth.Particle[j][i].position.x += m_Cloth.Particle[j][i].velocity.x * DELTA_TIME;
			}

			if (!m_Cloth.Particle[j][i].onLockY) {
				m_Cloth.Particle[j][i].position.y += m_Cloth.Particle[j][i].velocity.y * DELTA_TIME;
			}
			if (!m_Cloth.Particle[j][i].onLockZ) {
				m_Cloth.Particle[j][i].position.z += m_Cloth.Particle[j][i].velocity.z * DELTA_TIME;
			}

			if (m_Cloth.Particle[0][i].onLockX && m_Cloth.Particle[0][i].onLockY && m_Cloth.Particle[0][i].onLockZ && (i % 10) == 0) {
				//�Œ�_�̉��̓_��X���W��Z���W���Œ�_�̒����ɌŒ�
				m_Cloth.Particle[j][i].position.x = m_Cloth.Particle[0][i].position.x;
				m_Cloth.Particle[j][i].position.y = m_Cloth.Particle[0][i].position.y - (j * 5.0f);
				m_Cloth.Particle[j][i].position.z = m_Cloth.Particle[0][i].position.z;
			}
		}
	}
}

void CurtainComponent::Draw() {
	C_Mesh::Draw();
}