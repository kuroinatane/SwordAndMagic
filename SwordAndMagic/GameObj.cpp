// �Q�[�� �I�u�W�F�N�g [GameObj.cpp]
#include <algorithm>
#include "GameObj.h"
#include "Scene.h"
#include "ObjectRegister.h"
#include "Component.h"
#include "SceneController.h"
#include "Scene.h"

using namespace DirectX;

/**
 * @brief �R���X�g���N�^�BObjectRegister�o�R��ID��U�蕪���Ă��炤�B
 * @param[in] �V�[���ւ̃|�C���^
 */
C_GameObject::C_GameObject(C_Scene* scene)
{
	objectID = ObjectRegister::GetInstance().RegisterObject();
	m_pScene = scene;
}

/**
 * @brief �f�X�g���N�^�BObjectRegister�Ɏg�p���Ă���ID��Ԃ��B
 */
C_GameObject::~C_GameObject()
{
	ObjectRegister::GetInstance().UnregisterObject(objectID);
}

/**
 * @brief �X�V�֐��BaddComponent�������ōs����B
 */
void C_GameObject::Update()
{
	std::vector<std::unique_ptr<C_Component>>::iterator itr;
	for (itr = componentList.begin(); itr != componentList.end(); itr++) {
		(*itr)->Update();
	}

	for (itr = addComponentList.begin(); itr != addComponentList.end(); itr++) {
		componentList.push_back(std::move(*itr));
	}
	addComponentList.clear();
	////�R���e�i����ɂȂ�܂Ń��[�v��������
	//while (!addComponentList.empty()) {
	//	//�擪�̗v�f���R���e�i���ֈړ�������
	//	componentList.push_back(std::move(addComponentList.front()));
	//}
	for (int col : currentColList) {
		auto colItr = std::find(oldColList.begin(), oldColList.end(),col);
		bool isExist = colItr != oldColList.end();
		if (!isExist) {
			//�����蔻��O��܂���
			std::vector<std::unique_ptr<C_Component>>::iterator itr;
			for (itr = componentList.begin(); itr != componentList.end(); itr++) {
				(*itr)->OnCollisionExit(m_pScene->GetObjectWithID(col));
			}
		}
	}
	oldColList.clear();
	oldColList = currentColList;
	currentColList.clear();


}
/**
 * @brief �`��֐�
 */
void C_GameObject::Draw()
{
	std::vector<std::unique_ptr<C_Component>>::iterator itr;
	for (itr = componentList.begin(); itr != componentList.end(); itr++) {
		(*itr)->Draw();
	}
}

/**
 * @brief ���������ɌĂ΂��悤�ɐݒ肳��Ă���Component�̊֐���S�ČĂ�
 */
void C_GameObject::Start() 
{
	std::vector<std::unique_ptr<C_Component>>::iterator itr;

	//����������addComponent����������
	for (itr = addComponentList.begin(); itr != addComponentList.end(); itr++) {
		componentList.push_back(std::move(*itr));
	}
	addComponentList.clear();

	for (itr = componentList.begin(); itr != componentList.end(); itr++) {
		(*itr)->Start();
	}
}

/**
 * @brief �I�u�W�F�N�g��j�����A�j�����ɌĂ΂��悤�ɐݒ肳��Ă���Component�̊֐���S�ČĂ�
 */
void C_GameObject::Destroy() 
{
	std::vector<std::unique_ptr<C_Component>>::iterator itr;
	for (itr = componentList.begin(); itr != componentList.end(); itr++) {
		(*itr)->OnDestroy();
	}
	//�V�[����ɑ��݂��Ă���΃V�[������폜���Ă��炤(���݂��Ă��Ȃ���Ώ���ɏ�����͂�)
	if(m_pScene)
	m_pScene->DeleteObject(objectID);
}

/**
 * @brief �쐬�����R���|�[�l���g��n���Ă�����ł̊Ǘ��ɐ؂�ւ���
 * @param[in] component make_unique�ō�����R���|�[�l���g�̃��j�[�N�|�C���^
 */
void C_GameObject::AddComponent(std::unique_ptr<C_Component> component) {
	addComponentList.push_back(std::move(component));
}

/**
 * @brief �I�u�W�F�N�g�̔ԍ����擾����B
 * @return �I�u�W�F�N�g�̔ԍ�
 */
int C_GameObject::GetID() 
{
	return objectID;
}

/**
 * @brief 2D�I�u�W�F�N�g(=�X�N���[�����W�ɒ��ڕ`�悷��)���ǂ����̃t���O��Ԃ��B(�`��p)
 * @return 2D�I�u�W�F�N�g�Ȃ�true/3D�I�u�W�F�N�g�Ȃ�false
 */
bool C_GameObject::Getis2D() {
	return is2D;
}

/**
 * @brief 2D�I�u�W�F�N�g(=�X�N���[�����W�ɒ��ڕ`�悷��)���ǂ����̃t���O���Z�b�g����B(�`��p)
 * @param[in] flg 2D�I�u�W�F�N�g�Ȃ�true�A3D�I�u�W�F�N�g�Ȃ�false
 */
void C_GameObject::Set2DFlg(bool flg) {
	is2D = flg;
}

/**
 * @brief �\�[�g�p�̔ԍ��擾�B
 * @return �\�[�g�p�̔ԍ� 
 */
int C_GameObject::GetSortOrder() {
	return sortOrder;
}

/**
 * @brief �\�[�g�p�̔ԍ��ݒ�B
 * @param[in] order �w�肷��ԍ�
 */
void C_GameObject::SetSortOrder(int order) {
	sortOrder = order;
}

/**
 * @brief Rigidbody�������ǂ������擾
 * @param[in] Rigidbody�����ǂ���
 */
bool C_GameObject::GetIsRigidbody() {
	return isRigidbody;
}

/**
 * @brief Rigidbody�������ǂ�����ݒ�B��{�I��Rigidbody�n����̌Ăяo���ŁB
 * @param[in] order �w�肷��ԍ�
 */
void C_GameObject::SetIsRigidbody(bool flg) {
	isRigidbody = flg;
}

/**
 * @brief Rigidbody�̓����蔻�肩��Ă΂��B����������Component�Ɋe�����B
 * @param[in] collision ���������I�u�W�F�N�g
 */
void C_GameObject::OnCollision(C_GameObject* collision) {
	auto colItr = std::find(oldColList.begin(), oldColList.end(), collision->objectID);
	bool isExist = colItr != oldColList.end();
	
	currentColList.push_back(collision->objectID);
	//���ɓ������Ă�
	if (isExist) {
		std::vector<std::unique_ptr<C_Component>>::iterator itr;
		for (itr = componentList.begin(); itr != componentList.end(); itr++) {
			(*itr)->OnCollisionStay(collision);
		}
		return;
	}
	//�O�܂œ������ĂȂ�����
	std::vector<std::unique_ptr<C_Component>>::iterator itr;
	for (itr = componentList.begin(); itr != componentList.end(); itr++) {
		(*itr)->OnCollisionEnter(collision);
	}
}
//=======================================================================================
//	End of File
//=======================================================================================
