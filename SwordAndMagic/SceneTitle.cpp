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

	//�T�E���h�t�@�C����ǂݍ��ށB�T�E���h�����₷����SoundName��enum�ɒǉ����Ă��������BStopSound�������B
	SoundManager::GetInstance().SetSound(SOUND_BGM01, L"data/sound/bgm.wav");

	//�ǂݍ��񂾃T�E���h���Đ�
	SoundManager::GetInstance().PlayLoopSound(SOUND_BGM01);


	//�����̍쐬
	LightInfo light;

	//����
	light.direction[0] = 0.0f;
	light.direction[1] = -0.5f;
	light.direction[2] = 0.5f;

	//�ʒu
	light.pos[0] = 0.0f;
	light.pos[1] = 500.0f;
	light.pos[2] = -100.0f;

	//type = 0 �_���� type = 1 �X�|�b�g���C�g
	light.type = 1;

	//�����̋���
	light.power = 20000;

	//0��1�͌����W��(�傫������قǋ����ɉ����Č�������)�B�X�|�b�g���C�g�̎��̂݁A2���ŏ��p�x�A3���ő�p�x(�̂͂�)
	light.value[0] = 6.0f;
	light.value[1] = 15.0f;
	light.value[2] = 0.0f;
	light.value[3] = 60.0f;

	//�����̓o�^
	ShaderLightBuffer::GetInstance().Register(light);


	//�V�[����ɃI�u�W�F�N�g��u�����@
	//�܂�GameObject�����
	std::unique_ptr<C_GameObject> obj = std::make_unique<C_GameObject>(this);

	//�ʒu���ړ��������肷��B�ق�Unity�Ɠ���������move�Ƃ�Rotate�Ƃ�������
	obj->transform.move(DirectX::XMFLOAT3(160, -200, 300));

	//AddComponent�ŋ@�\�ǉ��B�V�����@�\�𑝂₵��������Component���p�������V�����N���X����낤�B
	//���f���̓f�[�^�p�X/GameObject�̃|�C���^�A���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�A�A�E�g���C��(true/false)�Őݒ�B
	//���_�V�F�[�_�[�ȍ~�̈����͂Ȃ��Ă����v
	obj->AddComponent(std::make_unique<ModelDataComponent>("data/model/soldier4.fbx", obj.get(), (LPCSTR)NULL, "FbxModelPixelToon",true));
	obj->AddComponent(std::make_unique<RotationComponent>(obj.get(), 1));

	//�V�[���ɃI�u�W�F�N�g��o�^
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

	//�����I�u�W�F�N�g��RigidDynamic��RigidStatic������
	//�����̓I�u�W�F�N�g�|�C���^�AXMFLOAT3(�ÓI���C�A���I���C�A���ˌW��)�A�����蔻��`��
	//RigidBody�n�𑕒������I�u�W�F�N�g��Component�ɑ΂���
	//OnCollision�n���Ăяo����(�g������Unity�Ɠ����ATitle.cpp���Q��)
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

	//UI�̍쐬
	//������ƕ��G
	obj = std::make_unique<C_GameObject>(this);
	std::unique_ptr<C_GameObject> obj2 = std::make_unique<C_GameObject>(this);

	//UI�쐬�̊�{�B2D�t���O��On�ɂ���
	obj->Set2DFlg(true);
	obj2->Set2DFlg(true);
	obj->transform.setScale(DirectX::XMFLOAT3(0.5f, 0.25f, 1));
	obj2->transform.setScale(DirectX::XMFLOAT3(0.5f, 0.25f, 1));
	obj->transform.move(DirectX::XMFLOAT3(0.0f, -0.5f, 0.0f));
	obj2->transform.move(DirectX::XMFLOAT3(0.0f, -0.8f, 0.0f));

	//2D�I�u�W�F�N�g
	std::unique_ptr<PolygonComponent> pol = std::make_unique<PolygonComponent>(L"data/texture/startUI.png", obj.get());
	std::unique_ptr<PolygonComponent> pol2 = std::make_unique<PolygonComponent>(L"data/texture/endUI.png", obj2.get());

	//�ڂ���������GameStartUI���Q��
	std::unique_ptr<GameStartUI> gsUI = std::make_unique<GameStartUI>(obj.get(),pol.get(),true);
	std::unique_ptr<GameStartUI> gsUI2 = std::make_unique<GameStartUI>(obj2.get(),pol2.get(),false);
	obj->AddComponent(std::move(pol));
	obj2->AddComponent(std::move(pol2));

	//SetDownUI�Łu���{�^�������������ɑI�������{�^���v��o�^���Ă���
	gsUI->SetDownUI(gsUI2.get());
	//SetUpUI�Łu��{�^�������������ɑI�������{�^���v��o�^���Ă���
	gsUI2->SetUpUI(gsUI.get());
	obj->AddComponent(std::move(gsUI));
	obj2->AddComponent(std::move(gsUI2));
	AddObject(std::move(obj));
	AddObject(std::move(obj2));
}