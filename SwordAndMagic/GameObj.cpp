// ゲーム オブジェクト [GameObj.cpp]
#include "GameObj.h"
#include "Scene.h"
#include "ObjectRegister.h"
#include "Component.h"
#include "SceneController.h"
#include "Scene.h"

using namespace DirectX;

/**
 * @brief コンストラクタ。ObjectRegister経由でIDを振り分けてもらう。
 * @param[in] シーンへのポインタ
 */
C_GameObject::C_GameObject(C_Scene* scene)
{
	objectID = ObjectRegister::GetInstance().RegisterObject();
	m_pScene = scene;
}

/**
 * @brief デストラクタ。ObjectRegisterに使用していたIDを返す。
 */
C_GameObject::~C_GameObject()
{
	ObjectRegister::GetInstance().UnregisterObject(objectID);
}

/**
 * @brief 更新関数。addComponentもここで行われる。
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
	////コンテナが空になるまでループし続ける
	//while (!addComponentList.empty()) {
	//	//先頭の要素をコンテナ内へ移動させる
	//	componentList.push_back(std::move(addComponentList.front()));
	//}
}
/**
 * @brief 描画関数
 */
void C_GameObject::Draw()
{
	std::vector<std::unique_ptr<C_Component>>::iterator itr;
	for (itr = componentList.begin(); itr != componentList.end(); itr++) {
		(*itr)->Draw();
	}
}

/**
 * @brief 初期化時に呼ばれるように設定されているComponentの関数を全て呼ぶ
 */
void C_GameObject::Start() 
{
	std::vector<std::unique_ptr<C_Component>>::iterator itr;

	//初期化時のaddComponentを処理する
	for (itr = addComponentList.begin(); itr != addComponentList.end(); itr++) {
		componentList.push_back(std::move(*itr));
	}
	addComponentList.clear();

	for (itr = componentList.begin(); itr != componentList.end(); itr++) {
		(*itr)->Start();
	}
}

/**
 * @brief オブジェクトを破棄し、破棄時に呼ばれるように設定されているComponentの関数を全て呼ぶ
 */
void C_GameObject::Destroy() 
{
	std::vector<std::unique_ptr<C_Component>>::iterator itr;
	for (itr = componentList.begin(); itr != componentList.end(); itr++) {
		(*itr)->OnDestroy();
	}
	//シーン上に存在していればシーンから削除してもらう(存在していなければ勝手に消えるはず)
	if(m_pScene)
	m_pScene->DeleteObject(objectID);
}

/**
 * @brief 作成したコンポーネントを渡してこちらでの管理に切り替える
 * @param[in] component make_uniqueで作ったコンポーネントのユニークポインタ
 */
void C_GameObject::AddComponent(std::unique_ptr<C_Component> component) {
	addComponentList.push_back(std::move(component));
}

/**
 * @brief オブジェクトの番号を取得する。
 * @return オブジェクトの番号
 */
int C_GameObject::GetID() 
{
	return objectID;
}

/**
 * @brief 2Dオブジェクト(=スクリーン座標に直接描画する)かどうかのフラグを返す。(描画用)
 * @return 2Dオブジェクトならtrue/3Dオブジェクトならfalse
 */
bool C_GameObject::Getis2D() {
	return is2D;
}

/**
 * @brief 2Dオブジェクト(=スクリーン座標に直接描画する)かどうかのフラグをセットする。(描画用)
 * @param[in] flg 2Dオブジェクトならtrue、3Dオブジェクトならfalse
 */
void C_GameObject::Set2DFlg(bool flg) {
	is2D = flg;
}

/**
 * @brief ソート用の番号取得。
 * @return ソート用の番号 
 */
int C_GameObject::GetSortOrder() {
	return sortOrder;
}

/**
 * @brief ソート用の番号設定。
 * @param[in] order 指定する番号
 */
void C_GameObject::SetSortOrder(int order) {
	sortOrder = order;
}
//=======================================================================================
//	End of File
//=======================================================================================
