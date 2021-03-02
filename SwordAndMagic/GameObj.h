/**
 * @file GameObj.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */

#pragma once
#include "Main.h"
#include "Transform.h"
#include <memory>
#include <vector>
#include <iterator>


enum ObjectTag {
	Tag_NULL
};

enum ObjectLayer {
	Layer_NULL
};

class C_Component;
class C_Scene;

/**
 * @class C_GameObject
 * @brief �I�u�W�F�N�g�N���X�B��{�I�ɂ͌p�������A
 *		  AddComponent�ŋ@�\��t�������`�̎����B
 *		  Transform�͑S�ẴI�u�W�F�N�g�����̂Ńf�t�H���g�����ƂȂ邪�ACollider�n�̓R���|�[�l���g�Ƃ��Ď����B
 */
class C_GameObject
{
private:
	bool is2D = false;
	int objectID;		//!< @brief �I�u�W�F�N�g���ʎq�AGet�͊O�����炳���邪Set�͕K���������̂ݍs���B
	int sortOrder = 0;		//!< @brief �\�[�g�p�̔ԍ��A�w�肪�Ȃ����0�B

	std::vector<std::unique_ptr<C_Component>> componentList; //!< @brief ���̃I�u�W�F�N�g�ɕt�����Ă���R���|�[�l���g�̃��X�g�Badd�͂��邪remove�͍��̏������\�薳��(�K�v�ɂȂ�����������邩������Ȃ�)
	std::vector<std::unique_ptr<C_Component>> addComponentList; //!< @brief ���̃I�u�W�F�N�g�ɕt��������\��̃R���|�[�l���g�̃��X�g
	C_Scene* m_pScene; //!< @brief ���̃I�u�W�F�N�g�����݂���V�[���ւ̃|�C���^�B���̃|�C���^�̎w���悪�j������鎞�͂��̃I�u�W�F�N�g�������I�ɔj������鎖�ɂȂ�B
public:
	C_Transform transform;
	ObjectTag objectTag;		//!< @brief �I�u�W�F�N�g�ꊇ���ʗp�̃^�O
	ObjectLayer objectLayer;		//!< @brief �����蔻�莯�ʗp�̃��C���[

	C_GameObject(C_Scene* scene);
	~C_GameObject();

	void Start();

	void Update();
	void Draw();
	void Destroy();

	void AddComponent(std::unique_ptr<C_Component> component);

	template <class T> T* GetComponent()
	{
		T* component;

		std::vector<std::unique_ptr<C_Component>>::iterator itr;
		for (itr = componentList.begin(); itr != componentList.end(); itr++) {
			component = dynamic_cast<T*>(itr->get());
			if (component != nullptr)
				return component;
		}
		return nullptr;
	}

	int GetID();
	int GetSortOrder();
	void SetSortOrder(int order);
	bool Getis2D();
	void Set2DFlg(bool flg);
};

//=======================================================================================
//	End of File
//=======================================================================================
