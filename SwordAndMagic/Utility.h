#pragma once
/**
 * @file Utility.h
 * @brief �ėp�I�Ȋ֐��A�萔���`���Ďg���B
 *		  �C���N���[�h�t�@�C���̑����A�z�Q�Ƃ�����邽�߂Ɏg�p�B
 * @author MIURA DAIKI
 * @date 2020/12/07
 */

#include "Main.h"


 // ���_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	DirectX::XMFLOAT3 vtx;		// ���_���W
	DirectX::XMFLOAT3 nor;		// �@���x�N�g��
	DirectX::XMFLOAT4 diffuse;	// �g�U���ˌ�
	DirectX::XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	DirectX::XMFLOAT3 vtx;		// ���_���W
	DirectX::XMFLOAT4 diffuse;	// �g�U���ˌ�
	DirectX::XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

DirectX::XMFLOAT2 GetScreenSize();
DirectX::XMFLOAT2 GetScreenAspect();