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

	//サウンドファイルを読み込む。サウンド数増やす時はSoundNameのenumに追加してください。StopSoundもあるよ。
	SoundManager::GetInstance().SetSound(SOUND_BGM01, L"data/sound/bgm.wav");

	//読み込んだサウンドを再生
	SoundManager::GetInstance().PlayLoopSound(SOUND_BGM01);


	//光源の作成
	LightInfo light;

	//向き
	light.direction[0] = 0.0f;
	light.direction[1] = -0.5f;
	light.direction[2] = 0.5f;

	//位置
	light.pos[0] = 0.0f;
	light.pos[1] = 500.0f;
	light.pos[2] = -100.0f;

	//type = 0 点光源 type = 1 スポットライト
	light.type = 1;

	//光源の強さ
	light.power = 20000;

	//0と1は減衰係数(大きくするほど距離に応じて光が減衰)。スポットライトの時のみ、2が最小角度、3が最大角度(のはず)
	light.value[0] = 6.0f;
	light.value[1] = 15.0f;
	light.value[2] = 0.0f;
	light.value[3] = 60.0f;

	//光源の登録
	ShaderLightBuffer::GetInstance().Register(light);


	//シーン上にオブジェクトを置く方法
	//まずGameObjectを作る
	std::unique_ptr<C_GameObject> obj = std::make_unique<C_GameObject>(this);

	//位置を移動させたりする。ほぼUnityと同じ感じでmoveとかRotateとかがある
	obj->transform.move(DirectX::XMFLOAT3(160, -200, 300));

	//AddComponentで機能追加。新しい機能を増やしたい時はComponentを継承した新しいクラスを作ろう。
	//モデルはデータパス/GameObjectのポインタ、頂点シェーダー、ピクセルシェーダー、アウトライン(true/false)で設定。
	//頂点シェーダー以降の引数はなくても大丈夫
	obj->AddComponent(std::make_unique<ModelDataComponent>("data/model/soldier4.fbx", obj.get(), (LPCSTR)NULL, "FbxModelPixelToon",true));
	obj->AddComponent(std::make_unique<RotationComponent>(obj.get(), 1));

	//シーンにオブジェクトを登録
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
	obj->AddComponent(std::make_unique<ModelDataComponent>("data/model/house.fbx", obj.get()));
	AddObject(std::move(obj));

	obj = std::make_unique<C_GameObject>(this);
	obj->transform.move(DirectX::XMFLOAT3(0, 500, 300));
	obj->transform.setScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	obj->transform.Rotate(DirectX::XMFLOAT3(180, 90, 0));
	obj->AddComponent(std::make_unique<ModelDataComponent>("data/model/title.fbx", obj.get()));
	obj->AddComponent(std::make_unique<TitleComponent>(obj.get()));

	//物理オブジェクトはRigidDynamicとRigidStaticがある
	//引数はオブジェクトポインタ、XMFLOAT3(静的摩擦、動的摩擦、反射係数)、当たり判定形状
	//RigidBody系を装着したオブジェクトはComponentに対して
	//OnCollision系を呼び出せる(使い方はUnityと同じ、Title.cppを参照)
	obj->AddComponent(std::make_unique<RigidDynamicComponent>(obj.get(),
		DirectX::XMFLOAT3(0.5f, 0.5f, 0.6f),
		physx::PxBoxGeometry(40.0f, 5.0f, 40.0f),100,false));
	AddObject(std::move(obj));

	obj = std::make_unique<C_GameObject>(this);
	obj->transform.move(DirectX::XMFLOAT3(0, 100, 300));
	obj->AddComponent(std::make_unique<RigidStaticComponent>(obj.get(),
		DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f),
		physx::PxBoxGeometry(400.0f, 10.0f, 400.0f)));
	AddObject(std::move(obj));

	//UIの作成
	//ちょっと複雑
	obj = std::make_unique<C_GameObject>(this);
	std::unique_ptr<C_GameObject> obj2 = std::make_unique<C_GameObject>(this);

	//UI作成の基本。2DフラグをOnにする
	obj->Set2DFlg(true);
	obj2->Set2DFlg(true);
	obj->transform.setScale(DirectX::XMFLOAT3(0.5f, 0.25f, 1));
	obj2->transform.setScale(DirectX::XMFLOAT3(0.5f, 0.25f, 1));
	obj->transform.move(DirectX::XMFLOAT3(0.0f, -0.5f, 0.0f));
	obj2->transform.move(DirectX::XMFLOAT3(0.0f, -0.8f, 0.0f));

	//2Dオブジェクト
	std::unique_ptr<PolygonComponent> pol = std::make_unique<PolygonComponent>(L"data/texture/startUI.png", obj.get());
	std::unique_ptr<PolygonComponent> pol2 = std::make_unique<PolygonComponent>(L"data/texture/endUI.png", obj2.get());

	//詳しい作り方はGameStartUIを参照
	std::unique_ptr<GameStartUI> gsUI = std::make_unique<GameStartUI>(obj.get(),pol.get(),true);
	std::unique_ptr<GameStartUI> gsUI2 = std::make_unique<GameStartUI>(obj2.get(),pol2.get(),false);
	obj->AddComponent(std::move(pol));
	obj2->AddComponent(std::move(pol2));

	//SetDownUIで「下ボタンを押した時に選択されるボタン」を登録している
	gsUI->SetDownUI(gsUI2.get());
	//SetUpUIで「上ボタンを押した時に選択されるボタン」を登録している
	gsUI2->SetUpUI(gsUI.get());
	obj->AddComponent(std::move(gsUI));
	obj2->AddComponent(std::move(gsUI2));
	AddObject(std::move(obj));
	AddObject(std::move(obj2));
}