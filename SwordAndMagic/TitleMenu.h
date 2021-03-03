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
  * @brief SelectableUIComponent継承
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

	//このボタンが設定されていて、尚且つ決定ボタンが押された時の挙動を設定
	void OnPlay() {
		//effekseerからのエフェクト読み込み
		std::unique_ptr<C_GameObject> obj = std::make_unique<C_GameObject>(SceneController::GetInstance().GetCurrentScene());
		obj->transform.move(DirectX::XMFLOAT3(0, 0, 250));
		obj->transform.setScale(DirectX::XMFLOAT3(30, 30, 30));
		obj->AddComponent(std::make_unique<EffectComponent>(u"data/fx/Light.efk", obj.get(), -1.2f));
		SceneController::GetInstance().GetCurrentScene()->AddObject(std::move(obj));
	}

	//毎フレームの呼び出し
	void Update() {
		SelectableUIComponent::Update();
		if (m_isSelected) {
			m_Polygon->SetPolygonAlpha(fabs(sin(timeGetTime() * 0.001f)));
		}
	}

	//このボタンがアクティブになった時の処理
	void OnActive() {
		m_isSelected = true;
	}

	//このボタンが非アクティブになった時の処理
	void OnInactive() {
		m_isSelected = false;
		m_Polygon->SetPolygonAlpha(1);
	}
};


