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
 * @brief オブジェクトクラス。基本的には継承せず、
 *		  AddComponentで機能を付け足す形の実装。
 *		  Transformは全てのオブジェクトが持つのでデフォルト実装となるが、Collider系はコンポーネントとして実装。
 */
class C_GameObject
{
private:
	bool is2D = false;
	int objectID;		//!< @brief オブジェクト識別子、Getは外部からさせるがSetは必ず生成時のみ行う。
	int sortOrder = 0;		//!< @brief ソート用の番号、指定がなければ0。

	std::vector<std::unique_ptr<C_Component>> componentList; //!< @brief このオブジェクトに付属しているコンポーネントのリスト。addはするがremoveは今の所実装予定無し(必要になったら実装するかもしれない)
	std::vector<std::unique_ptr<C_Component>> addComponentList; //!< @brief このオブジェクトに付属させる予定のコンポーネントのリスト
	C_Scene* m_pScene; //!< @brief このオブジェクトが存在するシーンへのポインタ。このポインタの指す先が破棄される時はこのオブジェクトも自動的に破棄される事になる。
public:
	C_Transform transform;
	ObjectTag objectTag;		//!< @brief オブジェクト一括識別用のタグ
	ObjectLayer objectLayer;		//!< @brief 当たり判定識別用のレイヤー

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
