/**
 * @file Scene.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Camera.h"
#include "FbxLight.h"
#include "GameObj.h"

#include <memory>
#include <vector>


class C_GameObject;
 /**
  * @class C_Scene
  * @brief �V�[���̃x�[�X�N���X�B��{�I�ɃV�[���̊�{�I�ȓ����͓����Ȃ̂ŁA
  *		   �R���X�g���N�^�ŃV�[�����Ƃ̃I�u�W�F�N�g�𐶐�����`�őΉ�����B
  *		   �V�[�����ƂɁu�ǂ̃I�u�W�F�N�g��u�����v�ȊO�̋@�\�͂��ׂċ��ʂȂ̂�
  *		   �x�[�X�N���X�ɂ��ׂĂ̋@�\���������A�q�N���X��private�Ōp������
  */
class C_Scene {
private:
	bool m_bFirst = true;
	std::unique_ptr<C_Camera> sceneCamera;	//!< @brief �V�[���Ŏg�p����J����
	std::unique_ptr<C_FbxLight> sceneLight;	//!< @brief �V�[���Ŏg�p����J����
	std::vector<std::unique_ptr<C_GameObject>> sceneObjList;	//!< @brief �V�[����̑S�I�u�W�F�N�g
	std::vector<std::unique_ptr<C_GameObject>> addObjList;	//!< @brief �V�[���ɒǉ��\��̃I�u�W�F�N�g(���[�u����ŏ��L�����ړ�������)
	std::vector<int> deleteObjList;	//!< @brief �V�[������폜�\��̃I�u�W�F�N�g�̔ԍ�
public:
	C_Scene();

	/**
	 * @brief �I�[�o�[���C�h�p�A�S�ẴV�[���I�u�W�F�N�g�𐶐����鏃��
	*/
	virtual void SpawnSceneObject() = 0;
	C_Scene(C_Camera* camera, C_FbxLight* light);
	~C_Scene();
	C_Camera* GetCamera();
	C_FbxLight* GetLight();
	void Init();
	void Update();
	void Draw();
	void Draw2D();
	void DeleteObject(int objID);
	void AddObject(std::unique_ptr<C_GameObject> obj);
};