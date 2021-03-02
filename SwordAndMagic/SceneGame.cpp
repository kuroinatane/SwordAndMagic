#include "SceneGame.h"
#include "GameObj.h"
#include "ModelData.h"
#include "Curtain.h"
#include "Title.h"
#include "Rotation.h"
#include "Polygon.h"
#include "BasicMesh.h"
#include "SoundManager.h"
#include "RigidStatic.h"
#include "RigidDynamic.h"
#include "EffectGenerator.h"
#include "Effect.h"

C_SceneGame::C_SceneGame() {

}
C_SceneGame::~C_SceneGame() {

}

void C_SceneGame::SpawnSceneObject() {

	std::unique_ptr<C_GameObject> obj = std::make_unique<C_GameObject>(this);
	obj->transform.move(DirectX::XMFLOAT3(125, 260, -30));
	obj->AddComponent(std::make_unique<CurtainComponent>(obj.get(), -1, false));
	AddObject(std::move(obj));

	obj = std::make_unique<C_GameObject>(this);
	obj->transform.move(DirectX::XMFLOAT3(-125, 260, -30));
	obj->AddComponent(std::make_unique<CurtainComponent>(obj.get(), 1, false));
	AddObject(std::move(obj));

	obj = std::make_unique<C_GameObject>(this);
	obj->transform.move(DirectX::XMFLOAT3(0, 0, 200));
	obj->transform.setScale(DirectX::XMFLOAT3(30, 30, 30));
	obj->AddComponent(std::make_unique<EffectComponent>(u"data/fx/Laser01.efk", obj.get(), 1.2f));
	AddObject(std::move(obj));
}