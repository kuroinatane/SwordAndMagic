#include "SceneTitle.h"
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
#include "ShaderLight.h"
#include "TitleMenu.h"
#include "Effect.h"

C_SceneTitle::C_SceneTitle() {
}
C_SceneTitle::~C_SceneTitle() {

}

void C_SceneTitle::SpawnSceneObject() {

	SoundManager::GetInstance().SetSound(SOUND_BGM01, L"data/sound/bgm.wav");
	SoundManager::GetInstance().PlayLoopSound(SOUND_BGM01);

	LightInfo light;
	light.direction[0] = 0.0f;
	light.direction[1] = -0.5f;
	light.direction[2] = 0.5f;
	light.pos[0] = 0.0f;
	light.pos[1] = 500.0f;
	light.pos[2] = -100.0f;
	light.type = 1;
	light.power = 20000;
	light.value[0] = 6.0f;
	light.value[1] = 15.0f;
	light.value[2] = 0.0f;
	light.value[3] = 60.0f;

	ShaderLightBuffer::GetInstance().Register(light);

	std::unique_ptr<C_GameObject> obj = std::make_unique<C_GameObject>(this);
	obj->transform.move(DirectX::XMFLOAT3(160, -200, 300));
	obj->AddComponent(std::make_unique<ModelDataComponent>("data/model/soldier4.fbx", obj.get(), (LPCSTR)NULL, "FbxModelPixelToon",true));
	obj->AddComponent(std::make_unique<RotationComponent>(obj.get(), 1));
	AddObject(std::move(obj));

	obj = std::make_unique<C_GameObject>(this);
	obj->transform.move(DirectX::XMFLOAT3(-160, -200, 300));
	obj->AddComponent(std::make_unique<ModelDataComponent>("data/model/cultist.fbx", obj.get(), (LPCSTR)NULL, "FbxModelPixelToon",true));
	obj->AddComponent(std::make_unique<RotationComponent>(obj.get(), -1));
	AddObject(std::move(obj));

	obj = std::make_unique<C_GameObject>(this);
	obj->transform.setPosition(DirectX::XMFLOAT3(0, -300, 950));
	obj->transform.Rotate(DirectX::XMFLOAT3(0, 180, 0));
	obj->transform.setScale(DirectX::XMFLOAT3(2, 2, 2));
	obj->AddComponent(std::make_unique<ModelDataComponent>("data/model/house.fbx", obj.get(), (LPCSTR)NULL, (LPCSTR)NULL));
	AddObject(std::move(obj));

	obj = std::make_unique<C_GameObject>(this);
	obj->transform.move(DirectX::XMFLOAT3(0, 500, 300));
	obj->transform.setScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	obj->transform.Rotate(DirectX::XMFLOAT3(180, 90, 0));
	obj->AddComponent(std::make_unique<ModelDataComponent>("data/model/title.fbx", obj.get(), (LPCSTR)NULL, (LPCSTR)NULL));
	obj->AddComponent(std::make_unique<TitleComponent>(obj.get()));
	obj->AddComponent(std::make_unique<RigidDynamicComponent>(obj.get(),
		DirectX::XMFLOAT3(0.5f, 0.5f, 0.6f),
		physx::PxBoxGeometry(40.0f, 5.0f, 40.0f),false));
	AddObject(std::move(obj));

	obj = std::make_unique<C_GameObject>(this);
	obj->transform.move(DirectX::XMFLOAT3(0, 100, 300));
	obj->AddComponent(std::make_unique<RigidStaticComponent>(obj.get(),
		DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f),
		physx::PxBoxGeometry(200.0f, 10.0f, 200.0f)));
	AddObject(std::move(obj));

	obj = std::make_unique<C_GameObject>(this);
	std::unique_ptr<C_GameObject> obj2 = std::make_unique<C_GameObject>(this);
	obj->Set2DFlg(true);
	obj2->Set2DFlg(true);
	obj->transform.setScale(DirectX::XMFLOAT3(0.5f, 0.25f, 1));
	obj2->transform.setScale(DirectX::XMFLOAT3(0.5f, 0.25f, 1));
	obj->transform.move(DirectX::XMFLOAT3(0.0f, -0.5f, 0.0f));
	obj2->transform.move(DirectX::XMFLOAT3(0.0f, -0.8f, 0.0f));
	std::unique_ptr<PolygonComponent> pol = std::make_unique<PolygonComponent>(L"data/texture/startUI.png", obj.get());
	std::unique_ptr<PolygonComponent> pol2 = std::make_unique<PolygonComponent>(L"data/texture/endUI.png", obj2.get());

	std::unique_ptr<GameStartUI> gsUI = std::make_unique<GameStartUI>(obj.get(),pol.get(),true);
	std::unique_ptr<GameStartUI> gsUI2 = std::make_unique<GameStartUI>(obj2.get(),pol2.get(),false);
	obj->AddComponent(std::move(pol));
	obj2->AddComponent(std::move(pol2));
	gsUI->SetDownUI(gsUI2.get());
	gsUI2->SetUpUI(gsUI.get());
	obj->AddComponent(std::move(gsUI));
	obj2->AddComponent(std::move(gsUI2));
	AddObject(std::move(obj));
	AddObject(std::move(obj2));
}