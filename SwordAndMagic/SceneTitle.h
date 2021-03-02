/**
 * @file SceneTitle.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Scene.h"

class C_GameObject;
/**
 * @class C_SceneTitle
 * @brief タイトルシーン用クラス。ベースシーンを継承。
 */
class C_SceneTitle : public C_Scene {
private:
public:
	C_SceneTitle();
	~C_SceneTitle();
	void SpawnSceneObject();
};