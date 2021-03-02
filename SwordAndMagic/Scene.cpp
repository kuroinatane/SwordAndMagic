#include "Scene.h"
#include "Camera.h"
#include <algorithm>
#include "PhyXGenerator.h"
#include "EffectGenerator.h"

/**
 * @brief �R���X�g���N�^�A�J����������
 */
C_Scene::C_Scene() {
	sceneCamera.reset(new C_Camera());
	sceneLight.reset(new C_FbxLight());
	PhyXGenerator::GetInstance().CreatePhyXScene(30);
	sceneObjList.clear();
	EffectGenerator::EffectGenerator();
}

/**
 * @brief �R���X�g���N�^�A�J�����ݒ�
 */
C_Scene::C_Scene(C_Camera* camera, C_FbxLight* light) {
	sceneCamera.reset(camera);
	sceneLight.reset(light);

	sceneObjList.clear();
}

/**
 * @brief �f�X�g���N�^�A�S�ẴI�u�W�F�N�g�̎Q�Ƃ�j��
 */
C_Scene::~C_Scene() {
	//�S�ẴI�u�W�F�N�g�̎Q�Ƃ�j��
	sceneObjList.clear();
	addObjList.clear();
	deleteObjList.clear();
}

/**
 * @brief �J�������̎擾
 * @return �V�[�����g�p���Ă���J�����ւ̃|�C���^
 */
C_Camera* C_Scene::GetCamera() { 
	return sceneCamera.get();
}

/**
 * @brief ���C�������̎擾
 * @return �V�[�����g�p���Ă��郁�C�������ւ̃|�C���^
 */
C_FbxLight* C_Scene::GetLight() {
	return sceneLight.get();
}

/**
 * @brief AddObj�̏���
 */
void C_Scene::Init() {
	SpawnSceneObject();
	//�R���e�i����ɂȂ�܂Ń��[�v��������
	std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
	for (itr = addObjList.begin(); itr != addObjList.end(); itr++) {
		sceneObjList.push_back(std::move((*itr)));
	}
	addObjList.clear();

	for (itr = sceneObjList.begin(); itr != sceneObjList.end(); itr++) {
		(*itr)->Start();
	}
}

/**
 * @brief �V�[����̃I�u�W�F�N�g�S�Ă̍X�V���V�K�I�u�W�F�N�g�̒ǉ�
 */
void C_Scene::Update() {
	PhyXGenerator::GetInstance().Update();

	if (m_bFirst) {
		Init();
		m_bFirst = false;
	}
	sceneCamera->Update();
	std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
	for (itr = sceneObjList.begin(); itr != sceneObjList.end(); itr++) {
		(*itr)->Update();
	}

	for (itr = addObjList.begin(); itr != addObjList.end(); itr++) {
		(*itr)->Start();
		sceneObjList.push_back(std::move((*itr)));
	}
	addObjList.clear();

	//std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
	for (itr = sceneObjList.begin(); itr != sceneObjList.end();) {
		bool isDestroy = false;
		for (int deleteNum : deleteObjList) {
			if ((*itr)->GetID() == deleteNum) {
				isDestroy = true;
				break;
			}
		}
		if (isDestroy) {
			itr = sceneObjList.erase(itr);
		}
		else {
			itr++;
		}
	}
	deleteObjList.clear();
	EffectGenerator::GetInstance().Update();
}

/**
 * @brief �`��(3D���W��̃I�u�W�F�N�g)
 */
void C_Scene::Draw() {

	//�I�[�_�[�\�[�g
	std::sort(sceneObjList.begin(), sceneObjList.end(), [](const std::unique_ptr<C_GameObject>& lhoge, const std::unique_ptr<C_GameObject>& rhoge)
	{
		return lhoge->GetSortOrder() < rhoge->GetSortOrder();
	});

	std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
	for (itr = sceneObjList.begin(); itr != sceneObjList.end(); itr++) {
		if(((*itr)->Getis2D()) == false)
		(*itr)->Draw();
	}
	EffectGenerator::GetInstance().Draw();
}

/**
 * @brief �`��(�X�N���[�����W���2D�I�u�W�F�N�g)
 */
void C_Scene::Draw2D() {
	std::vector<std::unique_ptr<C_GameObject>>::iterator itr;
	for (itr = sceneObjList.begin(); itr != sceneObjList.end(); itr++) {
		if (((*itr)->Getis2D()) == true)
			(*itr)->Draw();
	}
}

/**
 * @brief �I�u�W�F�N�g�̍폜
 * @param[in] objID �폜����I�u�W�F�N�g�̔ԍ�
 */
void C_Scene::DeleteObject(int objID) {
	deleteObjList.push_back(objID);
}

/**
 * @brief �I�u�W�F�N�g�̒ǉ�
 * @param[in] obj make_unique�ō쐬�����I�u�W�F�N�g�̃��j�[�N�|�C���^
 */
void C_Scene::AddObject(std::unique_ptr<C_GameObject> obj) {
	addObjList.push_back(std::move(obj));
}