/**
 * @file TitleMenu.h
 * @author MIURA DAIKI
 * @date 2020/02/20
 */
#pragma once
#include "SelectableUI.h"
#include "GameObj.h"
#include "Polygon.h"
#include "SceneController.h"
#include "Effect.h"
#include <memory>
 /**
  * @class GameStartUI
  * @brief SelectableUIComponentåpè≥
  */
class GameStartUI : public SelectableUIComponent
{
private:
	bool m_isSelected = false;
	PolygonComponent* m_Polygon;
public:
	GameStartUI(C_GameObject* obj, PolygonComponent* pol,bool defaultState) : SelectableUIComponent(obj, defaultState){
		m_Polygon = pol;
		m_isSelected = defaultState;
	}
	~GameStartUI() {

	}
	void OnPlay() {
		std::unique_ptr<C_GameObject> obj = std::make_unique<C_GameObject>(SceneController::GetInstance().GetCurrentScene());
		obj->transform.move(DirectX::XMFLOAT3(0, 0, 250));
		obj->transform.setScale(DirectX::XMFLOAT3(30, 30, 30));
		obj->AddComponent(std::make_unique<EffectComponent>(u"data/fx/Light.efk", obj.get(), -1.2f));
		SceneController::GetInstance().GetCurrentScene()->AddObject(std::move(obj));
	}
	void Update() {
		SelectableUIComponent::Update();
		if (m_isSelected) {
			m_Polygon->SetPolygonAlpha(fabs(sin(timeGetTime() * 0.001f)));
		}
	}
	void OnActive() {
		m_isSelected = true;
	}
	void OnInactive() {
		m_isSelected = false;
		m_Polygon->SetPolygonAlpha(1);
	}
};


