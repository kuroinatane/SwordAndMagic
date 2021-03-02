/**
 * @file SelectableUI.h
 * @author MIURA DAIKI
 * @date 2020/02/20
 */
#pragma once
#include "Component.h"

class C_GameObject;
/**
 * @class SelectableUIComponent
 * @brief 選択式UI用のコンポーネントはこれを継承する。上下左右のキー入力で次のUIに行く基本設定。OnPlayとOnActive、OnInactiveをオーバーライドして決定時、選択時、非選択時挙動を定義できる。
 */
class SelectableUIComponent : public C_Component
{
protected:
	C_GameObject* m_pObj;
	SelectableUIComponent* m_UpUI = NULL;
	SelectableUIComponent* m_DownUI = NULL;
	SelectableUIComponent* m_RightUI = NULL;
	SelectableUIComponent* m_LeftUI = NULL;
	bool isActive = false;
public:
	SelectableUIComponent(C_GameObject* obj, bool defaultState);
	~SelectableUIComponent();
	virtual void Update();
	virtual void OnPlay();
	virtual void OnActive();
	virtual void OnInactive();
	void SetUpUI(SelectableUIComponent* pUp);
	void SetDownUI(SelectableUIComponent* pDown);
	void SetRightUI(SelectableUIComponent* pRight);
	void SetLeftUI(SelectableUIComponent* pLeft);
	void SetActive(bool active);
	void ToUp();
	void ToDown();
	void ToRight();
	void ToLeft();
};

